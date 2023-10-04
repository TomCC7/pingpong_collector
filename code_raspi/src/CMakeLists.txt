add_executable(
  ${PROJECT_NAME} algorithms.cpp
  main.cpp
  classes.cpp
  camera.cpp
  detector.cpp
  serial.cpp
  controller.cpp
  nucleo.cpp
)
include_directories(${PROJECT_SOURCE_DIR}/inc)
find_package(OpenCV REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(
  ${PROJECT_NAME}
  ${OpenCV_LIBS}
  Eigen3::Eigen
  Threads::Threads
)
