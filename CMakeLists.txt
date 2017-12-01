cmake_minimum_required(VERSION 3.2)

project(Game_engine)

set(CMAKE_CXX_STANDARD 11)
set(INCLUDES ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_LIBS_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_LIBS_DIR})
set(SOURCES ${CMAKE_SOURCE_DIR}/src/game.cpp)
set(LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/engine.cpp)

include_directories(${INCLUDES})

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

link_directories(${PROJECT_LIBS_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(ENGINE_LIB_NAME engined)
    add_library(${ENGINE_LIB_NAME} SHARED ${LIB_SOURCES})
    set(ENGINE_LINK_LIB -lSDL2d)
else(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(ENGINE_LIB_NAME engine)
    add_library(${ENGINE_LIB_NAME} SHARED ${LIB_SOURCES})
    set(ENGINE_LINK_LIB -lSDL2)
endif()
    target_link_libraries(${ENGINE_LIB_NAME} ${ENGINE_LINK_LIB})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ENGINE_LIB_NAME})