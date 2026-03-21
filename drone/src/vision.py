import cv2
import numpy as np

from config import (
    HSV_GREEN_LOWER,
    HSV_GREEN_UPPER,
    MIN_GREEN_AREA,
    REQUIRED_CONSECUTIVE_FRAMES,
)


class GreenLedDetector:
    """
    Detects a solid green LED in the video stream.
    Uses HSV thresholding and requires detection across several frames
    to reduce false positives.
    """

    def __init__(self) -> None:
        self.consecutive_detections = 0

    def process_frame(self, frame):
        """
        Returns:
            detected_now: whether green is visible in this frame
            confirmed: whether detection has been stable long enough
            annotated_frame: frame with debug overlay
            mask: binary mask of detected green areas
        """
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower = np.array(HSV_GREEN_LOWER, dtype=np.uint8)
        upper = np.array(HSV_GREEN_UPPER, dtype=np.uint8)

        mask = cv2.inRange(hsv, lower, upper)
        mask = cv2.GaussianBlur(mask, (5, 5), 0)

        contours, _ = cv2.findContours(
            mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )

        detected_now = False
        largest_area = 0
        center = None

        for contour in contours:
            area = cv2.contourArea(contour)
            if area >= MIN_GREEN_AREA and area > largest_area:
                largest_area = area
                detected_now = True

                moments = cv2.moments(contour)
                if moments["m00"] != 0:
                    cx = int(moments["m10"] / moments["m00"])
                    cy = int(moments["m01"] / moments["m00"])
                    center = (cx, cy)

        if detected_now:
            self.consecutive_detections += 1
        else:
            self.consecutive_detections = 0

        confirmed = self.consecutive_detections >= REQUIRED_CONSECUTIVE_FRAMES

        annotated = frame.copy()
        status = "DETECTED" if detected_now else "NOT DETECTED"
        cv2.putText(
            annotated,
            f"Green LED: {status}",
            (10, 30),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.8,
            (0, 255, 0) if detected_now else (0, 0, 255),
            2,
        )
        cv2.putText(
            annotated,
            f"Stable frames: {self.consecutive_detections}",
            (10, 65),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (255, 255, 255),
            2,
        )

        if center is not None:
            cv2.circle(annotated, center, 10, (0, 255, 0), 2)

        return detected_now, confirmed, annotated, mask