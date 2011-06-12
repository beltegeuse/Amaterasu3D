<?xml version="1.0" ?>
<Shader name="FilmicToneOperator" type="Basic">
	<OutputFrame depthType="None" size="Screen">
    	<Frame name="Result" type="RGBA32" />
    </OutputFrame>
	<Textures>
		<Texture type="Custom" name="InputBuffer" id=0 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="BilateralFiltering.frag" />
</Shader>