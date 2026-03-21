import time

from config import (
    SAFE_BATTERY_MIN,
    ASCEND_CM,
    FORWARD_TO_MAZE_CM,
    LANDING_APPROACH_CM,
    FLIP_DIRECTION,
    COMMAND_DELAY,
)
from tello import TelloClient


class MazeMission:
    """
    Encapsulates the mission steps after LED detection.
    """

    def __init__(self, tello: TelloClient) -> None:
        self.tello = tello

    def preflight_check(self) -> bool:
        battery_response = self.tello.battery()
        if battery_response is None:
            print("Could not read battery level.")
            return False

        try:
            battery_level = int(battery_response)
        except ValueError:
            print(f"Unexpected battery response: {battery_response}")
            return False

        print(f"Battery: {battery_level}%")
        return battery_level >= SAFE_BATTERY_MIN

    def execute(self) -> None:
        """
        Assumes:
        - the LED indicates mission start
        - the maze entrance lies ahead in a known direction
        - distances were measured beforehand
        """
        print("Mission started.")

        print("Takeoff:", self.tello.takeoff())
        time.sleep(COMMAND_DELAY)

        print(f"Ascend {ASCEND_CM} cm:", self.tello.up(ASCEND_CM))
        time.sleep(COMMAND_DELAY)

        print(f"Flip {FLIP_DIRECTION}:", self.tello.flip(FLIP_DIRECTION))
        time.sleep(COMMAND_DELAY)

        print(
            f"Forward {FORWARD_TO_MAZE_CM} cm:",
            self.tello.forward(FORWARD_TO_MAZE_CM),
        )
        time.sleep(COMMAND_DELAY)

        print(
            f"Final approach {LANDING_APPROACH_CM} cm:",
            self.tello.forward(LANDING_APPROACH_CM),
        )
        time.sleep(COMMAND_DELAY)

        print("Landing:", self.tello.land())
        time.sleep(COMMAND_DELAY)

        print("Mission complete.")
