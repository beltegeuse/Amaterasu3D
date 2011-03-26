<?xml version="1.0" ?>
<Shader name="ShadowMap" type="Basic">
    <OutputFrame depthType="Texture" size="Custom" height=512 width=512>
    </OutputFrame>
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="View" name="ViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
	<VertexShader filename="ShadowMap.vert" />
	<FragmentShader filename="ShadowMap.frag" />
</Shader>