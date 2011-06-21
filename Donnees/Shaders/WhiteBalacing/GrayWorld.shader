<?xml version="1.0" ?>
<Shader name="SphereProjectionHemiCube" type="Basic">
    <Textures>
		<Texture type="Custom" name="LDRBuffer" id=0/>
		<Texture type="Custom" name="LumBuffer" id=1/>
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="GrayWorld.frag" />
</Shader>