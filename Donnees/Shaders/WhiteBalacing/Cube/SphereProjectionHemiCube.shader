<?xml version="1.0" ?>
<Shader name="SphereProjectionHemiCube" type="Basic">
    <OutputFrame depthType="None" size="Custom" width=512 height=512>
    	<Frame name="ColorBuffer" type="RGBA" mipmapping="True" />
    </OutputFrame>
    <Textures>
		<Texture type="Custom" name="ZTex" id=0/>
		<Texture type="Custom" name="YPlusTex" id=1 />
		<Texture type="Custom" name="YMoinsTex" id=2 />
		<Texture type="Custom" name="XPlusTex" id=3/>
		<Texture type="Custom" name="XMoinsTex" id=4/>
	</Textures>
	<VertexShader filename="SimpleQuadDisplay.vert" />
	<FragmentShader filename="SphereProjectionHemiCube.frag" />
</Shader>