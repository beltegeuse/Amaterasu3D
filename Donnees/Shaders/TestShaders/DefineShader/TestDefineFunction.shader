<?xml version="1.0" ?>
<Shader name="TestFBOTextureShader" type="Basic">
	<Defines>
		<Define name="FACTOR" defaultValue="0.05" />
	</Defines>

    <GeometryShader filename="TestDefineFunction.geom" in="Line" out="LineStrip" />
	<VertexShader filename="TestDefineFunction.vert" />
	<FragmentShader filename="TestDefineFunction.frag" />
</Shader>