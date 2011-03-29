<?xml version="1.0" ?>
<Shader name="LPVShowVPL" type="Basic">
    <MatrixInput>
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <Textures>
		<Texture type="Custom" name="Grid" id=0 />
	</Textures>
	<VertexShader filename="LPVShowVPL.vert" />
	<FragmentShader filename="LPVShowVPL.frag" />
</Shader>