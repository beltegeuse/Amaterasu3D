<?xml version="1.0" ?>
<Shader name="2DDraw" type="Basic">
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
	</Textures>
	<VertexShader filename="2DDraw.vert" />
	<FragmentShader filename="2DDraw.frag" />
</Shader>