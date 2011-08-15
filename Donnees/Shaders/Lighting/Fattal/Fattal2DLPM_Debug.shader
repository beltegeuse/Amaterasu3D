<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=800 height=600>
	 	<Frame name="outDeltaBuffer" type="RGBA32" />
	</OutputFrame>
	<Defines>
		<Define name="DEBUG_MODE" type="Bool" defaultValue="true" />
	</Defines>
	<Attributs>
    	<Attribut type="Custom" id=0 name="OriPosition" />
    	<Attribut type="Custom" id=1 name="OriDirection" />
    	<Attribut type="Custom" id=2 name="OriValue" />
    </Attributs>
	<Textures>
		<Texture type="Custom" name="UBuffer" id=0 />
	</Textures>
	<VertexShader filename="Fattal2DLPM.vert" />
	<FragmentShader filename="Fattal2DLPM.frag" />
	<GeometryShader filename="Fattal2DLPM.geom" in="Point" out="LineStrip" />
</Shader>