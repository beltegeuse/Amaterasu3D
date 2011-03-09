SET(Amaterasu3DTestApp_SRCS
	main.cpp
	helpers.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(Amaterasu3DTestApp ${Amaterasu3DTestApp_SRCS})

target_link_libraries(
  Amaterasu3DTestApp
  Amaterasu3D
)

SET_TARGET_PROPERTIES(Amaterasu3DTestApp PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS Amaterasu3DTestApp RUNTIME DESTINATION ${PROJECT_BUILD_PATH})