import React from 'react';
import { Doughnut, Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  ArcElement,
  CategoryScale,
  Filler,
  LinearScale,
  LineElement,
  PointElement,
  Tooltip,
  Legend,
} from 'chart.js';

ChartJS.register(
  ArcElement,
  CategoryScale,
  Filler,
  LinearScale,
  LineElement,
  PointElement,
  Tooltip,
  Legend,
);

function createDoughnutData(label, value, usedColor) {
  return {
    labels: ['Used', 'Free'],
    datasets: [
      {
        label: `${label} Usage`,
        data: [value, 100 - value],
        backgroundColor: [usedColor, '#e0e0e0'],
        borderWidth: 1,
      },
    ],
  };
}

const doughnutOptions = {
  responsive: true,
  maintainAspectRatio: false,
  cutout: '76%',
  plugins: {
    legend: {
      display: false,
    },
    tooltip: {
      callbacks: {
        label: (context) => `${context.label}: ${Number(context.raw).toFixed(1)}%`,
      },
    },
  },
};

function buildHistoryData(values, label, strokeColor, fillColor) {
  return {
    labels: values.map((_, index) => index + 1),
    datasets: [
      {
        label,
        data: values,
        fill: true,
        tension: 0.35,
        borderColor: strokeColor,
        backgroundColor: fillColor,
        pointRadius: 0,
        pointHitRadius: 12,
        borderWidth: 2,
      },
    ],
  };
}

function buildHistoryOptions({ min = 0, max = 100, unit = '%', stacked = false } = {}) {
  return {
    responsive: true,
    maintainAspectRatio: false,
    animation: false,
    interaction: {
      intersect: false,
      mode: 'index',
    },
    plugins: {
      legend: {
        display: false,
      },
      tooltip: {
        displayColors: false,
        callbacks: {
          label: (context) => `${context.dataset.label}: ${Number(context.raw).toFixed(1)}${unit}`,
        },
      },
    },
    scales: {
      x: {
        display: false,
        stacked,
      },
      y: {
        min,
        max,
        stacked,
        ticks: {
          color: 'rgba(160, 174, 192, 0.72)',
          callback: (value) => `${value}${unit}`,
        },
        grid: {
          color: 'rgba(148, 163, 184, 0.12)',
        },
      },
    },
  };
}

function CpuChart({ value }) {
  const data = createDoughnutData('CPU', value, 'rgba(96, 165, 250, 0.95)');
  return <Doughnut data={data} options={doughnutOptions} />;
}

function MemoryChart({ value }) {
  const data = createDoughnutData('Memory', value, 'rgba(52, 211, 153, 0.95)');
  return <Doughnut data={data} options={doughnutOptions} />;
}

function DiskChart({ value }) {
  const data = createDoughnutData('Disk', value, 'rgba(251, 191, 36, 0.95)');
  return <Doughnut data={data} options={doughnutOptions} />;
}

function CpuHistoryChart({ values }) {
  return (
    <Line
      data={buildHistoryData(values, 'CPU', 'rgba(96, 165, 250, 1)', 'rgba(59, 130, 246, 0.18)')}
      options={buildHistoryOptions()}
    />
  );
}

function MemoryHistoryChart({ values }) {
  return (
    <Line
      data={buildHistoryData(values, 'Memory', 'rgba(52, 211, 153, 1)', 'rgba(16, 185, 129, 0.16)')}
      options={buildHistoryOptions()}
    />
  );
}

function DiskHistoryChart({ values }) {
  const safeValues = values.length ? values : [0];
  const maxValue = Math.max(...safeValues, 1);

  return (
    <Line
      data={buildHistoryData(safeValues, 'Disk throughput', 'rgba(251, 191, 36, 1)', 'rgba(245, 158, 11, 0.16)')}
      options={buildHistoryOptions({ min: 0, max: Math.ceil(maxValue * 1.15), unit: ' MB/s' })}
    />
  );
}

export { CpuChart, CpuHistoryChart, DiskChart, DiskHistoryChart, MemoryChart, MemoryHistoryChart };
