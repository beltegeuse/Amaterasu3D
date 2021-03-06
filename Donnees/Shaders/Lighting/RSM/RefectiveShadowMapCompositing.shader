<?xml version="1.0" ?>
<Shader name="BasicTextureShader" type="Basic">
	<Textures>
		<Texture type="Custom" name="DiffuseBuffer" id=0 />
		<Texture type="Custom" name="SpecularBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
		<Texture type="Custom" name="DepthBuffer" id=3 />
		<Texture type="Custom" name="DepthRSM" id=4 />
		<Texture type="Custom" name="PositionRSM" id=6 />
		<Texture type="Custom" name="FluxRSM" id=7 />
		<Texture type="Custom" name="NoiseBuffer" id=8 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="RefectiveShadowMapCompositing.frag" />
</Shader>