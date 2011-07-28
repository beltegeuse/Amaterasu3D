SUBDIRS(Amaterasu3D)
SUBDIRS(TinyXML)
SUBDIRS(Assimp)
# Submodules
#if(NOT EXISTS "C3/CMakeFiles.txt")
#  MESSAGE("Need Submodule ... ")
#  add_custom_command(OUTPUT "C3/CMakeFiles.txt"
#       COMMAND git ARGS submodule init
#       COMMAND git ARGS submodule update
#       WORKING_DIRECTORY .)
#ENDIF()
ADD_SUBDIRECTORY(C3)
