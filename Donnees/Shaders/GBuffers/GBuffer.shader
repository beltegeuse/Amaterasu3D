<?xml version="1.0" ?>
<Shader name="BumpMapping" type="GBuffer">
    <MatrixInput>
       <Matrix type="ModelView" name="ModelViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
    </MatrixInput>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
		<Texture type="Normal" name="TextureNormal" />
		<Texture type="Specular" name="TextureSpecular" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Tangent" name="VertexTangent" />
    	<Attribut type="Normal" name="VertexNormal" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    	<Attribut type="Color" name="VertexColor" />
    </Attributs>
	<VertexShader filename="GBuffer.vert" />
	<FragmentShader filename="GBuffer.frag" />
</Shader>