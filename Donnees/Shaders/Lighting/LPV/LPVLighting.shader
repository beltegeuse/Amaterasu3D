<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <Textures>
		<Texture type="Custom" name="PositionBuffer" id=0 />
		<Texture type="Custom" name="NormalBuffer" id=1 />
		<Texture type="Custom" name="GridRed" id=2 />
		<Texture type="Custom" name="GridGreen" id=3 />
		<Texture type="Custom" name="GridBlue" id=4 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVLighting.frag" />
</Shader>