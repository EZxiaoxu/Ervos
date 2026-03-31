cmake_minimum_required(VERSION 3.13.4)
find_package(Geode REQUIRED v3.0.0-beta.1)
project(servoMod)

add_geode_mod(
    teknikclel69.servomod
    VERSION v1.1.0
    SOURCES 
        src/main_better.cpp 
        src/serial_library/src/SerialPort.cpp
)

# This tells the compiler where to find SerialPort.hpp
target_include_directories(${PROJECT_NAME} PUBLIC src/serial_library/include)
