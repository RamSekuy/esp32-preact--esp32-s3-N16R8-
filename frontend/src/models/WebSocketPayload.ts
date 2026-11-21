export type StatusPayload = {
  ram: {
    internal: RamStatus;
    psram: RamStatus;
  };
};

type RamStatus = {
  total: number;
  free: number;
};
