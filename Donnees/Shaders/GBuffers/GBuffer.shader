<?xml version="1.0" ?>
<Shader name="BumpMapping" type="GBuffer">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <OutputFrame depthType="Texture">
    	<Frame name="Diffuse" type="RGBA" />
    	<Frame name="Normal" type="RGBA32" />
    	<Frame name="Specular" type="RGBA" />
    	<Frame name="Position" type="RGBA32" />
    </OutputFrame>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
		<Texture type="Normal" name="TextureNormal" />
		<Texture type="Specular" name="TextureSpecular" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Tangent" name="VertexTangent" />
    	<Attribut type="BiTangent" name="VertexBiTangent" />
    	<Attribut type="Normal" name="VertexNormal" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    	<Attribut type="Color" name="VertexColor" />
    </Attributs>
	<VertexShader filename="GBuffer.vert" />
	<FragmentShader filename="GBuffer.frag" />
</Shader>