cmake_minimum_required(VERSION 3.0)
project(dummy)
set(CMAKE_CXX_STANDARD 17)
include_directories(
  ${PROJECT_SOURCE_DIR}/Inc
  # /home/cc/.platformio/packages/framework-stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Inc
)
file(
  GLOB SRCS
  "Src/*.cpp"
  "Src/*.c"
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(${PROJECT_NAME} ${SRCS})
