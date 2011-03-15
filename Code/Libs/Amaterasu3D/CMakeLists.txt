SET(LIB_AMATERASU3D_SRCS
	Debug/Exceptions.cpp
	Debug/OpenGLDebug.cpp
	
	Logger/Logger.cpp
	Logger/LoggerDebug.cpp
	Logger/LoggerFile.cpp
	
	Math/Rectangle.cpp
	Math/SphericalCoordinates.cpp
	
	Graphics/Lighting/LightingPassAbstract.cpp
	Graphics/Lighting/DeferredLighting/DeferredLighting.cpp
	Graphics/Camera/CameraAbstract.cpp
	Graphics/Camera/CameraFly.cpp
	Graphics/SceneGraph/DrawObject.cpp
	Graphics/SceneGraph/Group.cpp
	Graphics/SceneGraph/Debug/DebugCubeLeaf.cpp
	Graphics/SceneGraph/Assimp/AssimpMesh.cpp
	Graphics/SceneGraph/Assimp/AssimpNode.cpp
	Graphics/Font/FontManager.cpp
	Graphics/Font/GraphicString.cpp
	Graphics/Color.cpp
	Graphics/FBO.cpp
	Graphics/GLSLShader.cpp
	Graphics/Window.cpp
	Graphics/Texture.cpp
	
	System/MediaManager.cpp
	System/ResourceManager.cpp
	System/Resource.cpp
	System/Loaders/Loaders.cpp
	System/Loaders/ShadersLoader.cpp
	System/Loaders/AssimpLoader.cpp
	System/Loaders/ImagesLoader.cpp
	
	Utilities/File.cpp
)

SET(LIB_AMATERASU3D_HEADERS
  Empty.h
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	../TinyXML
)

SET(AMATERASU3D_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(Amaterasu3D SHARED ${LIB_AMATERASU3D_SRCS})

target_link_libraries(
  Amaterasu3D
  ${FREETYPE_LIBRARY}
  ${FREEIMAGE_LIBRARY}
  ${ZLIB_LIBRARY}
  ${SDL_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
  tinyxml
  sigc-2.0 
  assimp
  #TODO: Boost is until used?
  ${Boost_LIBRARIES}
)

install(TARGETS Amaterasu3D 
  RUNTIME DESTINATION ${PROJECT_BUILD_PATH}
  LIBRARY DESTINATION ${PROJECT_BUILD_PATH}
  ARCHIVE DESTINATION ${PROJECT_BUILD_PATH}
)