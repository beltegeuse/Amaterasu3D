# Unittest (only for Linux)
IF(LINUX)
	SUBDIRS(Unittests)
ENDIF(LINUX)

# Techniques
SUBDIRS(Techniques/GBuffer
		Techniques/LPV
		Techniques/SSAO
		Techniques/ShadowMap
		Techniques/WhiteBalancing
)