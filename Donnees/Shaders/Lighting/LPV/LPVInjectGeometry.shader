<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="Grid" type="RGBA32" />
    </OutputFrame>
    <Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
    <Textures>
		<Texture type="Custom" name="NormalBuffer" id=1 />
		<Texture type="Custom" name="DepthBuffer" id=2 />
	</Textures>
	<VertexShader filename="LPVInjectGeometry.vert" />
	<FragmentShader filename="LPVInjectGeometry.frag" />
</Shader>