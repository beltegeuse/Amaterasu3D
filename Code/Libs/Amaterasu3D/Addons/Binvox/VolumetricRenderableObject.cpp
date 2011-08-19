#include "VolumetricRenderableObject.h"

#include <Graphics/SceneManager.h>

namespace ama3D
{

VolumetricRenderableObject::VolumetricRenderableObject(const std::string& shaderfile) :
	m_Initialized(false),
	m_CubeModel(0)
{
	// Load shaders
	m_CubeShader = CShaderManager::Instance().LoadShader("CubePass.shader");
	m_volumeRenderingShader = CShaderManager::Instance().LoadShader(shaderfile);
	// Create FBO
	m_BackFBO = m_CubeShader->GetFBO()->Copy();
	m_FrontFBO = m_CubeShader->GetFBO();
}

VolumetricRenderableObject::~VolumetricRenderableObject()
{
}

void VolumetricRenderableObject::UpdateCubeFBO(CameraAbstract* cam)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_CubeShader->SetFBO(m_FrontFBO, false);
	m_CubeShader->Begin();
	cam->GetView();
	m_CubeModel->Render();
	m_CubeShader->End();

	glCullFace(GL_FRONT);
	m_CubeShader->SetFBO(m_BackFBO, false);
	m_CubeShader->Begin();
	cam->GetView();
	m_CubeModel->Render();
	m_CubeShader->End();
	glDisable(GL_CULL_FACE);
}

void VolumetricRenderableObject::Bind()
{
	m_volumeRenderingShader->Begin();
	m_FrontFBO->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+0);
	m_BackFBO->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+1);
	m_VolumeTexture->activateMultiTex(CUSTOM_TEXTURE+2);
	m_volumeRenderingShader->SetUniformVector("GridDimension", m_GridSize);
	//Math::TVector4F(sizeTex.x, sizeTex.y, repeatTex.x, repeatTex.y)
	m_volumeRenderingShader->SetUniformVector("GridTextureSize", m_TextureSpec);
	ShaderHelperUniformImagePlane(m_volumeRenderingShader);
}

void VolumetricRenderableObject::Unbind()
{
	m_volumeRenderingShader->End();
}

void VolumetricRenderableObject::Render()
{
	Assert(m_Initialized);
	Bind();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
	glEnd();
	Unbind();
}

void VolumetricRenderableObject::Initialise(TTexturePtr tex, const glm::ivec2& texSize,
			const glm::ivec2& texRepet, const glm::vec3& gridSize)
{
	m_Initialized = true;
	m_VolumeTexture = tex;
	m_TextureSpec = glm::vec4(texSize.x, texSize.y, texRepet.x, texRepet.y);
	m_GridSize = gridSize;

	// Update cube model
	if(m_CubeModel)
	{
		delete m_CubeModel;
	}
	m_CubeModel = CSceneManager::Instance().CreateSimpleMesh(MESH_CUBE, "cube1", 0);
	// Need translation because [-1,1]
	m_CubeModel->SetScale(m_GridSize);
	m_CubeModel->UpdateTransformations();
}


} // Namespace ama3D
