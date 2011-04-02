<?xml version="1.0" ?>
<Shader name="BumpMapping" type="GBuffer">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <OutputFrame depthType="Texture" size="Screen">
    	<Frame name="Diffuse" type="RGBA" />
    	<Frame name="Normal" type="RGBA32" />
    	<Frame name="Specular" type="RGBA" />
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
    <Materials>
    	<Material type="Diffuse" name="MaterialDiffuseColor" />
    	<Material type="Specular" name="MaterialSpecularColor" />
    </Materials>
	<VertexShader filename="GBuffer.vert" />
	<FragmentShader filename="GBuffer.frag" />
</Shader>