# How to calibrate

## Collect data

* print the `chessboard.pdf` file
* run `img_collect.py` to collect images, should have at least 20 images of
 the chessboard from different angles

## Calibration

* change parameters in `camera_calibration.py`
  * `num_intersections_in_x` shows how many intersection line in x
  * `num_intersections_in_y` shows how many intersection line in y
  * `square_size` gives the size of square in meters, used for determining the distance
* run the file and get calibration data
