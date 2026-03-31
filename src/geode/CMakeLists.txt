cmake_minimum_required(VERSION 3.13.4)

# Set the project name
project(MulTwoPlayer VERSION 1.1.0)

# 1. Check if the SDK environment variable exists
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK")
endif()

# 2. Convert the path to a format CMake likes and include the scripts
file(TO_CMAKE_PATH "$ENV{GEODE_SDK}" GEODE_SDK_PATH)
include("${GEODE_SDK_PATH}/cmake/Geode.cmake")

# 3. Setup the mod
add_geode_mod(
    teknikclel69.servomod
    VERSION v1.1.0
    SOURCES 
        src/main_better.cpp 
        src/serial_library/src/SerialPort.cpp
)

# 4. Include the SerialPort headers
target_include_directories(${PROJECT_NAME} PUBLIC src/serial_library/include)
