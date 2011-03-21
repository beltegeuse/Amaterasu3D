<?xml version="1.0" ?>
<Shader name="2DFBODrawDepth" type="Basic">
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="2DFBODrawDepth.frag" />
</Shader>