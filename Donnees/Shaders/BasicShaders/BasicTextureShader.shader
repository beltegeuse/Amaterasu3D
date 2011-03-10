<?xml version="1.0" ?>
<Shader name="BasicTextureShader" type="Basic">
    <MatrixInput>
       <Matrix type="ModelView" name="ModelViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Textures>
		<Texture type="Diffuse" name="Texture0" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
	<VertexShader filename="BasicTextureShader.vert" />
	<FragmentShader filename="BasicTextureShader.frag" />
</Shader>