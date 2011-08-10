PROJECT(Amaterasu3D)

#------- Configuration ------------
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Config/CMake ${CMAKE_MODULE_PATH})

# * -- To install project ---
SET(PROJECT_BUILD_PATH ${CMAKE_SOURCE_DIR}/Build)

# * --Subprojects--
MESSAGE("Search dependancies ... ")
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
FIND_PACKAGE(Freetype REQUIRED)
# ----- FreeImage
FIND_PACKAGE(FreeImage REQUIRED)
# ----- Zlib
FIND_PACKAGE(ZLIB REQUIRED)
# ----- Python
FIND_PACKAGE(PythonLibs REQUIRED)
# ----- Boost
IF(WIN32)
set(BOOST_ROOT C:/MinGW)
ELSE()
set(BOOST_ROOT /usr)
ENDIF()
set(BOOST_LIBS filesystem system regex program_options) # LINUX PROBLEM : random
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
MESSAGE("Boost libraries link is ${Boost_LIBRARIES}")

SUBDIRS(Code)