<?xml version="1.0" ?>
<Shader name="CubeProjection" type="Basic">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <OutputFrame depthType="RenderTarget" size="Custom" width=512 height=512>
    	<Frame name="ColorBuffer" type="RGBA" />
    </OutputFrame>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    </Attributs>
	<VertexShader filename="CubeProjection.vert" />
	<FragmentShader filename="CubeProjection.frag" />
</Shader>