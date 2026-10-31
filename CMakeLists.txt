    cmake_minimum_required(VERSION 3.20)
    project(flight_blackbox CXX)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_compile_options(-Wall -Wextra -pedantic)
    include_directories(include)`
    add_executable(flight_blackbox
            src/main.cpp
            src/TelemetryBlackBox.cpp
            include/TelemetryBlackBox.h
    )