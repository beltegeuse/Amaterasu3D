<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="GridRed" type="RGBA32" />
    	<Frame name="GridGreen" type="RGBA32" />
    	<Frame name="GridBlue" type="RGBA32" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="LPVRed" id=0 />
		<Texture type="Custom" name="LPVGreen" id=1 />
		<Texture type="Custom" name="LPVBlue" id=2 />
		<Texture type="Custom" name="Occlusion" id=3 />
	</Textures>
	<Defines>
		<Define name="NB_CASCADE" type="Integer" defaultValue="2" />
	</Defines>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="LPVPropagation.frag" />
</Shader>