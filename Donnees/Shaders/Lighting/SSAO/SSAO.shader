<?xml version="1.0" ?>
<Shader name="SSAO" type="Basic">
	<OutputFrame depthType="None" size="Screen">
		<Frame name="AmbiantOcculsion" type="RGBA" />
	</OutputFrame>
	<Textures>
		<Texture type="Custom" name="NormalBuffer" id=0 />
		<Texture type="Custom" name="DepthBuffer" id=1 />
		<Texture type="Custom" name="RandomBuffer" id=2 />
		<Texture type="Custom" name="RendererBuffer" id=3 />
		<Texture type="Custom" name="PositionBuffer" id=4 />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="SSAO.frag" />
</Shader>