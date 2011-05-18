<?xml version="1.0" ?>
<Shader name="ParaboloidProjection" type="Basic">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
	<VertexShader filename="ParaboloidProjection.vert" />
	<FragmentShader filename="ParaboloidProjection.frag" />
</Shader>