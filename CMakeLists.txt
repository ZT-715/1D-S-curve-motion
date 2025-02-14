# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.12)

# Set the project name and language
project(1D-S-curve-motion LANGUAGES C)

# Specify C99 as the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF) # Disable GNU extensions for strict C99 compliance
add_compile_options(-Wall -Wpedantic)

# Add an executable target
add_executable(${PROJECT_NAME} main.c)
