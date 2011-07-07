
IF(CMAKE_BUILD_TYPE MATCHES debug)
	ADD_DEFINITIONS("-Wall -W -D_DEBUG -pg") #Pour afficher les warning lors de la compilation
    
    #IF(LINUX)
    	#ADD_DEFINITIONS("-pg")
    #ENDIF()
    
    #SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} " -D_DEBUG -ansi -Wall -Wextra -Wfatal-errors -pedantic -Wmain -Wswitch-default -Wunreachable-code -Winit-self -Wshadow -Wwrite-strings -Wuninitialized") #Pour afficher les warning lors de la compilation
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)

SET(LIB_AMATERASU3D_SRCS
	#
	# Addons Section
	#
	Addons/Console/Console.cpp
	Addons/Console/DefaultLook.cpp
	Addons/FPS/FPS.cpp
	Addons/Logo/Logo.cpp
	Addons/PerformancePanel/PerformancePanel.cpp
	Addons/Binvox/BinvoxModel.cpp
	Addons/Binvox/prop.cpp
	Addons/LPV/LPV.cpp
	Addons/ToneMappingOperator/AbsrtactToneOperator.cpp
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.cpp
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.cpp
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.cpp
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.cpp
	Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.cpp
	Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.cpp
	
	#
	# Math Section
	#
	Math/Rectangle.cpp
	Math/SphericalCoordinates.cpp
	Math/Quaternion.cpp
	#
	# Graphics Section
	#
	
	### SceneGraph
	Graphics/SceneNode/ICameraSceneNode.cpp  
	Graphics/SceneNode/IMeshSceneNode.cpp        
	Graphics/SceneNode/ISceneNode.cpp
 	Graphics/SceneNode/ILightSceneNode.cpp   
 	Graphics/SceneNode/IRenderableSceneNode.cpp  
 	Graphics/SceneNode/ISimpleRenderableSceneNode.cpp
 	Graphics/SceneNode/Others/Skydome.cpp
 	Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.cpp
 	Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.cpp
 	Graphics/SceneNode/SimpleRenderable/Rectangle2D.cpp
 	
	### Other Graphics
	Graphics/Font/FontManager.cpp
	Graphics/Font/GraphicString.cpp
	Graphics/Lighting/LightingPassAbstract.cpp
	Graphics/Lighting/DeferredLighting/DeferredLighting.cpp
	Graphics/Camera/CameraAbstract.cpp
	Graphics/Camera/CameraFPS.cpp
	Graphics/Camera/FixedCamera.cpp
	
	### Shader Graphics
	Graphics/Shaders/Shader.cpp
	Graphics/Shaders/ShaderUnit.cpp
	Graphics/Shaders/GBufferShader.cpp
	Graphics/Shaders/Compiler/ShaderCompiler.cpp
	Graphics/Shaders/Tools/ManualMipmapping.cpp
	### Core Graphics
	Graphics/MatrixManagement.cpp
	Graphics/Color.cpp
	Graphics/FBO.cpp
	Graphics/Texture.cpp
	Graphics/SceneManager.cpp
	Graphics/RenderableObject.cpp
	
	#
	# System Section
	#
	System/GraphicsDevice.cpp
	System/EventListeners.cpp
	System/EventManager.cpp
	System/SettingsManager.cpp
	System/MediaManager.cpp
	System/ResourceManager.cpp
	System/Resource.cpp
	System/Loaders/Loaders.cpp
	System/Loaders/ShadersLoader.cpp
	System/Loaders/AssimpLoader.cpp
	System/Loaders/ImagesLoader.cpp
	
	#
	# Other Section
	#
	Debug/Exceptions.cpp
	Debug/OpenGLDebug.cpp
	Logger/Logger.cpp
	Logger/LoggerDebug.cpp
	Logger/LoggerFile.cpp
	Utilities/File.cpp
	
	Application.cpp
)

SET(LIB_AMATERASU3D_HEADERS
  Empty.h
)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${FREETYPE_INCLUDE_DIRS}
    /usr/local/include/sigc++-2.0
	/usr/local/lib/sigc++-2.0/include
	/usr/local/include/assimp
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
