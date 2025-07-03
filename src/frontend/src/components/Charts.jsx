import React from 'react';
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS, LineElement, PointElement, CategoryScale, LinearScale } from 'chart.js';

ChartJS.register(LineElement, PointElement, CategoryScale, LinearScale);

const CpuChart = ({ dataPoints }) => {
  const data = {
    labels: dataPoints.map((_, i) => i + 1),
    datasets: [
      {
        label: 'CPU Usage (%)',
        data: dataPoints,
        borderColor: 'rgba(75,192,192,1)',
        tension: 0.3,
        fill: false
      }
    ]
  };

  const options = {
    responsive: true,
    scales: {
      y: { min: 0, max: 100 }
    }
  };

  return <Line data={data} options={options} />;
};

ChartJS.register(LineElement, PointElement, CategoryScale, LinearScale);

const DiskChart = ({ dataPoints }) => {
  const data = {
    labels: dataPoints.map((_, i) => i + 1),
    datasets: [
      {
        label: 'Disk Usage (%)',
        data: dataPoints,
        borderColor: 'rgb(98, 75, 192)',
        tension: 0.3,
        fill: false
      }
    ]
  };

  const options = {
    responsive: true,
    scales: {
      y: { min: 0, max: 100 }
    }
  };

  return <Line data={data} options={options} />;
};

ChartJS.register(LineElement, PointElement, CategoryScale, LinearScale);

const MemoryChart = ({ dataPoints }) => {
  const data = {
    labels: dataPoints.map((_, i) => i + 1),
    datasets: [
      {
        label: 'CPU Usage (%)',
        data: dataPoints,
        borderColor: 'rgb(191, 22, 22)',
        tension: 0.3,
        fill: false
      }
    ]
  };

  const options = {
    responsive: true,
    scales: {
      y: { min: 0, max: 100 }
    }
  };

  return <Line data={data} options={options} />;
};

export { MemoryChart, DiskChart, CpuChart };
