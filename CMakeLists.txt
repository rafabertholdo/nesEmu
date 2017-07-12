cmake_minimum_required(VERSION 3.0)

project(nesEmu)
set (CMAKE_CXX_STANDARD 14)

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/Rom.cpp   
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU.cpp    
   ${CMAKE_CURRENT_SOURCE_DIR}/Instruction.cpp    
   ${CMAKE_CURRENT_SOURCE_DIR}/PPU.cpp
)

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/Utils.h   
   ${CMAKE_CURRENT_SOURCE_DIR}/Rom.h   
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU.h      
   ${CMAKE_CURRENT_SOURCE_DIR}/Instruction.h      
   ${CMAKE_CURRENT_SOURCE_DIR}/PPU.h   
   ${CMAKE_CURRENT_SOURCE_DIR}/RegBit.h   
)

add_executable(${PROJECT_NAME} ${SOURCE})