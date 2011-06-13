<?xml version="1.0" ?>
<Shader name="ManualDownsamplingPass" type="Basic">
    <OutputFrame depthType="None" size="Screen">
    	<Frame name="ColorBuffer" type="RGBA" mipmapping="True" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="Buffer" />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="RenderTextureToRenderTarget.frag" />
</Shader>