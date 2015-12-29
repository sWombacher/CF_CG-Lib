project(chaos)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_BUILD_TYPE "Debug")

include_directories(include)
find_package(OpenCV REQUIRED)
find_library(GLUT_LIB NAMES glut)

# enable c++11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}) # shared libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}) # static libs

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release")
    message("${CMAKE_BUILD_TYPE} version")
else()
    message(SEND_ERROR "please set CMAKE_BUILD_TYPE to \"Debug\" or \"Release\" (case sensitive)")
endif()

# find all .h and .hpp files
file(GLOB HEADERS_FILES include/*.h include/*.hpp)

# find all .cpp files
file(GLOB SRC_FILES src/*.cpp src/*.c)

# create chaos lib
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADERS_FILES})
target_link_libraries(${PROJECT_NAME} ${GLUT_LIB} ${OpenCV_LIBS})

# add all example executeables
file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/examples/*.cpp)
foreach(example ${examples})
    # get file name without its path or extenstion
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example} ${HEADERS_FILES})
    target_link_libraries(${name} ${PROJECT_NAME})
endforeach()
