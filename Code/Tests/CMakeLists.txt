IF(WIN32)
ADD_SUBDIRECTORY(OpenGL3)
ADD_SUBDIRECTORY(OpenGLPWMTest)
ADD_SUBDIRECTORY(Amaterasu3D)
ADD_SUBDIRECTORY(NeheLesson1)
ENDIF()

INCLUDE_DIRECTORIES(
	${FREETYPE_INCLUDE_DIRS}
    /usr/local/include/sigc++-2.0
	/usr/local/lib/sigc++-2.0/include
	/usr/local/include/assimp
	../Libs/TinyXML
)

# Unsupported :
#  * Compositing Pass
#    - Check Texture ID [main.cpp]
#    - Add compute RSM Position (InverseCamera matrix ... etc) [GLSL]
#    - Pass all uniform to GLSL [main.cpp]
#ADD_SUBDIRECTORY(Amaterasu3DReflectiveShadowMapping)
ADD_SUBDIRECTORY(Amaterasu3DGBuffer)
ADD_SUBDIRECTORY(Amaterasu3DGeometryShader)
ADD_SUBDIRECTORY(Amaterasu3DWhiteBalancing)
ADD_SUBDIRECTORY(Amaterasu3DLPV)
ADD_SUBDIRECTORY(Amaterasu3DSSAO)
ADD_SUBDIRECTORY(CameraPath)
ADD_SUBDIRECTORY(BinvoxViewer)
ADD_SUBDIRECTORY(ShadowMap)
ADD_SUBDIRECTORY(Mipmapping)
ADD_SUBDIRECTORY(TestCould)
