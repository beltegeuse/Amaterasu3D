#
# Try to find FreeType library and include path.
# Once done this will define
#
# FreeType_FOUND
# FreeType_INCLUDE_DIR
# FreeType_LIBRARY
#
# Need LibFindMacros & LibFindVSPath (Win32 only)

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(FreeType_PKGCONF freetype)

IF(WIN32)
    include(LibFindVSPath)
    
	find_path(FreeType_INCLUDE_DIR_ft2build
 	 NAMES ft2build.h
  	 PATHS 
  	    ${FreeType_ROOT_DIR}/include
  	 	${FreeType_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 DOC "The directory where FreeType.h resides"
	)
	
	find_path(FreeType_INCLUDE_DIR_freetype2
 	 NAMES freetype/config/ftheader.h
	 PATH_SUFFIXES freetype2
  	 PATHS 
  	    ${FreeType_ROOT_DIR}/include
  	 	${FreeType_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 DOC "The directory where FreeType.h resides"
	)
	
	find_library(FreeType_LIBRARY
 	 NAMES freetype libfreetype
  	 PATHS 
  	    ${FreeType_ROOT_DIR}/lib
  	 	${FreeType_PKGCONF_LIBRARY_DIRS} 
  	 	${VS_DIR}/VC/lib
  	 	C:/MinGW/lib
  	 DOC "The FreeType library"
	)
	
ELSE(WIN32)
	find_path(FreeType_INCLUDE_DIR_ft2build
 	 NAMES ft2build.h
  	 PATHS 
  	    ${FreeType_ROOT_DIR}/include
  	 	${FreeType_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 DOC "The directory where FreeType.h resides"
	)
	
	find_path(FreeType_INCLUDE_DIR_freetype2
 	 NAMES freetype/config/ftheader.h
	 PATH_SUFFIXES freetype2
  	 PATHS 
  	    ${FreeType_ROOT_DIR}/include
  	 	${FreeType_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 DOC "The directory where FreeType.h resides"
	)
	
	find_library(FreeType_LIBRARY
 	 NAMES freetype libfreetype
  	 PATHS 
  	 	${FreeType_ROOT_DIR}/lib
  	 	${FreeType_PKGCONF_LIBRARY_DIRS} 
  	 	/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  	 DOC "The FreeType library"
	)
ENDIF(WIN32)

set(FreeType_INCLUDE_DIRS ${FreeType_INCLUDE_DIR_freetype2} ${FreeType_INCLUDE_DIR_ft2build})

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(FreeType_PROCESS_INCLUDES FreeType_INCLUDE_DIRS)
set(FreeType_PROCESS_LIBS FreeType_LIBRARY)
libfind_process(FreeType)