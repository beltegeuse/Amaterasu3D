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
		<Texture type="Custom" name="FontTexture" id=0 />
	</Textures>
	<VertexShader filename="2DDraw.vert" />
	<FragmentShader filename="2DDrawFont.frag" />
</Shader>