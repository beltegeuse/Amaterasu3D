SET(BinvoxViewer_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(BinvoxViewer ${BinvoxViewer_SRCS})

target_link_libraries(
  BinvoxViewer
  Amaterasu3D
)

SET_TARGET_PROPERTIES(BinvoxViewer PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS BinvoxViewer RUNTIME DESTINATION ${PROJECT_BUILD_PATH})