import time

import cv2

from config import VIDEO_URL, KEEPALIVE_INTERVAL
from mission import MazeMission
from tello import TelloClient
from vision import GreenLedDetector


def main() -> None:
    tello = TelloClient()
    detector = GreenLedDetector()
    mission_started = False
    last_keepalive = time.time()

    try:
        print("Entering SDK mode...")
        print("Response:", tello.enter_sdk_mode())

        print("Starting video stream...")
        print("Response:", tello.stream_on())
        time.sleep(2)

        cap = cv2.VideoCapture(VIDEO_URL, cv2.CAP_FFMPEG)
        if not cap.isOpened():
            raise RuntimeError("Could not open Tello video stream.")

        mission = MazeMission(tello)

        if not mission.preflight_check():
            raise RuntimeError("Preflight check failed.")

        print("Waiting for solid green LED detection...")

        while True:
            ret, frame = cap.read()
            if not ret:
                time.sleep(0.05)
                continue

            detected_now, confirmed, annotated, mask = detector.process_frame(frame)

            if confirmed and not mission_started:
                print("Green LED confirmed. Launching mission.")
                mission_started = True
                mission.execute()
                break

            if time.time() - last_keepalive > KEEPALIVE_INTERVAL:
                tello.keepalive()
                last_keepalive = time.time()

            cv2.imshow("Tello Camera", annotated)
            cv2.imshow("Green Mask", mask)

            key = cv2.waitKey(1) & 0xFF
            if key == ord("q"):
                break

        cap.release()
        cv2.destroyAllWindows()

    finally:
        try:
            tello.stream_off()
        except Exception:
            pass
        tello.close()


if __name__ == "__main__":
    main()
