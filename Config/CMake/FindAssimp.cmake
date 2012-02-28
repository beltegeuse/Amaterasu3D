#
# Try to find Assimp library and include path.
# Once done this will define
#
# Assimp_FOUND
# Assimp_INCLUDE_DIR
# Assimp_LIBRARY
#
# Need LibFindMacros & LibFindVSPath (Win32 only)

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(Assimp_PKGCONF assimp)

IF(WIN32)
    include(LibFindVSPath)
    
	find_path(Assimp_INCLUDE_DIR
 	 NAMES assimp.h
  	 PATHS 
  	    ${Assimp_ROOT_DIR}/include
  	 	${Assimp_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 DOC "The directory where assimp.h resides"
	)
	
	find_library(Assimp_LIBRARY
 	 NAMES libassimp assimp Assimp
  	 PATHS 
  	    ${Assimp_ROOT_DIR}/lib
  	 	${Assimp_PKGCONF_LIBRARY_DIRS} 
  	 	${VS_DIR}/VC/lib
  	 	C:/MinGW/lib
  	 DOC "The Assimp library"
	)
	
ELSE(WIN32)
	find_path(Assimp_INCLUDE_DIR
 	 NAMES assimp.h
  	 PATHS 
  	    ${Assimp_ROOT_DIR}/include
  	 	${Assimp_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 DOC "The directory where assimp.h resides"
	)
	
	find_library(Assimp_LIBRARY
 	 NAMES libassimp assimp Assimp
  	 PATHS 
  	 	${Assimp_ROOT_DIR}/lib
  	 	${Assimp_PKGCONF_LIBRARY_DIRS} 
  	 	/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  	 DOC "The Assimp library"
	)
ENDIF(WIN32)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Assimp_PROCESS_INCLUDES Assimp_INCLUDE_DIR)
set(Assimp_PROCESS_LIBS Assimp_LIBRARY)
libfind_process(Assimp)