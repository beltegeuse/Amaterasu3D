include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(LibSigGCXX2_PKGCONF sigc++-2.0)

IF(WIN32)
    include(LibFindVSPath)
    
	find_path(LibSigGCXX2_INCLUDE_DIR
 	 NAMES sigc++/sigc++.h
  	 PATHS 
  	    ${LibSigGCXX2_ROOT_DIR}/include
  	 	${LibSigGCXX2_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 PATH_SUFFIXES sigc++-2.0
  	 DOC "The directory where sigc++/sigc++.h resides"
	)
	
	find_path(LibSigGCXX2_CONFIG_INCLUDE_DIR
 	 NAMES sigc++config.h
  	 PATHS 
  	    ${LibSigGCXX2_ROOT_DIR}/include
  	 	${LibSigGCXX2_PKGCONF_INCLUDE_DIRS} 
  	 	${VS_DIR}/VC/include
  	 	C:/MinGW/include
  	 PATH_SUFFIXES sigc++-2.0
  	 DOC "The directory where sigc++config.h resides"
	)
	
	find_library(LibSigGCXX2_LIBRARY
 	 NAMES sigc-2.0 libsigc-2.0 ${LibSigGCXX2_PKGCONF_LIBRARIES} 
  	 PATHS 
  	    ${LibSigGCXX2_ROOT_DIR}/lib
  	 	${LibSigGCXX2_PKGCONF_LIBRARY_DIRS} 
  	 	${VS_DIR}/VC/lib
  	 	C:/MinGW/lib
  	 DOC "The LibSigGCXX2 library"
	)
	
ELSE(WIN32)
	find_path(LibSigGCXX2_INCLUDE_DIR
 	 NAMES sigc++/sigc++.h
  	 PATHS 
  	    ${LibSigGCXX2_ROOT_DIR}/include
  	 	${LibSigGCXX2_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
	 PATH_SUFFIXES sigc++-2.0
  	 DOC "The directory where LibSigGCXX2.h resides"
	)
	
	find_path(LibSigGCXX2_CONFIG_INCLUDE_DIR
 	 NAMES sigc++config.h
  	 PATHS 
  	    ${LibSigGCXX2_ROOT_DIR}/include
  	 	${LibSigGCXX2_PKGCONF_INCLUDE_DIRS} 
  	 	/usr/include
  	 	/usr/local/include
		/sw/include
		/opt/local/include
  	 PATH_SUFFIXES sigc++-2.0
  	 DOC "The directory where sigc++config.h resides"
	)
	
	find_library(LibSigGCXX2_LIBRARY
 	 NAMES sigc-2.0 libsigc-2.0 ${LibSigGCXX2_PKGCONF_LIBRARIES} 
  	 PATHS 
  	 	${LibSigGCXX2_ROOT_DIR}/include
  	 	${LibSigGCXX2_PKGCONF_LIBRARY_DIRS}
  	 	/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
  	 DOC "The LibSigGCXX2 library"
	)
ENDIF(WIN32)

set(LibSigGCXX2_INCLUDE_DIR ${LibSigGCXX2_INCLUDE_DIR} ${LibSigGCXX2_CONFIG_INCLUDE_DIR})

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(LibSigGCXX2_PROCESS_INCLUDES LibSigGCXX2_INCLUDE_DIR LibSigGCXX2_CONFIG_INCLUDE_DIR)
set(LibSigGCXX2_PROCESS_LIBS LibSigGCXX2_LIBRARY)
libfind_process(LibSigGCXX2)