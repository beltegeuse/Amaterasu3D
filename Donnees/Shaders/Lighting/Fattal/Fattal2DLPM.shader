<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=800 height=600>
	 	<Frame name="outDeltaIBuffer" type="RGBA32" />
		<!-- <Frame name="outDeltaUBuffer" type="RGBA32" /> -->
	</OutputFrame>
	<Attributs>
    	<Attribut type="Custom" id=0 name="OriPosition" />
    	<Attribut type="Custom" id=1 name="OriDirection" />
    	<Attribut type="Custom" id=2 name="OriValue" />
    </Attributs>
	<Textures>
		<Texture type="Custom" name="UBuffer" id=0 />
		<Texture type="Custom" name="IBuffer" id=1 />
	</Textures>
	<VertexShader filename="Fattal2DLPM.vert" />
	<FragmentShader filename="Fattal2DLPM.frag" />
	<GeometryShader filename="Fattal2DDebugLPM.geom" in="Point" out="LineStrip" />
</Shader>