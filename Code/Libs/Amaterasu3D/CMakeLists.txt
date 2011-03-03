SET(LIB_AMATERASU3D_SRCS
	Debug/Exceptions.cpp
	Debug/OpenGLDebug.cpp
	
	Logger/Logger.cpp
	Logger/LoggerDebug.cpp
	Logger/LoggerFile.cpp
	
	Math/Rectangle.cpp
	Math/SphericalCoordinates.cpp
	
	Graphics/SceneGraph/DrawObject.cpp
	Graphics/SceneGraph/Group.cpp
	Graphics/Font/FontManager.cpp
	Graphics/Font/GraphicString.cpp
	#Graphics/Texture.cpp
	#Graphics/ImageLoader.cpp
	Graphics/Color.cpp
	Graphics/Shader.cpp
	Graphics/Window.cpp
	
	System/MediaManager.cpp
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
  ${FREETYPE_LIBRARY}
  ${ZLIB_LIBRARY}
  ${SDL_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
  ${Boost_LIBRARIES}
)

install(TARGETS Amaterasu3D 
  RUNTIME DESTINATION ${PROJECT_BUILD_PATH}
  LIBRARY DESTINATION ${PROJECT_BUILD_PATH}
  ARCHIVE DESTINATION ${PROJECT_BUILD_PATH}
)