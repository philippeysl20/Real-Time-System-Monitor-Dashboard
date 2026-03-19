# Real-Time System Monitor Dashboard

A desktop system monitoring dashboard built with Electron, React, and a C backend that streams live machine metrics over WebSockets.

## Features

- Live CPU, memory, and disk monitoring
- Task Manager-inspired dashboard UI with charts and drive cards
- Native C backend for collecting system metrics
- Electron desktop shell with a React frontend

## Tech Stack

- Electron
- React + Vite
- Chart.js
- C + CMake
- WebSockets

## Run Locally

1. Install dependencies:

```bash
npm install
```

2. Build the backend:

```bash
cmake -S . -B build
cmake --build build
```

3. Start the app in development mode:

```bash
npm run dev
```

For a production-style launch, build the frontend and start Electron with:

```bash
npm run start
```

## Notes

- The Electron app expects the backend executable at `build/src/backend/monitor.exe` or `cmake-build-debug/src/backend/monitor.exe`.
- The current backend setup is Windows-focused and listens on `ws://localhost:3000`.
