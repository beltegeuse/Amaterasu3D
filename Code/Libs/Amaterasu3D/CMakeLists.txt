
IF(CMAKE_BUILD_TYPE MATCHES debug)
    IF(WIN32)
    	ADD_DEFINITIONS("-Wall -W -D_DEBUG")
    ELSE(WIN32)
    	ADD_DEFINITIONS("-Wall -W -D_DEBUG -pg")
    ENDIF(WIN32)
ENDIF(CMAKE_BUILD_TYPE MATCHES debug)


INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Dir name 
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Application.cpp
	Exceptions.cpp
	Logger.cpp
	LoggerDebug.cpp
	LoggerFile.cpp
	OpenGLDebug.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Application.h
	Enum.h
	Exceptions.h
	Logger.h
	LoggerDebug.h
	LoggerFile.h
	OpenGLDebug.h
	SDLUtilities.h
	Singleton.h
	TinyXMLHelper.h
)



# Dir name Addons/Console
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/Console/Console.cpp
	Addons/Console/DefaultLook.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/Console/Console.h
	Addons/Console/DefaultLook.h
	Addons/Console/Functor.h
	Addons/Console/Look.h
	Addons/Console/Functor.inl
)


# Dir name Addons/FPS
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/FPS/FPS.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/FPS/FPS.h
)


# Dir name Addons/Logo
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/Logo/Logo.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/Logo/Logo.h
)


# Dir name Addons/LPV
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/LPV/LPV.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/LPV/LPV.h
)


# Dir name Addons/PerformancePanel
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/PerformancePanel/PerformancePanel.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/PerformancePanel/PerformancePanel.h
)


# Dir name Addons/ToneMappingOperator
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/ToneMappingOperator/AbsrtactToneOperator.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/ToneMappingOperator/AbsrtactToneOperator.h
)



# Dir name Addons/ToneMappingOperator/AdaptationLuminance/Global
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.cpp
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimple.h
	Addons/ToneMappingOperator/AdaptationLuminance/Global/AdaptationLuminanceSimpleExp.h
)


# Dir name Addons/ToneMappingOperator/DurandToneOperator
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.h
)


# Dir name Addons/ToneMappingOperator/PhotographicToneOperator
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.cpp
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.h
	Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.h
)


# Dir name Addons/ToneMappingOperator/SumantToneOperator
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Addons/ToneMappingOperator/SumantToneOperator/BleachingFilter.h
	Addons/ToneMappingOperator/SumantToneOperator/ExpFilter.h
	Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.h
)


# Dir name Graphics
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Color.cpp
	Graphics/FBO.cpp
	Graphics/MatrixManagement.cpp
	Graphics/RenderableObject.cpp
	Graphics/SceneManager.cpp
	Graphics/Texture.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Color.h
	Graphics/FBO.h
	Graphics/MatrixManagement.h
	Graphics/RenderableObject.h
	Graphics/SceneManager.h
	Graphics/Texture.h
)


# Dir name Graphics/Camera
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Camera/CameraAbstract.cpp
	Graphics/Camera/CameraFixed.cpp
	Graphics/Camera/CameraFPS.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Camera/CameraAbstract.h
	Graphics/Camera/CameraAnimation.h
	Graphics/Camera/CameraFixed.h
	Graphics/Camera/CameraFPS.h
)


# Dir name Graphics/Font
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Font/FontManager.cpp
	Graphics/Font/GraphicString.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Font/FontManager.h
	Graphics/Font/GraphicString.h
)


# Dir name Graphics/Lighting
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Lighting/LightingPassAbstract.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Lighting/LightingPassAbstract.h
	Graphics/Lighting/LightingStructures.h
	Graphics/Lighting/LightShaders.h
)


# Dir name Graphics/Lighting/DeferredLighting
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Lighting/DeferredLighting/DeferredLighting.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Lighting/DeferredLighting/DeferredLighting.h
)


