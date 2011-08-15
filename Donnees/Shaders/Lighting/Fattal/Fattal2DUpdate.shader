<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=256 height=128>
		<Frame name="outUBuffer" type="RGBA32" />
		<Frame name="outIBuffer" type="RGBA32" />
	</OutputFrame>
	<Textures>
		<Texture type="Custom" name="UBuffer" id=0 />
		<Texture type="Custom" name="IBuffer" id=1 />
		<Texture type="Custom" name="DeltaBuffer" id=2 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="Fattal2DUpdate.frag" />
</Shader>