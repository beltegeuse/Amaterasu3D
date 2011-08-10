SET(FontNewSystem_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(FontNewSystem ${FontNewSystem_SRCS})

target_link_libraries(
  FontNewSystem
  Amaterasu3D
)

SET_TARGET_PROPERTIES(FontNewSystem PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS FontNewSystem RUNTIME DESTINATION ${PROJECT_BUILD_PATH})