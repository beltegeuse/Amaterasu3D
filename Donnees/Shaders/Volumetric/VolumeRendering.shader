<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<Textures>
		<Texture type="Custom" name="FrontBuffer" id=0 />
		<Texture type="Custom" name="BackBuffer" id=1 />
		<Texture type="Custom" name="VolumeBuffer" id=2 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="VolumeRendering2.frag" />
</Shader>