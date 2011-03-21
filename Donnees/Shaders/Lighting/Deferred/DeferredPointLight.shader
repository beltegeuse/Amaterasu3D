<?xml version="1.0" ?>
<Shader name="BasicTextureShader" type="Basic">
	<Textures>
		<Texture type="Custom" name="DiffuseBuffer" id=0 />
		<Texture type="Custom" name="SpecularBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
		<Texture type="Custom" name="PositionBuffer" id=3 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="DeferredPointLight.frag" />
</Shader>