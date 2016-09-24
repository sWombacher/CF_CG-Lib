# in case you want to change your compiler on unix systems
#set(CMAKE_C_COMPILER   "clang"  )
#set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_C_COMPILER_ID "Clang")

project(cf_cg)
cmake_minimum_required(VERSION 3.1)

# this is case sensitive!
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_BUILD_TYPE "Debug")

if (WIN32 OR CYGWIN)
	set(USE_SYSTEM_LIBS FALSE)
else()
	set(USE_SYSTEM_LIBS TRUE )
endif()

# enable compiler warnings for gcc/clang
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} EQUAL "Clang")
    set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Wextra ")
endif()


include_directories(include)
include_directories(3rdparty/required_include)
add_definitions(-D${CMAKE_SYSTEM_NAME})

if (USE_SYSTEM_LIBS)
    message("Using system libs")
	
    find_package(OpenCV REQUIRED)
	find_package(GLUT REQUIRED)
	find_package(OpenGL REQUIRED)
	set(GL_LIB ${OPENGL_LIBRARIES})
	set(GLU_LIB)
	set(GLUT_LIB ${GLUT_LIBRARY})

else()
    message("Using provided 3rd party libs")

    include_directories(3rdparty/include)
    if (NOT CMAKE_SIZEOF_VOID_P MATCHES "8")
        message(SEND_ERROR "Error:\nProvided libraries are 64bit versions,\nif you have a 64bit system try changing your compiler to a x86_x64 compiler\nNote: you may have to delete the cmake cache to do so")
    endif()	
	
	set(LIB_DIR 3rdparty/lib/${CMAKE_SYSTEM_NAME})
	if (WIN32 OR CYGWIN)
		file(GLOB GLUT_LIB_DEBUG ${LIB_DIR}/Debug/freeglut*.lib)
		file(GLOB GLUT_LIB_RELEASE ${LIB_DIR}/Release/freeglut*.lib)
		
		file(GLOB OpenCV_LIBS_DEBUG ${LIB_DIR}/Debug/*opencv*.lib)
		file(GLOB OpenCV_LIBS_RELEASE ${LIB_DIR}/Release/*opencv*.lib)

	elseif (APPLE)
		find_package(GLUT REQUIRED)
		find_package(OpenGL REQUIRED)
		set(GL_LIB ${OPENGL_LIBRARIES})
		set(GLU_LIB)
		set(GLUT_LIB ${GLUT_LIBRARY})

		file(GLOB OpenCV_LIBS ${LIB_DIR}/*)
	else()
		find_library(GL_LIB NAMES GL)
		find_library(GLU_LIB NAMES GLU)
		find_library(GLUT_LIB NAMES glut PATHS ${LIB_DIR} NO_DEFAULT_PATH)

		file(GLOB tmp ${LIB_DIR}/*opencv*${CMAKE_SHARED_MODULE_SUFFIX})
		foreach(file ${tmp})
			get_filename_component(name ${file} NAME_WE)

			# remove prefix
			# NOTE: the  ^  at the beginning is itentional, do not change
			string(REGEX REPLACE "^${CMAKE_SHARED_MODULE_PREFIX}" "" name ${name})
			find_library(file NAMES ${name} PATHS ${LIB_DIR} NO_DEFAULT_PATH)
			set(OpenCV_LIBS ${OpenCV_LIBS} ${file})
		endforeach()
	endif()

    SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_LIST_DIR}/3rdparty/lib/${CMAKE_SYSTEM_NAME}")
endif()

# enable c++11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set output paths
if (WIN32 OR CYGWIN)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin) # executeable
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # shared libs
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib) # static libs
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin/${CMAKE_BUILD_TYPE}) # executeable
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}) # shared libs
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}) # static libs
endif()

# endable openmp support
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
    find_package(OpenMP)
    if (OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        add_definitions(-DOPENMP_ENABLED)
        message("OpenMP enabled")
    endif()
endif()

# find all header and source files
file(GLOB HEADERS_FILES include/*.h include/*.hpp)
file(GLOB SRC_FILES src/*.cpp src/*.c)

# create chaos lib
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HEADERS_FILES})
if (WIN32 OR CYGWIN)
	# copy dlls
	file(GLOB tmp ${LIB_DIR}/dlls/Debug/*)
	file(COPY ${tmp} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
	
	file(GLOB tmp ${LIB_DIR}/dlls/Release/*)
	file(COPY ${tmp} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	
	target_link_libraries(${PROJECT_NAME} debug ${GLUT_LIB_DEBUG} optimized ${GLUT_LIB_RELEASE})
	
	# link debug and release libraries
	list(LENGTH OpenCV_LIBS_DEBUG     Debug_SIZE)
	list(LENGTH OpenCV_LIBS_RELEASE Release_SIZE)
	if (NOT ${Debug_SIZE} EQUAL ${Release_SIZE})
		message(SEND_ERROR "Error: Unequal number of opencv libs between debug and release")
	endif()
	
	math(EXPR SIZE "${Debug_SIZE} - 1")
	foreach(val RANGE ${SIZE})
		list(GET OpenCV_LIBS_DEBUG   ${val} deb)
		list(GET OpenCV_LIBS_RELEASE ${val} rel)
        target_link_libraries(${PROJECT_NAME} debug ${deb} optimized ${rel})
	endforeach()
else()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release")
		message("${CMAKE_BUILD_TYPE} version")
	else()
		message(SEND_ERROR "please set CMAKE_BUILD_TYPE to \"Debug\" or \"Release\" (case sensitive)")
	endif()
	target_link_libraries(${PROJECT_NAME} ${GL_LIB} ${GLU_LIB} ${GLUT_LIB} ${OpenCV_LIBS})
endif()

# add define
add_definitions(   -DCHAOS_FILE_PATH="${CMAKE_CURRENT_LIST_DIR}/chaos_files/")
add_definitions(-DGEOMETRY_FILE_PATH="${CMAKE_CURRENT_LIST_DIR}/geometry_files/")

# add all example executeables
file(GLOB examples ${CMAKE_CURRENT_LIST_DIR}/examples/*.cpp)
foreach(example ${examples})
    # get file name without its path or extenstion
    get_filename_component(name ${example} NAME_WE)

    add_executable(${name} ${example})
    target_link_libraries(${name} ${PROJECT_NAME})
    target_include_directories(${name} PUBLIC include)
endforeach()

# add solutions to all practices
set(DIRECTORIES_TO_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LIBRARIES_TO_LINK ${PROJECT_NAME})

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/Solutions)
        add_subdirectory(Solutions)
else()
	message("Warning: Solutions directory not found")
endif()
