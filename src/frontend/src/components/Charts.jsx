import React from 'react';
import { Pie } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  ArcElement,
  Tooltip,
  Legend,
} from 'chart.js';

ChartJS.register(ArcElement, Tooltip, Legend);

function createPieData(label, value, usedColor) {
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

const options = {
  responsive: true,
  plugins: {
    legend: {
      position: 'bottom',
    },
  },
};

function CpuChart(value) {
  const data = createPieData('CPU', value, 'rgba(75,192,192,0.8)');
  return <Pie data={data} options={options} />;
}

function DiskChart(value) {
  const data = createPieData('Disk', value, 'rgb(98, 75, 192)');
  return <Pie data={data} options={options} />;
}

function MemoryChart(value) {
  const data = createPieData('Memory', value, 'rgb(191, 22, 22)');
  return <Pie data={data} options={options} />;
}

export { CpuChart, DiskChart, MemoryChart };
