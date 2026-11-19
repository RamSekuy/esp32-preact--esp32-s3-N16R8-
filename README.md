# ESP32-S3 Web Dashboard

Prototype dashboard WiFi berbasis ESP32-S3 (ESP-IDF via PlatformIO) dengan UI
Preact + Tailwind yang disajikan langsung dari LittleFS.

> **Status:** prototype. Data yang ditampilkan masih dummy dan belum berasal
> dari perangkat secara langsung.

## Features

- WiFi Access Point (`ESP32_Hotspot`) + HTTP server di port 80
- UI dashboard: status device, RAM, storage, dan daftar perangkat terhubung
- File statis dilayani dari LittleFS (`/littlefs`)
- NVS otomatis di-reset saat versi berubah atau kapasitas penuh
- Partisi LittleFS terpisah untuk filesystem (`storage`, 1,5 MB)

## Hardware

- ESP32-S3 DevKitC-1
- Flash 16 MB (mode DIO)
- PSRAM OCT
- USB untuk upload dan serial monitor

## Prasyarat

- [PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/index.html)
- Node.js 20+
- [pnpm](https://pnpm.io/installation)

## Struktur proyek

```text
.
├── src/                  # Firmware ESP-IDF (wifi, webserver, storage)
├── frontend/             # UI Preact + Tailwind (Vite)
│   └── src/              # Komponen dan halaman dashboard
├── data -> frontend/dist # Symlink: sumber filesystem untuk uploadfs
├── partitions.csv        # Layout partisi flash
└── platformio.ini        # Konfigurasi board, flash, LittleFS
```

## Build & Upload

```bash
# 0. Siapkan target symlink data/ (wajib di clone baru)
mkdir -p frontend/dist

# 1. Build UI → hasil masuk ke frontend/dist (lewat symlink data/)
cd frontend
pnpm install
pnpm build
cd ..

# 2. Build dan upload firmware
pio run -t upload

# 3. Upload filesystem LittleFS
pio run -t uploadfs

# 4. Serial monitor
pio device monitor
```

> **Penting:** `data/` adalah symlink ke `frontend/dist/`, sedangkan
> `frontend/dist/` tidak disimpan di Git. Di clone baru, symlink tersebut
> rusak sampai folder `frontend/dist/` dibuat. Vite menulis hasil build ke
> `../data`, jadi tanpa langkah `mkdir -p frontend/dist` build akan gagal dengan
> error `Could not create directory for output chunks: .../data`.
>
> Jika symlink `data` tidak ikut tersalin (misalnya di Windows), buat manual:
> `ln -s frontend/dist data` (Linux/macOS) atau
> `mklink /D data frontend\dist` (Command Prompt Windows).

## Akses

| Item | Nilai |
|---|---|
| WiFi SSID | `ESP32_Hotspot` |
| WiFi password | `12345678` (ganti sebelum dipakai di luar lab) |
| Dashboard | http://192.168.4.1 |
| Serial | 115200 baud |

## Endpoint

| Method | Path | Keterangan |
|---|---|---|
| GET | `/` | Dashboard (`/littlefs/index.html`) |
| GET | `/assets/*` | CSS, JavaScript, dan aset statis |

Server hanya menyajikan file statis dari LittleFS; belum ada endpoint API
untuk data perangkat.

## Konfigurasi

- SSID, password, channel, dan jumlah koneksi AP: `src/wifi/wifi_ap.cpp`
- Partisi flash: `partitions.csv`
- Flash mode, ukuran flash, filesystem, dan baud rate: `platformio.ini`
- Default SDK (flash 16 MB, DIO, PSRAM OCT): `sdkconfig.defaults`
- UI dan konfigurasi build Vite: `frontend/`
- Sumber UI: `frontend/src/`
- Hasil build UI: `frontend/dist/` (diakses lewat symlink `data/`)

## Troubleshooting

| Gejala | Penyebab / solusi |
|---|---|
| Build UI gagal: `Could not create directory for output chunks: .../data` | Jalankan `mkdir -p frontend/dist` sebelum `pnpm build` |
| Halaman kosong atau 404 | `uploadfs` belum dijalankan, atau symlink `data` rusak. Cek `ls data/index.html` |
| Log `LittleFS mount failed` | Cek partisi `storage` di `partitions.csv` dan ukuran flash di `platformio.ini` |
| Serial monitor kosong | Pastikan baud 115200 dan port USB yang benar |

## Limitasi saat ini

- Data dashboard hardcode di `frontend/src/app.tsx`, belum mengambil data aktual dari ESP32.
- Belum ada endpoint API / komunikasi firmware ↔ frontend.
- Endpoint `/mini-game-1` dan `/mini-game-2` belum memiliki handler.
- Password Access Point masih hardcode dan dicetak ke serial monitor (khusus development).
- Tidak ada autentikasi pada HTTP server; hanya cocok untuk jaringan AP lokal.

## Lisensi

Belum ditentukan. Tambahkan file `LICENSE` sebelum repositori dipublikasikan.
