<?xml version="1.0" ?>
<Shader name="LPVShowVPL" type="Basic">
    <MatrixInput>
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
	<Textures>
		<Texture type="Custom" name="PositionBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
	</Textures>
	<VertexShader filename="LPVShowVPL.vert" />
	<FragmentShader filename="LPVShowVPL.frag" />
</Shader>