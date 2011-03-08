<?xml version="1.0" ?>
<Shader name="BasicTextureShader">
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