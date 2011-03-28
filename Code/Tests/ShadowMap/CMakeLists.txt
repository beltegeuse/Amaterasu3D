SET(Amaterasu3DShadow_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(Amaterasu3DShadow ${Amaterasu3DShadow_SRCS})

target_link_libraries(
  Amaterasu3DShadow
  Amaterasu3D
)

SET_TARGET_PROPERTIES(Amaterasu3DShadow PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS Amaterasu3DShadow RUNTIME DESTINATION ${PROJECT_BUILD_PATH})