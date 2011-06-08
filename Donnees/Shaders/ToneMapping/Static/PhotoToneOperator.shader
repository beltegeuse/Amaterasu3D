<?xml version="1.0" ?>
<Shader name="FilmicToneOperator" type="Basic">
	<OutputFrame depthType="None" size="Screen">
    	<Frame name="Result" type="RGBA" />
    </OutputFrame>
	<Textures>
		<Texture type="Custom" name="HDRBuffer" id=0 />
		<Texture type="Custom" name="AdaptationBuffer" id=1 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="PhotoToneOperator.frag" />
</Shader>