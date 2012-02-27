PROJECT(Amaterasu3D)

#------- Configuration ------------
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Config/CMake ${CMAKE_MODULE_PATH})

# * -- To install project ---
SET(PROJECT_BUILD_PATH ${CMAKE_SOURCE_DIR}/Build)

################################
# * --Subprojects--
################################
MESSAGE("Search dependancies ... ")
##### Root dir
SET(GLEW_ROOT_DIR ${DEPENDANCIES_ROOT_DIR})
SET(FreeImage_ROOT_DIR ${DEPENDANCIES_ROOT_DIR})
SET(LibSigGCXX2_ROOT_DIR ${DEPENDANCIES_ROOT_DIR})
SET(FreeType_ROOT_DIR ${DEPENDANCIES_ROOT_DIR})

# ----- Glew
FIND_PACKAGE(GLEW REQUIRED)
# ----- OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
  MESSAGE(STATUS "Find OpenGL")
ELSE(OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "You must install OpenGL")
ENDIF(OPENGL_FOUND)
# ----- FreeType
FIND_PACKAGE(FreeType REQUIRED)
# ----- FreeImage
FIND_PACKAGE(FreeImage REQUIRED)
# ----- Assimp
FIND_PACKAGE(Assimp REQUIRED)
# ----- Zlib
FIND_PACKAGE(ZLIB REQUIRED)
# ----- Python
FIND_PACKAGE(PythonLibs REQUIRED)
# ----- Boost
IF(WIN32)
	IF(MSVC)
		set(Boost_USE_STATIC_LIBS        ON)
		set(BOOST_ROOT "C:/Program Files (x86)/boost/boost_1_47/")
	ENDIF()
	IF(MINGW)
		set(BOOST_ROOT "C:/MinGW/")
	ENDIF()
ELSE()
	set(BOOST_ROOT /usr)
ENDIF()

set(BOOST_LIBS filesystem system regex program_options signals) # random
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
MESSAGE(STATUS "Boost libraries link is ${Boost_LIBRARIES}")
MESSAGE(STATUS "Boost include dir is ${Boost_INCLUDE_DIRS}")

##### Setup includes
include_directories(
	${Boost_INCLUDE_DIRS}
	${FreeType_INCLUDE_DIRS}
	${PYTHON_INCLUDE_DIRS}
)

###################################
## Special configuration
###################################
IF(MSVC)
	ADD_DEFINITIONS("-D_USE_MATH_DEFINES") # For M_PI ... etc
	ADD_DEFINITIONS("-DMSVC") # For add windows.h before gl.h
	link_directories(${Boost_LIBRARY_DIRS})
ENDIF()

SUBDIRS(Code)