<?xml version="1.0" ?>
<Shader name="FilmicToneOperator" type="Basic">
	<OutputFrame depthType="None" size="Screen">
    	<Frame name="Result" type="L" mipmapping="True" />
    </OutputFrame>
	<Textures>
		<Texture type="Custom" name="HDRBuffer" id=0 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="ComputeLuminance.frag" />
</Shader>