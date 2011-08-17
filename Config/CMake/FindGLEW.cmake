#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIR
# GLEW_LIBRARY
#
# Need LibFindMacros & LibFindVSPath (Win32 only)

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(GLEW_PKGCONF GLEW)

IF(WIN32)
    include(LibFindVSPath)
    
	find_path(GLEW_INCLUDE_DIR
 	 NAMES GL/glew.h
  	 PATHS 
  	    ${GLEW_ROOT_DIR}/include
  	 	${GLEW_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 DOC "The directory where GLEW.h resides"
	)
	
	find_library(GLEW_LIBRARY
 	 NAMES libglew glew glew32 glew32s
  	 PATHS 
  	    ${GLEW_ROOT_DIR}/lib
  	 	${GLEW_PKGCONF_LIBRARY_DIRS} 
  	 	${VS_DIR}/VC/lib
  	 	C:/MinGW/lib
  	 DOC "The GLEW library"
	)
	
ELSE(WIN32)
	find_path(GLEW_INCLUDE_DIR
 	 NAMES GL/glew.h
  	 PATHS 
  	    ${GLEW_ROOT_DIR}/include
  	 	${GLEW_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 DOC "The directory where GLEW.h resides"
	)
	
	find_library(GLEW_LIBRARY
 	 NAMES libglew glew glew32 glew32s
  	 PATHS 
  	 	${GLEW_ROOT_DIR}/include
  	 	${GLEW_PKGCONF_LIBRARY_DIRS} 
  	 	/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  	 DOC "The GLEW library"
	)
ENDIF(WIN32)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(GLEW_PROCESS_INCLUDES GLEW_INCLUDE_DIR)
set(GLEW_PROCESS_LIBS GLEW_LIBRARY)
libfind_process(GLEW)
