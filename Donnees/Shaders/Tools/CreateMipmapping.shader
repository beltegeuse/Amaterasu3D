<?xml version="1.0" ?>
<Shader name="SphereProjectionHemiCube" type="Basic">
    <OutputFrame depthType="None" size="Screen">
    	<Frame name="ColorBuffer" type="RGBA" mipmapping="True" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="Buffer" />
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="CreateMipmapping.frag" />
</Shader>