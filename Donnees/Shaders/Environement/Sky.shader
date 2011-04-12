<?xml version="1.0" ?>
<Shader name="BasicTextureShader" type="Basic">
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
	<Textures>
		<Texture type="Custom" name="CloudBuffer" id=0 />
	</Textures>
	<VertexShader filename="Sky.vert" />
	<FragmentShader filename="Sky.frag" />
</Shader>