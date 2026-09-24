async function updateStats() {
    try {
        const res = await fetch('/api/stats');
        const data = await res.json();

        // CPU
        document.getElementById('cpu-val').innerText = `${data.cpu.percent.toFixed(1)}%`;
        document.getElementById('cpu-bar').style.width = `${Math.min(data.cpu.percent, 100)}%`;

        // RAM
        document.getElementById('ram-val').innerText = `${data.ram.percent.toFixed(1)}%`;
        document.getElementById('ram-sub').innerText = `${data.ram.used_gb.toFixed(1)} / ${data.ram.total_gb.toFixed(1)} GB`;
        document.getElementById('ram-bar').style.width = `${Math.min(data.ram.percent, 100)}%`;

        // Disk
        document.getElementById('disk-val').innerText = `${data.disk.percent.toFixed(1)}%`;
        document.getElementById('disk-sub').innerText = `${data.disk.used_gb.toFixed(1)} / ${data.disk.total_gb.toFixed(1)} GB`;
        document.getElementById('disk-bar').style.width = `${Math.min(data.disk.percent, 100)}%`;

        // Net & Temp
        document.getElementById('net-rx').innerText = `${data.network.rx_kBps.toFixed(1)} kB/s`;
        document.getElementById('net-tx').innerText = `${data.network.tx_kBps.toFixed(1)} kB/s`;
        document.getElementById('temp-val').innerText = data.thermal.found ? `${data.thermal.celsius.toFixed(1)} °C` : 'N/A';

        // Uptime
        const d = Math.floor(data.uptime / 86400);
        const h = Math.floor((data.uptime % 86400) / 3600);
        const m = Math.floor((data.uptime % 3600) / 60);
        const s = data.uptime % 60;
        document.getElementById('uptime').innerText = `Uptime: ${d}d ${h}h ${m}m ${s}s`;

        // Processes
        const tbody = document.getElementById('proc-table');
        tbody.innerHTML = data.processes.map(p => `
            <tr>
                <td>${p.pid}</td>
                <td><strong>${p.name}</strong></td>
                <td>${p.rss_mb.toFixed(1)} MB</td>
            </tr>
        `).join('');

    } catch (err) {
        console.error("Fetch error:", err);
    }
}

setInterval(updateStats, 1000);
updateStats();