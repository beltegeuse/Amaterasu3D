#include "Skydome.h"

Skydome::Skydome() :
m_Time(0.0)
{
	// TODO Auto-generated constructor stub
	m_Obj=gluNewQuadric();
	m_TextureCloud = Texture::LoadFromFile("cloud.bmp");
	m_SkyShader = CShaderManager::Instance().LoadShader("Sky.shader");
}

Skydome::~Skydome() {
	// TODO Auto-generated destructor stub
}

void Skydome::FrameStarted(double delta)
{
	m_Time += delta;
}

void Skydome::Draw()
{
	static Math::TVector3F eye(0.0f, 0.0f, 0.0f);
	static Math::TVector3F look(0.0f, 0.0f, 1.0f);
	static Math::TVector3F up(0.0f, 1.0f, 0.0f);

	Math::TVector3F trans = CMatrixManager::Instance().GetMatrix(VIEW_MATRIX).GetTranslation();
	Math::CMatrix4 transMatrix;
	transMatrix.SetTranslation(-trans.x, -trans.y, -trans.z);
	CMatrixManager::Instance().PushMatrix(transMatrix);
	m_TextureCloud->activateMultiTex(CUSTOM_TEXTURE+0);
	m_SkyShader->Begin();
	m_SkyShader->SetUniform1f("time",  m_Time / 10.0);
	m_SkyShader->SetUniformVector("horizon",  Math::TVector4F(0.9f, 0.7f, 0.7f, 1.0f));
	glDisable(GL_DEPTH_TEST);
	gluSphere(m_Obj,2.0,300,300);
	glEnable(GL_DEPTH_TEST);
	m_SkyShader->End();
	m_TextureCloud->desactivateMultiTex(CUSTOM_TEXTURE+0);
	CMatrixManager::Instance().PopMatrix();
}
