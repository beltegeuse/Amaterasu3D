<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="GridRed" type="RGBA32" />
    	<Frame name="GridGreen" type="RGBA32" />
    	<Frame name="GridBlue" type="RGBA32" />
    </OutputFrame>
    <Defines>
		<Define name="NB_CASCADE" type="Integer" defaultValue="2" />
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
	<GeometryShader filename="LPVInjectVPL.geom" in="Point" out="Point" />
</Shader>