import socket
import threading
import time
from typing import Optional

from config import (
    TELLO_IP,
    TELLO_CMD_PORT,
    LOCAL_CMD_PORT,
    LOCAL_STATE_PORT,
)


class TelloClient:
    """
    Handles UDP communication with the Tello drone.
    Sends commands and listens for responses/state in background threads.
    """

    def __init__(self) -> None:
        self.tello_addr = (TELLO_IP, TELLO_CMD_PORT)
        self.last_response: Optional[str] = None
        self.latest_state: dict[str, str] = {}
        self.running = True

        self.cmd_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.cmd_socket.bind(("", LOCAL_CMD_PORT))
        self.cmd_socket.settimeout(5)

        self.state_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.state_socket.bind(("", LOCAL_STATE_PORT))
        self.state_socket.settimeout(1)

        self.response_thread = threading.Thread(
            target=self._receive_responses, daemon=True
        )
        self.state_thread = threading.Thread(target=self._receive_state, daemon=True)

        self.response_thread.start()
        self.state_thread.start()

    def _receive_responses(self) -> None:
        while self.running:
            try:
                data, _ = self.cmd_socket.recvfrom(1024)
                self.last_response = data.decode("utf-8").strip()
            except socket.timeout:
                continue
            except OSError:
                break

    def _receive_state(self) -> None:
        while self.running:
            try:
                data, _ = self.state_socket.recvfrom(2048)
                state_raw = data.decode("utf-8").strip()
                self.latest_state = self._parse_state(state_raw)
            except socket.timeout:
                continue
            except OSError:
                break

    @staticmethod
    def _parse_state(raw: str) -> dict[str, str]:
        state = {}
        for part in raw.split(";"):
            if ":" in part:
                key, value = part.split(":", 1)
                state[key] = value
        return state

    def send_command(self, command: str, timeout: float = 7.0) -> Optional[str]:
        """
        Send a command to the drone and wait for its response.
        Returns the response text or None if timed out.
        """
        self.last_response = None
        self.cmd_socket.sendto(command.encode("utf-8"), self.tello_addr)

        start = time.time()
        while time.time() - start < timeout:
            if self.last_response is not None:
                return self.last_response
            time.sleep(0.05)

        return None

    """
    High-level command methods for common drone actions.
    Each method sends the appropriate command string and waits for a response.
    """

    def enter_sdk_mode(self) -> Optional[str]:
        return self.send_command("command")

    def stream_on(self) -> Optional[str]:
        return self.send_command("streamon")

    def stream_off(self) -> Optional[str]:
        return self.send_command("streamoff")

    def takeoff(self) -> Optional[str]:
        return self.send_command("takeoff", timeout=10)

    def land(self) -> Optional[str]:
        return self.send_command("land", timeout=10)

    def up(self, cm: int) -> Optional[str]:
        return self.send_command(f"up {cm}")

    def forward(self, cm: int) -> Optional[str]:
        return self.send_command(f"forward {cm}")

    def back(self, cm: int) -> Optional[str]:
        return self.send_command(f"back {cm}")

    def cw(self, degrees: int) -> Optional[str]:
        return self.send_command(f"cw {degrees}")

    def ccw(self, degrees: int) -> Optional[str]:
        return self.send_command(f"ccw {degrees}")

    def flip(self, direction: str) -> Optional[str]:
        return self.send_command(f"flip {direction}", timeout=10)

    def battery(self) -> Optional[str]:
        return self.send_command("battery?")

    def keepalive(self) -> Optional[str]:
        """
        Tello lands automatically after 15 seconds without commands.
        A harmless read command can keep communication active.
        """
        return self.battery()

    def close(self) -> None:
        self.running = False
        try:
            self.cmd_socket.close()
        except OSError:
            pass
        try:
            self.state_socket.close()
        except OSError:
            pass
