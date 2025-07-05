import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import { CpuChart, MemoryChart, DiskChart } from './components/Charts'
import './App.css'

function App() {
  const [count, setCount] = useState(0)
  let CPUarray = [];
  let cpugraphic = "";
  useEffect(() => {
    const socket = new WebSocket('ws://localhost:3000');

    socket.addEventListener("open", () => {
      console.log("Websocket connection opened")
    })

    socket.addEventListener("message", (event) => {
      const data = JSON.parse(event.data);
      if (data.type === "CPU-data") {
        CPUarray.push(data.cpudata);
        cpugraphic = CpuChart(CPUarray)

        // når der ikke er mere plads i chart:
        if (CPUarray.length > 50) {
          CPUarray.splice(0);
        }
        // do something with the cpu data. use the CpuChart function
      } else if (data.type === "Disk-data") {
        // do something with the disk data. use the DiskChart function
      } else if (data.type === "Memory-data") {
        // do something with the memory data. use the MemoryChart function
      }
    })
  })


  return (
    <>
      <div>
        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={() => setCount((count) => count + 1)}>
          count is {count}
        </button>
        <p>
          Edit <code>src/App.jsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
    </>
  )
}

export default App
