#include "Logo.h"

Logo::Logo()
{
	m_Logo = Texture::LoadFromFile("logo.png");
}

Logo::~Logo()
{
}

void Logo::Draw()
{
	m_Logo->activateTextureMapping();
	m_Logo->activateTexture();


	m_Logo->desactivateTextureMapping();
}