# Dir name Graphics/SceneNode
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/SceneNode/ICameraSceneNode.cpp
	Graphics/SceneNode/ILightSceneNode.cpp
	Graphics/SceneNode/IMeshSceneNode.cpp
	Graphics/SceneNode/IRenderableSceneNode.cpp
	Graphics/SceneNode/ISceneNode.cpp
	Graphics/SceneNode/ISimpleRenderableSceneNode.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/SceneNode/ICameraSceneNode.h
	Graphics/SceneNode/ILightSceneNode.h
	Graphics/SceneNode/IMeshSceneNode.h
	Graphics/SceneNode/IRenderableSceneNode.h
	Graphics/SceneNode/ISceneNode.h
	Graphics/SceneNode/ISimpleRenderableSceneNode.h
)


# Dir name Graphics/SceneNode/Others
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/SceneNode/Others/Skydome.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/SceneNode/Others/Skydome.h
)


# Dir name Graphics/SceneNode/SimpleRenderable
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.cpp
	Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.cpp
	Graphics/SceneNode/SimpleRenderable/Rectangle2D.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.h
	Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.h
	Graphics/SceneNode/SimpleRenderable/Rectangle2D.h
)


# Dir name Graphics/Shaders
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Shaders/GBufferShader.cpp
	Graphics/Shaders/Shader.cpp
	Graphics/Shaders/ShaderUnit.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Shaders/GBufferShader.h
	Graphics/Shaders/Shader.h
	Graphics/Shaders/ShaderUnit.h
)


# Dir name Graphics/Shaders/Compiler
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Shaders/Compiler/ShaderCompiler.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Shaders/Compiler/ShaderCompiler.h
)


# Dir name Graphics/Shaders/Tools
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Graphics/Shaders/Tools/ManualMipmapping.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Graphics/Shaders/Tools/ManualMipmapping.h
)


# Dir name Math
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Math/Rectangle.cpp
	Math/SphericalCoordinates.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Math/InterpolatorCatmullRom.h
	Math/Rectangle.h
	Math/SphericalCoordinates.h
)


# Dir name System
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	System/EventListeners.cpp
	System/EventManager.cpp
	System/GraphicsDevice.cpp
	System/MediaManager.cpp
	System/PythonInterpreter.cpp
	System/Resource.cpp
	System/ResourceManager.cpp
	System/SettingsManager.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	System/EventListeners.h
	System/EventManager.h
	System/GraphicsDevice.h
	System/Input.h
	System/IXMLSerialisable.h
	System/MediaManager.h
	System/PythonInterpreter.h
	System/Resource.h
	System/ResourceManager.h
	System/SettingsManager.h
	System/MediaManager.inl
	System/ResourceManager.inl
)


# Dir name System/Loaders
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	System/Loaders/AssimpLoader.cpp
	System/Loaders/ImagesLoader.cpp
	System/Loaders/Loaders.cpp
	System/Loaders/ShadersLoader.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	System/Loaders/AssimpLoader.h
	System/Loaders/ImagesLoader.h
	System/Loaders/Loader.h
	System/Loaders/Loaders.h
	System/Loaders/ShadersLoader.h
)


# Dir name System/Python
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	System/Python/Jinja2Template.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	System/Python/Jinja2Template.h
)


# Dir name Utilities
SET(SOURCES_AMA3D ${SOURCES_AMA3D} 
	Utilities/File.cpp
	Utilities/StringUtils.cpp
)

SET(HEADERS_AMA3D ${HEADERS_AMA3D} 
	Utilities/File.h
	Utilities/GenHierarchies.h
	Utilities/SmartPtr.h
	Utilities/SmartPtrPolicies.h
	Utilities/StringUtils.h
	Utilities/Typelist.h
	Utilities/Util.h
	Utilities/SmartPtr.inl
	Utilities/StringUtils.inl
	Utilities/Typelist.inl
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
