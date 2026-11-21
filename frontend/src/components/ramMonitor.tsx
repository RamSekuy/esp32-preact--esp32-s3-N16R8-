import type { StatusPayload } from "../models/WebSocketPayload";
type Props = {
  ram?: StatusPayload["ram"];
};

const defaultRam: StatusPayload["ram"] = {
  internal: {
    total: 0,
    free: 0,
  },
  psram: {
    total: 0,
    free: 0,
  },
};

export default function RamMonitor(props: Props) {
  const ram = props.ram || defaultRam;
  const getMemoryUsage = (memory: { total: number; free: number }) => {
    const used = Math.max(0, memory.total - memory.free);
    const percent = memory.total > 0 ? (used / memory.total) * 100 : 0;

    return {
      used,
      total: memory.total,
      free: memory.free,
      percent,
    };
  };

  const internalRam = getMemoryUsage(ram.internal);
  const psram = getMemoryUsage(ram.psram);

  const formatKB = (bytes: number) => (bytes / 1024).toFixed(1);

  return (
    <div class="rounded-2xl border border-slate-800 bg-slate-900 p-6">
      <div class="flex items-center justify-between">
        <div>
          <p class="text-sm text-slate-400">Memory Usage</p>
          <h4 class="mt-2 text-2xl font-bold">RAM Overview</h4>
        </div>
        <span class="rounded-xl bg-cyan-500/10 p-3 text-2xl">◈</span>
      </div>

      <div class="mt-6 space-y-6">
        {/* Internal RAM */}
        <div>
          <div class="flex items-center justify-between">
            <div>
              <p class="font-medium">Internal RAM</p>
              <p class="text-xs text-slate-500">System memory</p>
            </div>
            <p class="text-lg font-semibold text-cyan-400">
              {internalRam.percent.toFixed(1)}%
            </p>
          </div>

          <div class="mt-3 h-2 overflow-hidden rounded-full bg-slate-800">
            <div
              class="h-full rounded-full bg-cyan-400 transition-all"
              style={{ width: `${internalRam.percent}%` }}
            />
          </div>

          <div class="mt-2 flex justify-between text-xs text-slate-400">
            <span>{formatKB(internalRam.used)} KB Used</span>
            <span>{formatKB(internalRam.total)} KB Total</span>
          </div>
        </div>

        {/* PSRAM */}
        <div>
          <div class="flex items-center justify-between">
            <div>
              <p class="font-medium">PSRAM</p>
              <p class="text-xs text-slate-500">External memory</p>
            </div>
            <p class="text-lg font-semibold text-violet-400">
              {psram.total > 0 ? `${psram.percent.toFixed(1)}%` : "N/A"}
            </p>
          </div>

          <div class="mt-3 h-2 overflow-hidden rounded-full bg-slate-800">
            <div
              class="h-full rounded-full bg-violet-400 transition-all"
              style={{ width: `${psram.percent}%` }}
            />
          </div>

          <div class="mt-2 flex justify-between text-xs text-slate-400">
            <span>{formatKB(psram.used)} KB Used</span>
            <span>{formatKB(psram.total)} KB Total</span>
          </div>
        </div>
      </div>
    </div>
  );
}
