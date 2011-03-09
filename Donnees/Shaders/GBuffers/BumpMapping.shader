<?xml version="1.0" ?>
<Shader name="BumpMapping">
    <MatrixInput>
       <Matrix type="ModelView" name="ModelViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
    </MatrixInput>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
		<Texture type="Normal" name="TextureNormal" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Tangent" name="VertexTangent" />
    	<Attribut type="Normal" name="VertexNormal" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
	<VertexShader filename="BasicShader.vert" />
	<FragmentShader filename="BasicShader.frag" />
</Shader>