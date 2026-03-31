cmake_minimum_required(VERSION 3.13.4)

# Set the project name
project(MulTwoPlayer VERSION 1.1.0)

# 1. Find the Geode SDK using the environment variable
if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK")
endif()

# 2. Add the Geode helper scripts
include("$ENV{GEODE_SDK}/cmake/Geode.cmake")

# 3. Setup the mod
add_geode_mod(
    teknikclel69.servomod
    VERSION v1.1.0
    SOURCES 
        src/main_better.cpp 
        src/serial_library/src/SerialPort.cpp
)

# 4. Tell it where the SerialPort.hpp is
target_include_directories(${PROJECT_NAME} PUBLIC src/serial_library/include)
