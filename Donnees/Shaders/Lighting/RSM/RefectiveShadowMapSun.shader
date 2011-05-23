<?xml version="1.0" ?>
<Shader name="RefectiveShadowMapSpot" type="GBuffer">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <Materials>
    	<Material type="Diffuse" name="MaterialDiffuseColor" />
    </Materials>
    <OutputFrame depthType="Texture" size="Custom" height=2048 width=2048>
    	<Frame name="Flux" type="RGBA32" />
    	<Frame name="Normal" type="RGBA32" />
    </OutputFrame>
    <Textures>
		<Texture type="Diffuse" name="TextureDiffuse" />
		<Texture type="Normal" name="TextureNormal" />
	</Textures>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Tangent" name="VertexTangent" />
    	<Attribut type="BiTangent" name="VertexBiTangent" />
    	<Attribut type="Normal" name="VertexNormal" />
    	<Attribut type="TexCoord" name="VertexTexCoord" />
    	<Attribut type="Color" name="VertexColor" />
    </Attributs>
	<VertexShader filename="RefectiveShadowMapSpot.vert" />
	<FragmentShader filename="RefectiveShadowMapSun.frag" />
</Shader>