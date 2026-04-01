cmake_minimum_required(VERSION 3.13.4)

project(MulTwoPlayer VERSION 1.1.0)

# This is the magic part. Instead of guessing the path, 
# we let the Geode SDK find itself.
find_package(Geode REQUIRED)

add_geode_mod(
    teknikclel69.servomod
    VERSION v1.1.0
    SOURCES 
        src/main_better.cpp 
        src/serial_library/src/SerialPort.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src/serial_library/include)
