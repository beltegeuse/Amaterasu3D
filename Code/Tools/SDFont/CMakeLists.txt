SET(SDFont_SRCS
	main.cpp
	BinPacker.cpp
	lodepng.cpp
	stb_image.c
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${FreeType_INCLUDE_DIRS}
	../../Libs/TinyXML
)

ADD_EXECUTABLE(SDFont ${SDFont_SRCS})

target_link_libraries(
  SDFont
  ${FreeType_LIBRARY}
  tinyxml
)

IF(MSVC)
	# Use to auto link
ELSE(MSVC)
	target_link_libraries(
		SDFont
		${Boost_LIBRARIES}
	)
ENDIF(MSVC)

SET_TARGET_PROPERTIES(SDFont PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS SDFont RUNTIME DESTINATION ${PROJECT_BUILD_PATH})