SET(LIB_AMATERASU3D_SRCS
	Debug/Exceptions.cpp
	
	Logger/Logger.cpp
	Logger/LoggerDebug.cpp
	Logger/LoggerFile.cpp
	
	Math/Rectangle.cpp
	Math/SphericalCoordinates.cpp
	
	Graphics/SceneGraph/DrawObject.cpp
	Graphics/SceneGraph/Group.cpp
	Graphics/Texture.cpp
)

SET(LIB_AMATERASU3D_HEADERS
  Empty.h
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

SET(AMATERASU3D_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(Amaterasu3D SHARED ${LIB_AMATERASU3D_SRCS})

target_link_libraries(
  Amaterasu3D
  ${SFML_WINDOW_LIBRARY}
  ${SFML_GRAPHICS_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
)

install(TARGETS Amaterasu3D 
  RUNTIME DESTINATION ${PROJECT_BUILD_PATH}
  LIBRARY DESTINATION ${PROJECT_BUILD_PATH}
  ARCHIVE DESTINATION ${PROJECT_BUILD_PATH}
)