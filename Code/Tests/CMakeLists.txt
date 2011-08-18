#
# Experimental
#
IF(WIN32)
#SUBDIRS(OpenGL3)
#SUBDIRS(Amaterasu3D)
#SUBDIRS(NeheLesson1)
ELSE()
#SUBDIRS(OpenCLRay)
ENDIF()

INCLUDE_DIRECTORIES(
	../Libs/Assimp/include
	../Libs/TinyXML
	../Libs/C3/Source
)

# Unsupported :
#  * Compositing Pass
#    - Check Texture ID [main.cpp]
#    - Add compute RSM Position (InverseCamera matrix ... etc) [GLSL]
#    - Pass all uniform to GLSL [main.cpp]
#SUBDIRS(Amaterasu3DReflectiveShadowMapping)
#SUBDIRS(Amaterasu3DWhiteBalancing)

SUBDIRS(Amaterasu3DGBuffer)
SUBDIRS(Amaterasu3DGeometryShader)
SUBDIRS(Amaterasu3DLPV)
SUBDIRS(Amaterasu3DSSAO)
SUBDIRS(CameraPath)
SUBDIRS(BinvoxViewer)
SUBDIRS(ShadowMap)
SUBDIRS(Mipmapping)
SUBDIRS(FastBilateralFiltering)
SUBDIRS(Amaterasu3DFattal)
SUBDIRS(Jinja2Test)
#SUBDIRS(SHDOMVisualizer)
SUBDIRS(TestCustomAttributs)
SUBDIRS(FontNewSystem)