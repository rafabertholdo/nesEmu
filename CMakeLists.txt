cmake_minimum_required(VERSION 3.0)

project(nesEmu)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED >=2.0.0)
find_package(SDL2_mixer REQUIRED >=2.0.0)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2MIXER_INCLUDE_DIR})
file(GLOB SOURCES *.cpp include/*.h instructions/*.cpp include/instruction/*.h)

# Include header files
include_directories ("${PROJECT_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})
