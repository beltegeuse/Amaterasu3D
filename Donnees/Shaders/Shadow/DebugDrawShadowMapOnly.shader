<?xml version="1.0" ?>
<Shader name="BasicShader" type="Basic">
    <Textures>
		<Texture type="Custom" name="ShadowBuffer" id=0 />
	</Textures>
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="View" name="ViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
	<VertexShader filename="DebugDrawShadowMapOnly.vert" />
	<FragmentShader filename="DebugDrawShadowMapOnly.frag" />
</Shader>