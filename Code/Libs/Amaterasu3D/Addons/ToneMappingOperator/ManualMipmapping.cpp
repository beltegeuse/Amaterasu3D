/*
 * ManualMipmapping.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: adrien
 */

#include "ManualMipmapping.h"

#include <Logger/Logger.h>
#include <Utilities/Util.h>

ManualMipmapping::ManualMipmapping(int initialeSize) :
	m_InitialSize(initialeSize)
{
	// Init object parameters
	m_InitialSize = NearestPowerOfTwo(m_InitialSize);
	Logger::Log() << "[INFO] Creation of Manual Mip-mapping : " << m_InitialSize << "\n";
	m_NbLevels = log(m_InitialSize)/log(2);
	Logger::Log() << "[INFO] Number levels : " << m_NbLevels;
}

ManualMipmapping::~ManualMipmapping()
{
}

void ManualMipmapping::Initialize()
{
	Logger::Log() << "[INFO] Manual mipmapping : Initialize ... \n";
	// Load Shaders
	m_ManualMipmappingShader = CShaderManager::Instance().LoadShader("ManualMipmappingPass.shader");
	m_DownSamplerShader = CShaderManager::Instance().LoadShader("ManualDownsamplingPass.shader");

	// Configure FBO
	FBOTextureBufferParam param;
	param.GenerateMipMapping = false;
	param.MinFiltering = GL_NEAREST;
	param.MaxFiltering = GL_NEAREST;
	param.InternalFormat = GL_RGBA32F;
	param.ExternalFormat = GL_RGBA;

	m_DownSamplerShader->Begin();
	param.Attachment = glGetFragDataLocation(m_DownSamplerShader->GetProgramObject(),"BufferColor");
	Logger::Log() << "           * Attachment : " << param.Attachment << "\n";
	m_DownSamplerShader->End();

	m_DownSamplerShader->SetFBO(CreateFBO(param, "BufferColor", m_InitialSize), true);

	m_ManualMipmappingShader->Begin();
	param.Attachment = glGetFragDataLocation(m_ManualMipmappingShader->GetProgramObject(),"BufferColor");
	Logger::Log() << "           * Attachment : " << param.Attachment << "\n";
	m_ManualMipmappingShader->End();

	// Create FBOs
	m_Mipmaps = new FBO*[m_NbLevels];
	for(int i = 0; i < 1; i++)
	{
		m_Mipmaps[i] = CreateFBO(param, "BufferColor", m_InitialSize/pow(2,i+1));
	}
}

FBO* ManualMipmapping::CreateFBO(FBOTextureBufferParam p, const std::string& buffername, int size)
{
	std::map<std::string, FBOTextureBufferParam> buffers;
	buffers[buffername] = p;
	FBODepthBufferParam bufferDepth;
	return new FBO(Math::TVector2I(size, size), buffers, FBODEPTH_NONE, bufferDepth);
}

int ManualMipmapping::NumberLevels()
{
	return m_NbLevels+1;
}

Texture* ManualMipmapping::GetLevel(int l)
{
	if(l==0)
		return m_DownSamplerShader->GetFBO()->GetTexture("ColorBuffer");
	return m_Mipmaps[l-1]->GetTexture("ColorBuffer");
}

void ManualMipmapping::Compute(Texture* texture)
{
	m_DownSamplerShader->Begin();
	texture->activateMultiTex(CUSTOM_TEXTURE+0);
	DrawQuad();
	texture->desactivateMultiTex(CUSTOM_TEXTURE+0);
	m_DownSamplerShader->End();

	for(int i =0; i < m_NbLevels; i++)
	{
		m_ManualMipmappingShader->SetFBO(m_Mipmaps[i], false);
		m_ManualMipmappingShader->Begin();

		if(i == 0)
			m_DownSamplerShader->GetFBO()->GetTexture("ColorBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
		else
			m_Mipmaps[i-1]->GetTexture("ColorBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);

		int size = m_InitialSize/pow(2,i);
		m_ManualMipmappingShader->SetUniformVector("TexSize", Math::TVector2F(size,size));
		DrawQuad();

		if(i == 0)
			m_DownSamplerShader->GetFBO()->GetTexture("ColorBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		else
			m_Mipmaps[i-1]->GetTexture("ColorBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_ManualMipmappingShader->End();
	}

}

void ManualMipmapping::DrawQuad()
{
	// Draw ...
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
}
