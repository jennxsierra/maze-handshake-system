import time

from config import (
    ASCEND_CM,
    FORWARD_TO_MAZE_CM,
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

        print("Landing:", self.tello.land())
        time.sleep(COMMAND_DELAY)

        print("Mission complete.")
