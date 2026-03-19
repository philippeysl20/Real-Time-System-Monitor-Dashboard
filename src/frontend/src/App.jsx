import { useEffect, useMemo, useRef, useState } from 'react';
import {
  CpuChart,
  CpuHistoryChart,
  DiskChart,
  DiskHistoryChart,
  MemoryChart,
  MemoryHistoryChart,
} from './components/Charts';
import './App.css';

const HISTORY_LIMIT = 36;

const initialCpu = {
  modelName: 'Waiting for CPU data',
  CPUUsage: 0,
  CPUSpeed: 0,
  CPUProcesses: 0,
  CPUThreads: 0,
};

const initialMemory = {
  totalPhysical: 0,
  totalAvailablePhysical: 0,
  usedPhysical: 0,
  percentageUsed: 0,
};

function formatPercent(value) {
  return `${Number(value ?? 0).toFixed(1)}%`;
}

function formatGigabytes(value) {
  return `${Number(value ?? 0).toFixed(1)} GB`;
}

function formatMegabytes(value) {
  return `${Number(value ?? 0).toFixed(1)} MB/s`;
}

function formatCompactBytes(value) {
  const units = ['B', 'KB', 'MB', 'GB', 'TB'];
  let output = Number(value ?? 0);
  let unitIndex = 0;

  while (output >= 1024 && unitIndex < units.length - 1) {
    output /= 1024;
    unitIndex += 1;
  }

  return `${output.toFixed(output >= 10 || unitIndex === 0 ? 0 : 1)} ${units[unitIndex]}`;
}

function appendHistory(history, value) {
  const nextHistory = [...history, Number(value ?? 0)];
  return nextHistory.slice(-HISTORY_LIMIT);
}

