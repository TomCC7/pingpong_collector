#!/usr/bin/env python3
"""
Collect image data for calibration
Usage: press 'n' to take next frame,
             'q' to stop
"""
import cv2
import os

store_dir = "data"


def collect():
    if not os.path.exists(store_dir):
        os.mkdir(store_dir)

    os.chdir(store_dir)
    cnt = 0
    while True:
        cv2.destroyAllWindows()
        cap = cv2.VideoCapture(0)
        ret, frame = cap.read()
        file_name = str(cnt) + ".jpg"
        cv2.imshow(file_name, frame)
        cap.release()
        while True:
            key = cv2.waitKey(0) & 0xFF
            if key == ord('q'):
                return
            elif key == ord('a'):
                break
            elif key == ord('n'):
                cv2.imwrite(file_name, frame)
                cnt+=1
                break


if __name__ == "__main__":
    collect()
