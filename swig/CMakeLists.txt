set(PYTHON_MODULE_NAME "cf")

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

set(CMAKE_SWIG_FLAGS "-Wall")

set_property(SOURCE cf.i PROPERTY CPLUSPLUS ON)

swig_add_library(cf LANGUAGE python SOURCES cf.i)
swig_link_libraries(cf ${PYTHON_LIBRARIES} ${PROJECT_NAME}_shared)