function App() {
  const [connectionState, setConnectionState] = useState('Connecting');
  const [cpu, setCpu] = useState(initialCpu);
  const [memory, setMemory] = useState(initialMemory);
  const [drives, setDrives] = useState([]);
  const [cpuHistory, setCpuHistory] = useState(Array(HISTORY_LIMIT).fill(0));
  const [memoryHistory, setMemoryHistory] = useState(Array(HISTORY_LIMIT).fill(0));
  const [diskHistory, setDiskHistory] = useState(Array(HISTORY_LIMIT).fill(0));
  const socketRef = useRef(null);
  const reconnectTimeoutRef = useRef(null);

  useEffect(() => {
    let isDisposed = false;

    const clearTimers = () => {
      if (reconnectTimeoutRef.current) {
        window.clearTimeout(reconnectTimeoutRef.current);
        reconnectTimeoutRef.current = null;
      }
    };

    const connect = () => {
      if (isDisposed) {
        return;
      }

      setConnectionState('Connecting');
      const socket = new WebSocket('ws://localhost:3000');
      socketRef.current = socket;

      socket.addEventListener('open', () => {
        setConnectionState('Live');
      });

      socket.addEventListener('close', () => {
        clearTimers();
        setConnectionState('Disconnected');

        if (!isDisposed) {
          reconnectTimeoutRef.current = window.setTimeout(connect, 1800);
        }
      });

      socket.addEventListener('error', () => {
        setConnectionState('Error');
      });

      socket.addEventListener('message', (event) => {
        let data;

        try {
          data = JSON.parse(event.data);
        } catch {
          return;
        }

        if (data.type === 'CPU-data') {
          setCpu(data);
          setCpuHistory((history) => appendHistory(history, data.CPUUsage));
        } else if (data.type === 'Memory-data') {
          setMemory(data);
          setMemoryHistory((history) => appendHistory(history, data.percentageUsed));
        } else if (data.type === 'Disk-data') {
          const nextDrives = Array.isArray(data.drives) ? data.drives : [];
          setDrives(nextDrives);

          const aggregateThroughput = nextDrives.reduce((sum, drive) => {
            return sum + Number(drive.readBps ?? 0) + Number(drive.writeBps ?? 0);
          }, 0) / (1024 * 1024);

          setDiskHistory((history) => appendHistory(history, aggregateThroughput));
        }
      });
    };

    connect();

    return () => {
      isDisposed = true;
      clearTimers();

      if (socketRef.current && socketRef.current.readyState < WebSocket.CLOSING) {
        socketRef.current.close();
      }
    };
  }, []);

  const primaryDrive = useMemo(() => {
    if (!drives.length) {
      return null;
    }

    return [...drives].sort((left, right) => {
      const leftUsage = 1 - Number(left.freeBytes ?? 0) / Math.max(Number(left.totalBytes ?? 1), 1);
      const rightUsage = 1 - Number(right.freeBytes ?? 0) / Math.max(Number(right.totalBytes ?? 1), 1);
      return rightUsage - leftUsage;
    })[0];
  }, [drives]);

  const driveCards = useMemo(() => {
    return drives.map((drive) => {
      const totalBytes = Number(drive.totalBytes ?? 0);
      const freeBytes = Number(drive.freeBytes ?? 0);
      const usedBytes = Math.max(totalBytes - freeBytes, 0);
      const usagePercent = totalBytes > 0 ? (usedBytes / totalBytes) * 100 : 0;

      return {
        id: drive.path,
        name: drive.path,
        type: drive.type || 'NA',
        usagePercent,
        capacity: `${formatCompactBytes(usedBytes)} of ${formatCompactBytes(totalBytes)}`,
        readRate: formatMegabytes(Number(drive.readBps ?? 0) / (1024 * 1024)),
        writeRate: formatMegabytes(Number(drive.writeBps ?? 0) / (1024 * 1024)),
      };
    });
  }, [drives]);

  const primaryDriveUsage = primaryDrive
    ? ((Number(primaryDrive.totalBytes ?? 0) - Number(primaryDrive.freeBytes ?? 0)) / Math.max(Number(primaryDrive.totalBytes ?? 1), 1)) * 100
    : 0;

  const primaryDriveReadRate = primaryDrive ? Number(primaryDrive.readBps ?? 0) / (1024 * 1024) : 0;
  const primaryDriveWriteRate = primaryDrive ? Number(primaryDrive.writeBps ?? 0) / (1024 * 1024) : 0;

  const sidebarItems = [
    {
      name: 'CPU',
      accent: 'cpu',
      value: formatPercent(cpu.CPUUsage),
      detail: `${Number(cpu.CPUSpeed ?? 0).toFixed(0)} MHz`,
      chart: <CpuChart value={cpu.CPUUsage} />,
    },
    {
      name: 'Memory',
      accent: 'memory',
      value: `${formatGigabytes(memory.usedPhysical)} / ${formatGigabytes(memory.totalPhysical)}`,
      detail: formatPercent(memory.percentageUsed),
      chart: <MemoryChart value={memory.percentageUsed} />,
    },
    {
      name: primaryDrive?.path || 'Disk',
      accent: 'disk',
      value: formatPercent(primaryDriveUsage),
      detail: `${formatMegabytes(primaryDriveReadRate)} read`,
      chart: <DiskChart value={primaryDriveUsage} />,
    },
  ];

  return (
    <main className="dashboard-shell">
      <section className="dashboard-panel">
        <aside className="performance-sidebar">
          <div className="sidebar-header">
            <div>
              <p className="eyebrow">Performance</p>
              <h1>System Monitor</h1>
            </div>
            <div className={`status-pill status-${connectionState.toLowerCase()}`}>{connectionState}</div>
          </div>

          <div className="sidebar-list">
            {sidebarItems.map((item) => (
              <article key={item.name} className={`sidebar-card accent-${item.accent}`}>
                <div className="sidebar-chart">{item.chart}</div>
                <div className="sidebar-copy">
                  <span className="sidebar-label">{item.name}</span>
                  <strong>{item.value}</strong>
                  <span>{item.detail}</span>
                </div>
              </article>
            ))}
          </div>

          <div className="sidebar-footer">
            <span>Sample cadence</span>
            <strong>1.5 seconds</strong>
          </div>
        </aside>

        <section className="main-stage">
          <header className="hero-card">
            <div className="hero-copy">
              <p className="eyebrow">Overview</p>
              <h2>{cpu.modelName}</h2>
              <p>
                Task Manager performance tab clone - By Philippe Bourrachot.
              </p>
            </div>

            <div className="hero-metrics">
              <article>
                <span>CPU utilization</span>
                <strong>{formatPercent(cpu.CPUUsage)}</strong>
              </article>
              <article>
                <span>Memory in use</span>
                <strong>{formatGigabytes(memory.usedPhysical)}</strong>
              </article>
              <article>
                <span>Busiest drive</span>
                <strong>{primaryDrive?.path || 'Waiting...'}</strong>
              </article>
            </div>
          </header>

          <section className="chart-grid">
            <article className="chart-card chart-card-large">
              <div className="card-heading">
                <div>
                  <p className="eyebrow">CPU</p>
                  <h3>Utilization history</h3>
                </div>
                <strong>{formatPercent(cpu.CPUUsage)}</strong>
              </div>
              <div className="chart-frame">
                <CpuHistoryChart values={cpuHistory} />
              </div>
              <div className="metric-row">
                <div>
                  <span>Clock speed</span>
                  <strong>{Number(cpu.CPUSpeed ?? 0).toFixed(0)} MHz</strong>
                </div>
                <div>
                  <span>Processes</span>
                  <strong>{cpu.CPUProcesses ?? 0}</strong>
                </div>
                <div>
                  <span>Threads</span>
                  <strong>{cpu.CPUThreads ?? 0}</strong>
                </div>
              </div>
            </article>

            <article className="chart-card">
              <div className="card-heading">
                <div>
                  <p className="eyebrow">Memory</p>
                  <h3>Physical memory load</h3>
                </div>
                <strong>{formatPercent(memory.percentageUsed)}</strong>
              </div>
              <div className="chart-frame compact-chart">
                <MemoryHistoryChart values={memoryHistory} />
              </div>
              <div className="metric-row">
                <div>
                  <span>Used</span>
                  <strong>{formatGigabytes(memory.usedPhysical)}</strong>
                </div>
                <div>
                  <span>Available</span>
                  <strong>{formatGigabytes(memory.totalAvailablePhysical)}</strong>
                </div>
                <div>
                  <span>Total</span>
                  <strong>{formatGigabytes(memory.totalPhysical)}</strong>
                </div>
              </div>
            </article>

            <article className="chart-card">
              <div className="card-heading">
                <div>
                  <p className="eyebrow">Disk</p>
                  <h3>Aggregate throughput</h3>
                </div>
                <strong>{formatMegabytes(primaryDriveReadRate + primaryDriveWriteRate)}</strong>
              </div>
              <div className="chart-frame compact-chart">
                <DiskHistoryChart values={diskHistory} />
              </div>
              <div className="metric-row">
                <div>
                  <span>Read</span>
                  <strong>{formatMegabytes(primaryDriveReadRate)}</strong>
                </div>
                <div>
                  <span>Write</span>
                  <strong>{formatMegabytes(primaryDriveWriteRate)}</strong>
                </div>
                <div>
                  <span>Used space</span>
                  <strong>{formatPercent(primaryDriveUsage)}</strong>
                </div>
              </div>
            </article>
          </section>

          <section className="storage-section">
            <div className="section-heading">
              <div>
                <p className="eyebrow">Storage</p>
                <h3>Installed drives</h3>
              </div>
              <span>Labels explain each value so the numbers stay readable at a glance.</span>
            </div>

            <div className="drive-grid">
              {driveCards.length ? (
                driveCards.map((drive) => (
                  <article key={drive.id} className="drive-card">
                    <div className="drive-header">
                      <div>
                        <h4>{drive.name}</h4>
                        <span>Type: {drive.type}</span>
                      </div>
                      <strong>{formatPercent(drive.usagePercent)}</strong>
                    </div>

                    <div className="drive-bar">
                      <span style={{ width: `${Math.min(drive.usagePercent, 100)}%` }} />
                    </div>

                    <dl className="drive-stats">
                      <div>
                        <dt>Capacity used</dt>
                        <dd>{drive.capacity}</dd>
                      </div>
                      <div>
                        <dt>Read throughput</dt>
                        <dd>{drive.readRate}</dd>
                      </div>
                      <div>
                        <dt>Write throughput</dt>
                        <dd>{drive.writeRate}</dd>
                      </div>
                    </dl>
                  </article>
                ))
              ) : (
                <article className="drive-card drive-card-empty">
                  <h4>Waiting for storage metrics</h4>
                  <p>The backend has not sent disk information yet.</p>
                </article>
              )}
            </div>
          </section>
        </section>
      </section>
    </main>
  );
}

export default App;
