#include "Logo.h"

#include <System/SettingsManager.h>

Logo::Logo() :
m_Rectangle(0)
{
}

Logo::~Logo()
{
}

void Logo::Draw()
{
	// On initialise au premier appel
	if(m_Rectangle == 0)
	{
		Math::TVector2I screenSize = Math::TVector2I(CSettingsManager::Instance().GetSizeRenderingWindow());
		m_Rectangle = new Rectangle2D(screenSize - Math::TVector2I(240,160), screenSize - Math::TVector2I(10,10), true);
		m_Rectangle->GetObject().AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("logo.png"));
		m_2DShader = CShaderManager::Instance().LoadShader("2DDraw.shader");
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
	m_2DShader->Begin();
	m_Rectangle->Render();
	m_2DShader->End();
	glDisable(GL_BLEND);
}
