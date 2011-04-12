<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <Textures>
		<Texture type="Custom" name="GridRed" id=0 />
		<Texture type="Custom" name="GridGreen" id=1 />
		<Texture type="Custom" name="GridBlue" id=2 />
		
		<Texture type="Custom" name="DepthBuffer" id=3 />
		<Texture type="Custom" name="NormalBuffer" id=4 />
		<Texture type="Custom" name="DiffuseBuffer" id=5 />
		<Texture type="Custom" name="SpecularBuffer" id=6 />
		<Texture type="Custom" name="ShadowBuffer" id=7 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVLightingAll.frag" />
</Shader>