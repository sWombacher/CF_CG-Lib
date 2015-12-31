# in case you want to change your compiler from gcc/g++ to clang/clang++
#set(CMAKE_C_COMPILER   "/usr/bin/clang"  )
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(chaos)
cmake_minimum_required(VERSION 3.1)

#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

set(USE_SYSTEM_LIBS FALSE)
#set(USE_SYSTEM_LIBS TRUE )

include_directories(include)
add_definitions(-D${CMAKE_SYSTEM_NAME})

if(UNIX AND NOT CYGWIN)
    # search for system libs,
    # windows provides it without linking? TODO check
    find_library(GL_LIB NAMES GL)
    find_library(GLU_LIB NAMES GLU)
endif()

if (USE_SYSTEM_LIBS)
    message("Using system libs")
    find_package(OpenCV REQUIRED)
    find_library(GLUT_LIB NAMES glut)
    set(EXTERNAL_LIBS ${GLUT_LIB} ${OpenCV_LIBS})
else()
    message("Using provided 3rd party libs")
    include_directories(3rdparty/include)
    set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty/lib/${CMAKE_SYSTEM_NAME})
    if(WIN32 OR CYGWIN)
        set(LIB_DIR ${LIB_DIR}/${CMAKE_BUILD_TYPE}) # windows requires linking against similar build type (debug/release)
    endif()
    file(GLOB EXTERNAL_LIBS ${LIB_DIR}/*)

    # set rpath (unix only),
    # this prefers .so files from a specified directory over system libs
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH "${LIB_DIR}")
endif()

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
target_link_libraries(${PROJECT_NAME} ${GL_LIB} ${GLU_LIB} ${EXTERNAL_LIBS})

# add all example executeables
file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/examples/*.cpp)
foreach(example ${examples})
    # get file name without its path or extenstion
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example} ${HEADERS_FILES})
    target_link_libraries(${name} ${PROJECT_NAME})
endforeach()
