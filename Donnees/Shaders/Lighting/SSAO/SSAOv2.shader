<?xml version="1.0" ?>
<Shader name="SSAO" type="Basic">
	<OutputFrame depthType="None" size="Screen">
		<Frame name="AmbiantOcculsion" type="RGBA" />
	</OutputFrame>
	<Textures>
		<Texture type="Custom" name="DepthBuffer" id=0 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="SSAOv2.frag" />
</Shader>