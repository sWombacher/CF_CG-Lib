project(chaos)
cmake_minimum_required(VERSION 2.8)

include(functions.cmake)
include_directories(include)


# set Debug/Release
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")


find_package(OpenCV REQUIRED)


set(directories_to_include
    include
)
set(sources_to_include
    src
)
set(single_src_files
    main.cpp
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug version")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release version")
else()
    message(SEND_ERROR "please set CMAKE_BUILD_TYPE to \"Debug\" or \"Release\" (case sensitive)")
endif()

_add_lib(glut GLUT_LIB)
_add_cxxFlag(" -std=c++1y ")

# find all .h and .hpp files
FOREACH(dir ${directories_to_include})
    INCLUDE_DIRECTORIES(${dir})
    file(GLOB HEADERS_FILES ${HEADERS_FILES} ${dir}/*.h ${dir}/*.hpp)
ENDFOREACH()

# find all .cpp files
FOREACH(dir ${sources_to_include})
    file(GLOB SRC_FILES ${SRC_FILES} ${dir}/*.cpp ${dir}/*.c)
ENDFOREACH()

# add alle single source files (like main.cpp)
FOREACH(file ${single_src_files})
    set(SINGLE_FILES ${SINGLE_FILES} ${file})
ENDFOREACH()


add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES} ${SINGLE_FILES})
target_link_libraries(${PROJECT_NAME} ${GLUT_LIB})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
