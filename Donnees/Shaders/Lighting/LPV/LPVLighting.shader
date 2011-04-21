<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <Textures>
		<Texture type="Custom" name="DepthBuffer" id=0 />
		<Texture type="Custom" name="NormalBuffer" id=1 />
		<Texture type="Custom" name="GridRed" id=2 />
		<Texture type="Custom" name="GridGreen" id=3 />
		<Texture type="Custom" name="GridBlue" id=4 />
		<Texture type="Custom" name="DiffuseBuffer" id=5 />
	</Textures>
	<Defines>
		<Define name="NB_CASCADE" defaultValue="3" />
	</Defines>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVLighting.frag" />
</Shader>