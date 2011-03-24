<?xml version="1.0" ?>
<Shader name="GaussianBlurH" type="Basic">
	<OutputFrame depthType="None" size="Screen">
		<Frame name="Result" type="RGBA" />
	</OutputFrame>
	<Textures>
		<Texture type="Custom" name="TextureBuffer" id=0 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="GaussianBlurV.frag" />
</Shader>