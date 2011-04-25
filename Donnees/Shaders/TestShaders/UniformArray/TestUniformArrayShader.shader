<?xml version="1.0" ?>
<Shader name="TestFBOTextureShader" type="Basic">
    <GeometryShader filename="TestUniformArrayShader.geom" in="Line" out="LineStrip" />
	<VertexShader filename="TestUniformArrayShader.vert" />
	<FragmentShader filename="TestUniformArrayShader.frag" />
	<Defines>
		<Define name="NB" defaultValue="10" />
	</Defines>
</Shader>