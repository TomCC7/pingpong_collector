#!/usr/bin/env python3

import cv2

cap = cv2.VideoCapture(0)
ret, frame = cap.read()

if not ret:
    raise Exception("No camera found!")

cv2.imshow("origin",frame)
cv2.imwrite("origin.jpg", frame)
# second image
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
ret, frame = cap.read()
cv2.imshow("720p",frame)
cv2.imwrite("720p.jpg", frame)
cv2.waitKey(0)
cv2.destroyAllWindows()
