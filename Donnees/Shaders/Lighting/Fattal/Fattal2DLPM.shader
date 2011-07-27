<?xml version="1.0" ?>
<Shader name="VolumeRendering" type="Basic">
	<OutputFrame depthType="None" size="Custom" width=800 height=600>
		<Frame name="outDeltaIBuffer" type="RGBA32" />
		<Frame name="outDeltaUBuffer" type="RGBA32" />
	</OutputFrame>
	<Attributs>
    	<Attribut type="Custom" id=0 name="OriPosition" />
    	<Attribut type="Custom" id=1 name="OriDirection" />
    	<Attribut type="Custom" id=2 name="OriValue" />
    </Attributs>
	<VertexShader filename="Fattal2DLPM.vert" />
	<FragmentShader filename="Fattal2DLPM.frag" />
</Shader>