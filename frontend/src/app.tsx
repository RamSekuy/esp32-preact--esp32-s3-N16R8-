export function App() {
  // Dummy device data
  const device = {
    name: "ESP32-S3",
    status: "Online",
    ip: "192.168.4.1",
    ram: {
      used: 128,
      total: 512,
    },
    sdcard: {
      detected: true,
      used: 2.4,
      total: 16,
    },
    hotspot: {
      connected: 3,
      devices: [
        { name: "iPhone", ip: "192.168.4.2" },
        { name: "Laptop", ip: "192.168.4.3" },
        { name: "Android", ip: "192.168.4.4" },
      ],
    },
  };

  const ramPercent = (device.ram.used / device.ram.total) * 100;
  const sdPercent = (device.sdcard.used / device.sdcard.total) * 100;

  return (
    <main class="min-h-screen bg-slate-950 text-slate-100">
      {/* Navbar */}
      <nav class="border-b border-slate-800 px-6 py-4">
        <div class="mx-auto flex max-w-6xl items-center justify-between">
          <div class="flex items-center gap-3">
            <div class="flex h-10 w-10 items-center justify-center rounded-xl bg-cyan-500 font-bold text-slate-950">
              E
            </div>
            <div>
              <h1 class="font-bold">ESP32 Control</h1>
              <p class="text-xs text-slate-500">Device Dashboard</p>
            </div>
          </div>

          <div class="flex items-center gap-2">
            <span class="h-2 w-2 rounded-full bg-emerald-400" />
            <span class="text-sm text-slate-400">Online</span>
          </div>
        </div>
      </nav>

      <div class="mx-auto max-w-6xl space-y-8 px-6 py-10">
        {/* Header */}
        <section>
          <p class="mb-2 text-sm font-medium text-cyan-400">DEVICE OVERVIEW</p>
          <h2 class="text-3xl font-bold tracking-tight sm:text-4xl">
            System Dashboard
          </h2>
          <p class="mt-2 text-slate-400">
            Monitor your ESP32 resources and connected devices.
          </p>
        </section>

        {/* Device information */}
        <section class="grid gap-4 sm:grid-cols-2">
          <div class="rounded-2xl border border-slate-800 bg-slate-900 p-5">
            <p class="text-sm text-slate-400">Device</p>
            <h3 class="mt-3 text-2xl font-bold">{device.name}</h3>
            <p class="mt-2 text-xs text-emerald-400">● Online</p>
          </div>

          <div class="rounded-2xl border border-slate-800 bg-slate-900 p-5">
            <p class="text-sm text-slate-400">IP Address</p>
            <h3 class="mt-3 text-2xl font-bold">{device.ip}</h3>
            <p class="mt-2 text-xs text-slate-500">Access Point</p>
          </div>
        </section>

        {/* Resource monitoring */}
        <section>
          <div class="mb-4">
            <h3 class="text-xl font-bold">System Resources</h3>
            <p class="mt-1 text-sm text-slate-400">Memory and storage usage.</p>
          </div>

          <div class="grid gap-4 md:grid-cols-2">
            {/* RAM */}
            <div class="rounded-2xl border border-slate-800 bg-slate-900 p-6">
              <div class="flex items-center justify-between">
                <div>
                  <p class="text-sm text-slate-400">RAM Usage</p>
                  <h4 class="mt-2 text-3xl font-bold">
                    {ramPercent.toFixed(1)}%
                  </h4>
                </div>
                <span class="rounded-xl bg-cyan-500/10 p-3 text-2xl">◈</span>
              </div>

              <div class="mt-6 h-2 overflow-hidden rounded-full bg-slate-800">
                <div
                  class="h-full rounded-full bg-cyan-400"
                  style={{ width: `${ramPercent}%` }}
                />
              </div>

              <div class="mt-3 flex justify-between text-xs text-slate-400">
                <span>{device.ram.used} KB Used</span>
                <span>{device.ram.total} KB Total</span>
              </div>
            </div>

            {/* SD Card */}
            <div class="rounded-2xl border border-slate-800 bg-slate-900 p-6">
              <div class="flex items-center justify-between">
                <div>
                  <p class="text-sm text-slate-400">SD Card Storage</p>
                  <h4 class="mt-2 text-3xl font-bold">
                    {device.sdcard.detected
                      ? `${(device.sdcard.total - device.sdcard.used).toFixed(1)} GB`
                      : "Not detected"}
                  </h4>
                </div>
                <span class="rounded-xl bg-violet-500/10 p-3 text-2xl">▤</span>
              </div>

              {device.sdcard.detected ? (
                <>
                  <div class="mt-6 h-2 overflow-hidden rounded-full bg-slate-800">
                    <div
                      class="h-full rounded-full bg-violet-400"
                      style={{ width: `${sdPercent}%` }}
                    />
                  </div>

                  <div class="mt-3 flex justify-between text-xs text-slate-400">
                    <span>{device.sdcard.used} GB Used</span>
                    <span>{device.sdcard.total} GB Total</span>
                  </div>
                </>
              ) : (
                <p class="mt-5 text-sm text-red-400">No SD card available.</p>
              )}
            </div>
          </div>
        </section>

        {/* Hotspot devices */}
        <section class="rounded-2xl border border-slate-800 bg-slate-900 p-6">
          <div class="flex flex-wrap items-center justify-between gap-3">
            <div>
              <h3 class="text-xl font-bold">Connected Devices</h3>
              <p class="mt-1 text-sm text-slate-400">
                Devices connected to ESP32 hotspot.
              </p>
            </div>

            <span class="rounded-full bg-emerald-500/10 px-3 py-1.5 text-sm font-medium text-emerald-400">
              {device.hotspot.connected} Connected
            </span>
          </div>

          <div class="mt-6 divide-y divide-slate-800">
            {device.hotspot.devices.map((item) => (
              <div
                key={item.ip}
                class="flex items-center justify-between gap-4 py-4"
              >
                <div class="flex items-center gap-3">
                  <div class="flex h-10 w-10 items-center justify-center rounded-xl bg-slate-800">
                    <span>▣</span>
                  </div>
                  <div>
                    <p class="font-medium">{item.name}</p>
                    <p class="text-xs text-slate-500">{item.ip}</p>
                  </div>
                </div>

                <span class="h-2 w-2 rounded-full bg-emerald-400" />
              </div>
            ))}
          </div>
        </section>

        {/* Applications */}
        <section>
          <div class="mb-4">
            <h3 class="text-xl font-bold">Applications</h3>
            <p class="mt-1 text-sm text-slate-400">
              Explore available applications.
            </p>
          </div>

          <div class="grid gap-4 sm:grid-cols-2">
            <a
              href="/mini-game-1"
              class="group rounded-2xl border border-slate-800 bg-slate-900 p-6 transition hover:border-cyan-500"
            >
              <div class="mb-4 text-3xl">🎮</div>
              <h4 class="font-semibold group-hover:text-cyan-400">
                Mini Game 1
              </h4>
              <p class="mt-2 text-sm text-slate-400">
                Open the first mini game.
              </p>
              <p class="mt-5 text-sm font-medium text-cyan-400">
                Launch application →
              </p>
            </a>

            <a
              href="/mini-game-2"
              class="group rounded-2xl border border-slate-800 bg-slate-900 p-6 transition hover:border-cyan-500"
            >
              <div class="mb-4 text-3xl">🕹️</div>
              <h4 class="font-semibold group-hover:text-cyan-400">
                Mini Game 2
              </h4>
              <p class="mt-2 text-sm text-slate-400">
                Open the second mini game.
              </p>
              <p class="mt-5 text-sm font-medium text-cyan-400">
                Launch application →
              </p>
            </a>
          </div>
        </section>

        <footer class="border-t border-slate-800 pt-6 text-center text-xs text-slate-500">
          ESP32 Control Panel · Powered by ESP-IDF + Preact
        </footer>
      </div>
    </main>
  );
}
