cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(rr LANGUAGES CXX)

# Set C/C++ compiler version to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set visibility settings
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)



# Add project subdirectory for OpenXLSX
add_subdirectory(OpenXLSX)

# Add your executable
add_executable(rr main.cpp)

# Include directories and link libraries for your project
target_include_directories(rr PRIVATE OpenXLSX/OpenXLSX/include)
target_link_libraries(rr OpenXLSX::OpenXLSX)
