SET(Amaterasu3DTextureTest_SRCS
	main.cpp
)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../../Libs/Amaterasu3D
)

ADD_EXECUTABLE(Amaterasu3DTextureTest ${Amaterasu3DTextureTest_SRCS})

target_link_libraries(
  Amaterasu3DTextureTest
  Amaterasu3D
  assimp
  ${SDL_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
)

SET_TARGET_PROPERTIES(Amaterasu3DTextureTest PROPERTIES
  INSTALL_RPATH ${PROJECT_BUILD_PATH}
  INSTALL_RPATH_USE_LINK_PATH true
)

install(TARGETS Amaterasu3DTextureTest RUNTIME DESTINATION ${PROJECT_BUILD_PATH})