export type WebSocketEvent<T = any> = {
  type: string;
  action: string;
  payload: T;
};
