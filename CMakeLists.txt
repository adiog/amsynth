cmake_minimum_required(VERSION 3.0)

project(amsynth)

set(CMAKE_CXX_STANDARD 14)

#execute_process(
#                 COMMAND
#                     git rev-parse --short HEAD
#                 RESULT_VARIABLE
#                     SHORT_HASH_RESULT
#                 OUTPUT_VARIABLE
#                     SHORT_HASH
#                OUTPUT_STRIP_TRAILING_WHITESPACE)
#add_definitions(-DVERSION=${SHORT_HASH})

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

add_definitions(-DHAVE_CONFIG_H)

if(LINUX)
    add_definitions(-DARCH_LIN)
endif()
if (APPLE)
    add_definitions(-DARCH_MAC)

    execute_process(
            COMMAND
                brew --prefix
            RESULT_VARIABLE
                BREW_PREFIX_RESULT
            OUTPUT_VARIABLE
                BREW_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    include_directories(${BREW_PREFIX}/include)
    link_directories(${BREW_PREFIX}/lib)
endif()
if(WIN32)
    add_definitions(-DARCH_WIN)
endif()

find_package(GTK2 REQUIRED gtk)
        if(GTK2_FOUND)
            include_directories(${GTK2_INCLUDE_DIRS})
        endif()

add_definitions(-DPKGDATADIR="./data")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources src/**.cpp src/**.c)
file(GLOB_RECURSE headers src/**.h)

add_executable(amsynth ${sources} ${headers} config.h)

target_link_libraries(amsynth ${GTK2_LIBRARIES} pthread lo asound jack)

ADD_CUSTOM_TARGET(res ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)
