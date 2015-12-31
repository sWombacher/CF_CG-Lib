# in case you want to change your compiler
#set(CMAKE_C_COMPILER   "/usr/bin/clang"  )
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(chaos)
cmake_minimum_required(VERSION 3.1)

#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

#set(USE_SYSTEM_LIBS FALSE)
set(USE_SYSTEM_LIBS TRUE )

include_directories(include)
add_definitions(-D${CMAKE_SYSTEM_NAME})

if (USE_SYSTEM_LIBS)
    message("Using system libs")
    find_package(OpenCV REQUIRED)
    find_library(GL_LIB NAMES GL)
    find_library(GLU_LIB NAMES GLU)
    find_library(GLUT_LIB NAMES glut)
else()
    message("Using provided 3rd party libs")
    include_directories(3rdparty/include)
    find_library(GL_LIB NAMES GL)
    find_library(GLU_LIB NAMES GLU)
    find_library(GLUT_LIB NAMES glut PATHS 3rdparty/lib/${CMAKE_SYSTEM_NAME} NO_DEFAULT_PATH)

    file(GLOB tmp 3rdparty/lib/${CMAKE_SYSTEM_NAME}/*opencv*${CMAKE_SHARED_MODULE_SUFFIX})
    foreach(file ${tmp})
        get_filename_component(name ${file} NAME_WE)

        # remove prefix
        # NOTE: the  ^  at the beginning is itentional, do not change
        string(REGEX REPLACE "^${CMAKE_SHARED_MODULE_PREFIX}" "" name ${name})
        find_library(file NAMES ${name} PATHS 3rdparty/lib/${CMAKE_SYSTEM_NAME} NO_DEFAULT_PATH)
        set(OpenCV_LIBS ${OpenCV_LIBS} ${file})
    endforeach()

    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_LIST_DIR}/3rdparty/lib/${CMAKE_SYSTEM_NAME}")
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
target_link_libraries(${PROJECT_NAME} ${GL_LIB} ${GLU_LIB} ${GLUT_LIB} ${OpenCV_LIBS})


# add all example executeables
file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/examples/*.cpp)
foreach(example ${examples})
    # get file name without its path or extenstion
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example} ${HEADERS_FILES})
    target_link_libraries(${name} ${PROJECT_NAME})
endforeach()
