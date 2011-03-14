<?xml version="1.0" ?>
<Shader name="BasicTextureShader" type="Basic">
    <MatrixInput>
       <Matrix type="ModelView" name="ModelViewMatrix" />
    </MatrixInput>
	<Textures>
		<Texture type="Custom" name="DiffuseBuffer" id=0 />
		<Texture type="Custom" name="SpecularBuffer" id=1 />
		<Texture type="Custom" name="NormalBuffer" id=2 />
		<Texture type="Custom" name="PositionBuffer" id=3 />
	</Textures>
	<VertexShader filename="DeferredPointLight.vert" />
	<FragmentShader filename="DeferredPointLight.frag" />
</Shader>