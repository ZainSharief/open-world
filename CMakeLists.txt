cmake_minimum_required(VERSION 3.10)
project(open-world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Define source, include, and library directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Find all .cpp files in src, including subdirectories
file(GLOB_RECURSE SRCS "${SRC_DIR}/*.cpp" "${SRC_DIR}/dependencies/glad.c")

# Include all subdirectories in src for header files
include_directories(${INCLUDE_DIR} ${SRC_DIR})

# Link directories
link_directories(${LIB_DIR})

# Define the executable
add_executable(${PROJECT_NAME} ${SRCS})

# Link libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME} glfw3dll)
else()
    find_package(glfw3 REQUIRED)
    target_link_libraries(${PROJECT_NAME} glfw)
endif()