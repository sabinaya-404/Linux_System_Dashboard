# Linux System Dashboard & Telemetry Monitor:-)

A lightweight, zero-dependency Linux system monitor and web dashboard written in pure C from scratch.
Still building it as my personal project.

---

## Features
- **Memory & Swap Monitor:** Reads `/proc/meminfo` (accurate available memory calculations).
- **System Uptime:** Tracks system boot time via `/proc/uptime`.
- **CPU Telemetry:** Calculates real-time total and per-core CPU usage via clock tick deltas in `/proc/stat`.
- **Disk Usage:** Queries filesystem blocks and inodes with `statvfs()`.
- **Network I/O:** Tracks live upload/download transfer rates via `/proc/net/dev`.
- **Thermal Sensors:** Reads hardware temperatures from `/sys/class/thermal`.
- **Process Manager:** Scans `/proc/[PID]` for top CPU & RAM consuming tasks.
- **Terminal UI (TUI):** Smooth, flicker-free terminal dashboard with ANSI escape sequences.
- **Embedded Web Dashboard:** Built-in POSIX TCP socket HTTP server serving real-time JSON and an interactive web UI.

---

## Build & Run

### Prerequisites
- Linux / WSL (Arch, Ubuntu, Debian, Fedora, etc.)
- GCC and Make (`base-devel` on Arch, `build-essential` on Ubuntu)

### Compilation
```bash
make
```

### Running the Monitor
```bash
./sysmon
```

### Cleaning Build Artifacts
```bash
make clean
```

---

## Documentation
- Master Reference & Kernel Guide: [`docs/documentation.txt`](docs/documentation.txt)
- Dev Oopsies & Fixes Log: [`docs/mistakes.md`](docs/mistakes.md)
