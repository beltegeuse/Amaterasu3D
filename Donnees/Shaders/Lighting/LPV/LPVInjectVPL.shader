<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="Grid" type="RGBA32" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="FluxBuffer" id=0 />
		<Texture type="Custom" name="PositionBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
	</Textures>
	<VertexShader filename="LPVInjectVPL.vert" />
	<FragmentShader filename="LPVInjectVPL.frag" />
</Shader>