<?xml version="1.0" ?>
<Shader name="ParaboloidProjection" type="Basic">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <OutputFrame depthType="Texture" size="Screen">
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