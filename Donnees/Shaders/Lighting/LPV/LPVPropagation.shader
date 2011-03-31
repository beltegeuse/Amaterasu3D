<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="GridRed" type="RGBA16" />
    	<Frame name="GridGreen" type="RGBA16" />
    	<Frame name="GridBlue" type="RGBA16" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="LPVRed" id=0 />
		<Texture type="Custom" name="LPVGreen" id=1 />
		<Texture type="Custom" name="LPVBlue" id=2 />
		<Texture type="Custom" name="Occlusion" id=3 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVPropagation.frag" />
</Shader>