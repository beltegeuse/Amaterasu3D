<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <Textures>
		<Texture type="Custom" name="Grid" id=0 />
		<Texture type="Custom" name="PositionBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVLighting.frag" />
</Shader>