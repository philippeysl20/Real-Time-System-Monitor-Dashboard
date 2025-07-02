const { app, BrowserWindow } = require('electron');
const path = require('path');

function createWindow() {
  const win = new BrowserWindow({
    width: 1000,
    height: 800,
    webPreferences: {
      nodeIntegration: true
    }
  });

  // For production (after vite build)
  win.loadFile(path.join(__dirname, '../frontend/dist/index.html'));

  // Optional: open DevTools
  win.webContents.openDevTools();
}

app.whenReady().then(createWindow);
