<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=256 height=128>
		<Frame name="outUBuffer" type="outDeltaIBuffer" />
		<Frame name="outUBuffer" type="outDeltaUBuffer" />
	</OutputFrame>
	<Attributs>
    	<Attribut type="Custom" id=0 name="OriPosition" />
    	<Attribut type="Custom" id=1 name="OriDirection" />
    </Attributs>
	<Textures>
		<Texture type="Custom" name="UBuffer" id=0 />
		<Texture type="Custom" name="IBuffer" id=1 />
	</Textures>
	<VertexShader filename="Fattal2DLPM.vert" />
	<FragmentShader filename="Fattal2DLPM.frag" />
	<GeometryShader filename="Fattal2DLPM.geom" in="Point" out="Point" />
</Shader>