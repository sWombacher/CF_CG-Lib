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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # shared libs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # static libs


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug version")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Release version")
else()
    message(SEND_ERROR "please set CMAKE_BUILD_TYPE to \"Debug\" or \"Release\" (case sensitive)")
endif()

_add_lib(glut GLUT_LIB)
_add_cxxFlag(" -std=c++0x ")

# find all .h and .hpp files
FOREACH(dir ${directories_to_include})
    INCLUDE_DIRECTORIES(${dir})
    file(GLOB HEADERS_FILES ${HEADERS_FILES} ${dir}/*.h ${dir}/*.hpp)
ENDFOREACH()

# find all .cpp files
FOREACH(dir ${sources_to_include})
    file(GLOB SRC_FILES ${SRC_FILES} ${dir}/*.cpp ${dir}/*.c)
ENDFOREACH()


add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})

# add all example programs
file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/examples/*.cpp)
foreach(example ${examples})
    # get file name without its path or extenstion
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example} ${SRC_FILES} ${HEADERS_FILES})
    target_link_libraries(${name} ${GLUT_LIB})
    target_link_libraries(${name} ${OpenCV_LIBS})
endforeach()

