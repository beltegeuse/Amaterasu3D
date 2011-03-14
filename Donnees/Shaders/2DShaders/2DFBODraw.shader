<?xml version="1.0" ?>
<Shader name="2DFBODraw" type="Basic">
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
	</Textures>
	<VertexShader filename="2DFBODraw.vert" />
	<FragmentShader filename="2DFBODraw.frag" />
</Shader>