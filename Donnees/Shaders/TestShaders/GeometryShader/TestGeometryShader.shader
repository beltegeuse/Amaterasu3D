<?xml version="1.0" ?>
<Shader name="TestFBOTextureShader" type="Basic">
    <OutputFrame depthType="Texture">
    	<Frame name="Color" type="RGBA" />
    </OutputFrame>
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
    <GeometryShader filename="TestGeometryShader.geom" in="Lines" out="StripLines" />
	<VertexShader filename="TestGeometryShader.vert" />
	<FragmentShader filename="TestGeometryShader.frag" />
</Shader>