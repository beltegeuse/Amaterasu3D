<?xml version="1.0" ?>
<Shader name="LPVShowVPL" type="Basic">
    <MatrixInput>
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
	<Textures>
		<Texture type="Custom" name="DepthBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
		<Texture type="Custom" name="FluxBuffer" id=3 />
	</Textures>
	<VertexShader filename="LPVShowVPL.vert" />
	<FragmentShader filename="LPVShowVPL.frag" />
</Shader>