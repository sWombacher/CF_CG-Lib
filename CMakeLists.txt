project(chaos)
cmake_minimum_required(VERSION 2.8)

include(functions.cmake)
include_directories(include)


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

_add_lib(GL GL_LIB)
_add_lib(glu GLU_LIB)
_add_lib(glut GLUT_LIB)
_add_lib(glfw GLFW_LIB)
_add_cxxFlag(" -std=c++1y ")


# add OS specific include folders (can be found in include/<OS>)
message(your\ system\ name\ is\ \"${CMAKE_SYSTEM_NAME}\"\ you\ may\ add\ OS\ specific\ code\ in\ <include/src>/${CMAKE_SYSTEM_NAME}\n\n)
set(    sources_to_include      ${sources_to_include}       src/${CMAKE_SYSTEM_NAME})
set(directories_to_include  ${directories_to_include}   include/${CMAKE_SYSTEM_NAME})


# find all .h and .hpp files
FOREACH(dir ${directories_to_include})
    INCLUDE_DIRECTORIES(${dir})
    file(GLOB HEADERS_FILES ${HEADERS_FILES} ${dir}/*.h ${dir}/*.hpp)
ENDFOREACH()

# find all .cpp files
FOREACH(dir ${sources_to_include})
    file(GLOB SRC_FILES ${SRC_FILES} ${dir}/*.cpp ${dir}/*.c)
ENDFOREACH()

# add alle single sorce files (like main.cpp)
FOREACH(file ${single_src_files})
    set(SRC_FILES ${SRC_FILES} ${file})
ENDFOREACH()


add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS_FILES})
target_link_libraries(${PROJECT_NAME} ${GL_LIB})
target_link_libraries(${PROJECT_NAME} ${GLU_LIB})
target_link_libraries(${PROJECT_NAME} ${GLUT_LIB})
target_link_libraries(${PROJECT_NAME} ${GLFW_LIB})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

