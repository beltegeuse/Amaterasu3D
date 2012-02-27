
IF(CMAKE_BUILD_TYPE MATCHES debug)
    IF(WIN32)
    	ADD_DEFINITIONS("-Wall -W -D_DEBUG")
    ELSE(WIN32)
    	ADD_DEFINITIONS("-Wall -W -D_DEBUG -pg")
    ENDIF(WIN32)
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)


SET(LIST_DIR 
	# ADDONS
	Addons
	Addons/FPS
	Addons/Logo
	Addons/PerformancePanel
	Addons/Binvox
	Addons/LPV
	Addons/ToneMappingOperator
	Addons/ToneMappingOperator/PhotographicToneOperator
	Addons/ToneMappingOperator/AdaptationLuminance
	Addons/ToneMappingOperator/SumantToneOperator
	Addons/ToneMappingOperator/DurandToneOperator
	
	Math
	
	Graphics
	Graphics/SceneNode
	Graphics/Font
	Graphics/Lighting
	Graphics/Camera
	Graphics/Shaders
	
	Debug
	Logger
	Utilities
)

SET(SOURCES_AMA3D)
SET(HEADERS_AMA3D)

list(LENGTH LIST_DIR len1)
math(EXPR len2 "${len1} - 1")

foreach(val RANGE ${len2})
	list(GET LIST_DIR ${val} dir)
	#MESSAGE("Directory : ${dir}")
	FILE(GLOB files_cpp "${dir}/*.cpp")
	FILE(GLOB files_headers "${dir}/*.h")
	
	#MESSAGE("Files : ${files_cpp}")
	
	# ADD TO GLOBAL LISTS
	SET(SOURCES_AMA3D ${SOURCES_AMA3D} ${files_cpp})
	SET(HEADERS_AMA3D ${HEADERS_AMA3D} ${files_headers})
	
	SOURCE_GROUP(${dir} FILES  ${files_cpp} ${files_headers})
ENDFOREACH()

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

