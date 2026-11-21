import { useCallback, useEffect, useRef, useState } from "preact/hooks";
import type { WebSocketEvent } from "../models/WebSocketEvent.model";

export default function useWebsocket() {
  const ws = useRef<WebSocket | null>(null);
  const [status, setStatus] = useState("connecting");

  // Register message listener
  const onMessage = useCallback((cb: (data: WebSocketEvent) => void) => {
    const socket = ws.current;
    if (!socket) return;

    socket.onmessage = (event) => {
      try {
        const data = JSON.parse(event.data) as WebSocketEvent;
        cb(data);
      } catch (error) {
        console.error("Invalid WebSocket message:", error);
      }
    };
  }, []);

  useEffect(() => {
    const socket = new WebSocket(`ws://${window.location.host}/ws`);

    ws.current = socket;

    socket.onopen = () => {
      setStatus("connected");
      console.log("WebSocket connected");
    };

    socket.onclose = () => {
      setStatus("disconnected");
      console.log("WebSocket disconnected");
    };

    socket.onerror = (error) => {
      setStatus("error");
      console.error("WebSocket error:", error);
    };

    return () => {
      socket.close();
      ws.current = null;
    };
  }, []);

  const send = useCallback((data: any) => {
    if (ws.current?.readyState !== WebSocket.OPEN) {
      console.warn("WebSocket is not connected");
      return;
    }

    ws.current.send(typeof data === "string" ? data : JSON.stringify(data));
  }, []);

  return {
    status,
    send,
    onMessage,
  };
}
