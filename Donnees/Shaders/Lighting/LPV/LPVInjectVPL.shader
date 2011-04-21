<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="GridRed" type="RGBA16" />
    	<Frame name="GridGreen" type="RGBA16" />
    	<Frame name="GridBlue" type="RGBA16" />
    </OutputFrame>
    <Defines>
		<Define name="NB_CASCADE" defaultValue="3" />
	</Defines>
    <Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
    <Textures>
		<Texture type="Custom" name="FluxBuffer" id=0 />
		<Texture type="Custom" name="DepthBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
	</Textures>
	<VertexShader filename="LPVInjectVPL.vert" />
	<FragmentShader filename="LPVInjectVPL.frag" />
</Shader>