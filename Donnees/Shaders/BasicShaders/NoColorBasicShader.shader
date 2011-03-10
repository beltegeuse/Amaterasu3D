<?xml version="1.0" ?>
<Shader name="NoColorBasicShader" type="Basic">
    <MatrixInput>
       <Matrix type="ModelView" name="ModelViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    </Attributs>
	<VertexShader filename="NoColorBasicShader.vert" />
	<FragmentShader filename="NoColorBasicShader.frag" />
</Shader>