SET(AMATERASU3D_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

IF(MSVC)
	ADD_LIBRARY(Amaterasu3D STATIC ${SOURCES_AMA3D} ${HEADERS_AMA3D})
ELSE()
	ADD_LIBRARY(Amaterasu3D SHARED ${SOURCES_AMA3D})
ENDIF()

target_link_libraries(
  Amaterasu3D
  
  # External dependencies
  ${FreeType_LIBRARY}
  ${FreeImage_LIBRARY}
  ${ZLIB_LIBRARY}
  ${GLEW_LIBRARY}
  ${OPENGL_LIBRARY}
  ${PYTHON_LIBRARIES}
  ${Assimp_LIBRARY}
  
  # Internal dependencies
  tinyxml
  C3
)

IF(MSVC)
	# Use to auto link
ELSE(MSVC)
	target_link_libraries(
	Amaterasu3D
	${Boost_LIBRARIES}
	)
ENDIF(MSVC)

install(TARGETS Amaterasu3D 
  LIBRARY DESTINATION ${Amaterasu3D_BINARY_DIR}
  ARCHIVE DESTINATION ${Amaterasu3D_BINARY_DIR}
)

###################
# Setup VS
###################
# SOURCE_GROUP(Addons FILES 
	# Addons/Console/Console.cpp
	# Addons/Console/DefaultLook.cpp
	# Addons/FPS/FPS.cpp
	# Addons/Logo/Logo.cpp
	# Addons/PerformancePanel/PerformancePanel.cpp
	# Addons/Binvox/BinvoxModel.cpp
	# Addons/Binvox/VolumetricRenderableObject.cpp
	# Addons/Binvox/SHDOMFilePropreties.cpp
	# Addons/LPV/LPV.cpp
	# Addons/ToneMappingOperator/AbsrtactToneOperator.cpp
	# Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.cpp
	# Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.cpp
	# Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.cpp
	# Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.cpp
	# Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.cpp
	# Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.cpp
# )

# SOURCE_GROUP(Math FILES 
	# Math/Rectangle.cpp
	# Math/SphericalCoordinates.cpp
	# Math/Quaternion.cpp
# )

# SOURCE_GROUP(Graphics FILES 
	# Graphics/SceneNode/ICameraSceneNode.cpp  
	# Graphics/SceneNode/IMeshSceneNode.cpp        
	# Graphics/SceneNode/ISceneNode.cpp
 	# Graphics/SceneNode/ILightSceneNode.cpp   
 	# Graphics/SceneNode/IRenderableSceneNode.cpp  
 	# Graphics/SceneNode/ISimpleRenderableSceneNode.cpp
 	# Graphics/SceneNode/Others/Skydome.cpp
 	# Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.cpp
 	# Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.cpp
 	# Graphics/SceneNode/SimpleRenderable/Rectangle2D.cpp
 	
	# ### Other Graphics
	# Graphics/Font/FontManager.cpp
	# Graphics/Font/GraphicString.cpp
	# Graphics/Lighting/LightingPassAbstract.cpp
	# Graphics/Lighting/DeferredLighting/DeferredLighting.cpp
	# Graphics/Camera/CameraAbstract.cpp
	# Graphics/Camera/CameraFPS.cpp
	# Graphics/Camera/FixedCamera.cpp
	
	# ### Shader Graphics
	# Graphics/Shaders/Shader.cpp
	# Graphics/Shaders/ShaderUnit.cpp
	# Graphics/Shaders/GBufferShader.cpp
	# Graphics/Shaders/Compiler/ShaderCompiler.cpp
	# Graphics/Shaders/Tools/ManualMipmapping.cpp
	# ### Core Graphics
	# Graphics/MatrixManagement.cpp
	# Graphics/Color.cpp
	# Graphics/FBO.cpp
	# Graphics/Texture.cpp
	# Graphics/SceneManager.cpp
	# Graphics/RenderableObject.cpp
# )

# SOURCE_GROUP(Others FILES
	# Debug/Exceptions.cpp
	# Debug/OpenGLDebug.cpp
	# Logger/Logger.cpp
	# Logger/LoggerDebug.cpp
	# Logger/LoggerFile.cpp
	# Utilities/File.cpp
	# Utilities/StringUtils.cpp
	# Application.cpp
# )

# SOURCE_GROUP(System FILES 
	# System/GraphicsDevice.cpp
	# System/EventListeners.cpp
	# System/EventManager.cpp
	# System/SettingsManager.cpp
	# System/MediaManager.cpp
	# System/ResourceManager.cpp
	# System/Resource.cpp
	# System/PythonInterpreter.cpp
	# System/Loaders/Loaders.cpp
	# System/Loaders/ShadersLoader.cpp
	# System/Loaders/AssimpLoader.cpp
	# System/Loaders/ImagesLoader.cpp
	# System/Python/Jinja2Template.cpp
# )



###################
# SOURCES
###################
# SET(LIB_AMATERASU3D_SRCS
	# #
	# # Addons Section
	# #
	# Addons/Console/Console.cpp
	# Addons/Console/DefaultLook.cpp
	# Addons/FPS/FPS.cpp
	# Addons/Logo/Logo.cpp
	# Addons/PerformancePanel/PerformancePanel.cpp
	# Addons/Binvox/BinvoxModel.cpp
	# Addons/Binvox/VolumetricRenderableObject.cpp
	# Addons/Binvox/SHDOMFilePropreties.cpp
	# Addons/LPV/LPV.cpp
	# Addons/ToneMappingOperator/AbsrtactToneOperator.cpp
	# Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.cpp
	# Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.cpp
	# Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.cpp
	# Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.cpp
	# Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.cpp
	# Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.cpp
	
	# #
	# # Math Section
	# #
	# Math/Rectangle.cpp
	# Math/SphericalCoordinates.cpp
	# Math/Quaternion.cpp
	
	# #
	# # Graphics Section
	# #
	
	# ### SceneGraph
	# Graphics/SceneNode/ICameraSceneNode.cpp  
	# Graphics/SceneNode/IMeshSceneNode.cpp        
	# Graphics/SceneNode/ISceneNode.cpp
 	# Graphics/SceneNode/ILightSceneNode.cpp   
 	# Graphics/SceneNode/IRenderableSceneNode.cpp  
 	# Graphics/SceneNode/ISimpleRenderableSceneNode.cpp
 	# Graphics/SceneNode/Others/Skydome.cpp
 	# Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.cpp
 	# Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.cpp
 	# Graphics/SceneNode/SimpleRenderable/Rectangle2D.cpp
 	
	# ### Other Graphics
	# Graphics/Font/FontManager.cpp
	# Graphics/Font/GraphicString.cpp
	# Graphics/Lighting/LightingPassAbstract.cpp
	# Graphics/Lighting/DeferredLighting/DeferredLighting.cpp
	# Graphics/Camera/CameraAbstract.cpp
	# Graphics/Camera/CameraFPS.cpp
	# Graphics/Camera/FixedCamera.cpp
	
	# ### Shader Graphics
	# Graphics/Shaders/Shader.cpp
	# Graphics/Shaders/ShaderUnit.cpp
	# Graphics/Shaders/GBufferShader.cpp
	# Graphics/Shaders/Compiler/ShaderCompiler.cpp
	# Graphics/Shaders/Tools/ManualMipmapping.cpp
	# ### Core Graphics
	# Graphics/MatrixManagement.cpp
	# Graphics/Color.cpp
	# Graphics/FBO.cpp
	# Graphics/Texture.cpp
	# Graphics/SceneManager.cpp
	# Graphics/RenderableObject.cpp
	
	# #
	# # System Section
	# #
	# System/GraphicsDevice.cpp
	# System/EventListeners.cpp
	# System/EventManager.cpp
	# System/SettingsManager.cpp
	# System/MediaManager.cpp
	# System/ResourceManager.cpp
	# System/Resource.cpp
	# System/PythonInterpreter.cpp
	# System/Loaders/Loaders.cpp
	# System/Loaders/ShadersLoader.cpp
	# System/Loaders/AssimpLoader.cpp
	# System/Loaders/ImagesLoader.cpp
	# System/Python/Jinja2Template.cpp
	# #
	# # Other Section
	# #
	# Debug/Exceptions.cpp
	# Debug/OpenGLDebug.cpp
	# Logger/Logger.cpp
	# Logger/LoggerDebug.cpp
	# Logger/LoggerFile.cpp
	# Utilities/File.cpp
	# Utilities/StringUtils.cpp
	# Application.cpp
	
# )
