<?xml version="1.0" ?>
<Shader name="BasicShader" type="Basic">
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="View" name="ViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Color" name="VertexColor" />
    </Attributs>
	<VertexShader filename="BasicShader.vert" />
	<FragmentShader filename="BasicShader.frag" />
</Shader>