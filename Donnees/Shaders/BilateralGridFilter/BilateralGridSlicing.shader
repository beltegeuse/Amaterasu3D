<?xml version="1.0" ?>
<Shader name="FilmicToneOperator" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=256 height=128>
    	<Frame name="Result" type="RGBA32" />
    </OutputFrame>
	<Textures>
		<Texture type="Custom" name="GridBuffer" id=0 />
		<Texture type="Custom" name="InputBuffer" id=1 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="BilateralGridSlicing.frag" />
</Shader>