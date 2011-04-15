IF(WIN32)
ADD_SUBDIRECTORY(OpenGL3)
ENDIF()

# Unsupported :
#  * Compositing Pass
#    - Check Texture ID [main.cpp]
#    - Add compute RSM Position (InverseCamera matrix ... etc) [GLSL]
#    - Pass all uniform to GLSL [main.cpp]
#ADD_SUBDIRECTORY(Amaterasu3DReflectiveShadowMapping)

ADD_SUBDIRECTORY(Amaterasu3D)
ADD_SUBDIRECTORY(Amaterasu3DGBuffer)
ADD_SUBDIRECTORY(Amaterasu3DGeometryShader)
ADD_SUBDIRECTORY(Amaterasu3DLPV)
ADD_SUBDIRECTORY(Amaterasu3DSSAO)
ADD_SUBDIRECTORY(ShadowMap)
