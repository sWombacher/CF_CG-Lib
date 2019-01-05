set(PYTHON_MODULE_NAME "cf")

if (WIN32 OR CYGWIN)
	set(SWIG_DIR ${CMAKE_SOURCE_DIR}/3rdparty/tools/swig)
	set(SWIG_EXECUTABLE ${CMAKE_SOURCE_DIR}/3rdparty/tools/swig/swig.exe)
endif()

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "-Wall")
set_property(SOURCE cf.i swig_glm.i PROPERTY CPLUSPLUS ON)

add_definitions(-DSWIG_SUPPORT)
swig_add_library(cf LANGUAGE python SOURCES cf.i)
swig_link_libraries(cf ${PYTHON_LIBRARIES} ${PROJECT_NAME})
