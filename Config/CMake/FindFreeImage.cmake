# Variables
IF(WIN32)
SET(FREEIMAGE_PATH_SEARCH_INCLUDE
	C:/cygwin/usr/local/include
	C:/msys/local/include
  	C:/MinGW/include
	T:/INC/MinGW/include
  	NO_DEFAULT_PATH
)
ELSE(WIN32)
SET(FREEIMAGE_PATH_SEARCH_INCLUDE
	/usr/include/SFML
  	/usr/local/include/SFML
	/temporaire/reseau/INC/SeaGull/include/SFML
	/temporaire/reseau/INC/SeaGull/include
  	NO_DEFAULT_PATH
)
ENDIF(WIN32)
SET(FREEIMAGE_PATH_SEARCH_LIB
	/usr/lib
 	/usr/local/lib
	/temporaire/reseau/INC/SeaGull/lib
  	C:/MinGW/lib
	T:/INC/MinGW/lib
  	NO_DEFAULT_PATH
)

# ------- Include ----------------

FIND_PATH(FREEIMAGE_INCLUDE_DIR FreeImage.h
	${FREEIMAGE_PATH_SEARCH_INCLUDE}
)

FIND_PATH(FREEIMAGE_INCLUDE_DIR FreeImage.h)

# ------- FreeImage ---------------

FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES libfreeimage freeimage PATHS 
  ${FREEIMAGE_PATH_SEARCH_LIB}
)

FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES libfreeimage freeimage)
IF(WIN32)
SET(FREEIMAGE_LIBRARY ${FREEIMAGE_LIBRARY} ws2_32 wsock32)
ENDIF()
MESSAGE("FREEIMAGE_LIBRARY is ${FREEIMAGE_LIBRARY}")
