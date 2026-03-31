cmake_minimum_required(VERSION 3.13.4)

# Tell CMake to find the Geode SDK
find_package(Geode REQUIRED v3.0.0-beta.1)

project(MulTwoPlayer VERSION 1.1.0)

# This defines the mod and tells it which files to compile
add_geode_mod(
    teknikclel69.servomod
    VERSION v1.1.0
    SOURCES 
        src/main_better.cpp 
        src/serial_library/src/SerialPort.cpp
        src/serial_library/include/SerialPort.hpp
)

# This tells the compiler where the .hpp file is hiding so main_better.cpp can find it
target_include_directories(${PROJECT_NAME} PUBLIC src/serial_library/include)
