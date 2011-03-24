<?xml version="1.0" ?>
<Shader name="RefectiveShadowMapSpot" type="GBuffer">
	<MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
       <Matrix type="Normal" name="NormalMatrix" />
       <Matrix type="View" name="ViewMatrix" />
    </MatrixInput>
    <OutputFrame depthType="Texture" size="Custom" height=512 width=512>
    	<Frame name="Flux" type="RGBA" />
    	<Frame name="Normal" type="RGBA32" />
    	<Frame name="Position" type="RGBA32" />
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
	<FragmentShader filename="RefectiveShadowMapSpot.frag" />
</Shader>