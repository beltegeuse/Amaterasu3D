SET(Amaterasu3DGeometry_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(Amaterasu3DGeometry ${Amaterasu3DGeometry_SRCS})

target_link_libraries(
  Amaterasu3DGeometry
  Amaterasu3D
)

SET_TARGET_PROPERTIES(Amaterasu3DGeometry PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS Amaterasu3DGeometry RUNTIME DESTINATION ${PROJECT_BUILD_PATH})