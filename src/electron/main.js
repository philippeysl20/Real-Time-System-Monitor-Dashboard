const { app, BrowserWindow } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

const isDev = !app.isPackaged;
let backendProcess;

function resolveBackendExecutable() {
  const candidates = [
    path.join(app.getAppPath(), 'build', 'src', 'backend', 'monitor.exe'),
    path.join(app.getAppPath(), 'cmake-build-debug', 'src', 'backend', 'monitor.exe'),
    path.join(process.cwd(), 'build', 'src', 'backend', 'monitor.exe'),
    path.join(process.cwd(), 'cmake-build-debug', 'src', 'backend', 'monitor.exe'),
  ];

  return candidates.find((candidate) => require('fs').existsSync(candidate));
}

function startBackend() {
  const executable = resolveBackendExecutable();

  if (!executable) {
    console.warn('Backend executable not found. Expected monitor.exe in build output.');
    return;
  }

  backendProcess = spawn(executable, [], {
    windowsHide: true,
  });

  backendProcess.on('error', (error) => {
    console.error('Failed to start backend process:', error);
  });
}

function stopBackend() {
  if (backendProcess && !backendProcess.killed) {
    backendProcess.kill();
  }
}

function createWindow() {
  const win = new BrowserWindow({
    width: 1440,
    height: 940,
    minWidth: 1180,
    minHeight: 760,
    backgroundColor: '#08111f',
    titleBarStyle: 'hiddenInset',
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
    }
  });

  if (isDev) {
    win.loadURL('http://localhost:5173');
  } else {
    win.loadFile(path.join(__dirname, '../frontend/dist/index.html'));
  }
}

app.whenReady().then(() => {
  startBackend();
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('before-quit', stopBackend);
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});
