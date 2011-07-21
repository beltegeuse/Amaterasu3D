<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=256 height=128>
		<Frame name="outUBuffer" type="outIBuffer" />
		<Frame name="outUBuffer" type="outUBuffer" />
	</OutputFrame>
	<Textures>
		<Texture type="Custom" name="UBuffer" id=0 />
		<Texture type="Custom" name="IBuffer" id=1 />
		<Texture type="Custom" name="DeltaIBuffer" id=2 />
		<Texture type="Custom" name="DeltaUBuffer" id=3 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="Fattal2DUpdate.frag" />
</Shader>