#
# Try to find FreeImage library and include path.
# Once done this will define
#
# FreeImage_FOUND
# FreeImage_INCLUDE_DIR
# FreeImage_LIBRARY
#
# Need LibFindMacros & LibFindVSPath (Win32 only)

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(FreeImage_PKGCONF FreeImage)

IF(WIN32)
    include(LibFindVSPath)
    
	find_path(FreeImage_INCLUDE_DIR
 	 NAMES FreeImage.h
  	 PATHS 
  	    ${FreeImage_ROOT_DIR}/include
  	 	${FreeImage_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 DOC "The directory where FreeImage.h resides"
	)
	
	find_library(FreeImage_LIBRARY
 	 NAMES libfreeimage freeimage FreeImage
  	 PATHS 
  	    ${FreeImage_ROOT_DIR}/lib
  	 	${FreeImage_PKGCONF_LIBRARY_DIRS} 
  	 	${VS_DIR}/VC/lib
  	 	C:/MinGW/lib
  	 DOC "The FreeImage library"
	)
	
ELSE(WIN32)
	find_path(FreeImage_INCLUDE_DIR
 	 NAMES FreeImage.h
  	 PATHS 
  	    ${FreeImage_ROOT_DIR}/include
  	 	${FreeImage_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 DOC "The directory where FreeImage.h resides"
	)
	
	find_library(FreeImage_LIBRARY
 	 NAMES libfreeimage freeimage
  	 PATHS 
  	 	${FreeImage_ROOT_DIR}/include
  	 	${FreeImage_PKGCONF_LIBRARY_DIRS} 
  	 	/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  	 DOC "The FreeImage library"
	)
ENDIF(WIN32)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(FreeImage_PROCESS_INCLUDES FreeImage_INCLUDE_DIR)
set(FreeImage_PROCESS_LIBS FreeImage_LIBRARY)
libfind_process(FreeImage)