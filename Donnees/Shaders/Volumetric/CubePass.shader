<?xml version="1.0" ?>
<Shader name="BasicShader" type="Basic">
	<OutputFrame depthType="None" size="Screen">
    	<Frame name="Color" type="RGBA" />
    </OutputFrame>
    <MatrixInput>
       <Matrix type="Model" name="ModelMatrix" />
       <Matrix type="View" name="ViewMatrix" />
       <Matrix type="Projection" name="ProjectionMatrix" />
    </MatrixInput>
	<Attributs>
    	<Attribut type="Vertex" name="VertexPosition" />
    	<Attribut type="Color" name="VertexColor" />
    </Attributs>
	<VertexShader filename="CubePass.vert" />
	<FragmentShader filename="CubePass.frag" />
</Shader>