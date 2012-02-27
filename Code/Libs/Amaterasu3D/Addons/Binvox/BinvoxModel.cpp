/*
 * BinvoxModel.cpp
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#include "BinvoxModel.h"

#include <GL/glew.h>
#include "glm/glm.hpp"
#ifdef MSVC
#include <windows.h>
#endif

#include <GL/gl.h>
#include <Logger/Logger.h>
#include <Debug/Exceptions.h>
#include <System/MediaManager.h>
#include <Utilities/Util.h>
#include <Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.h>
#include <Graphics/SceneManager.h>


#include <vector>
namespace ama3D
{
BinvoxModel::BinvoxModel(const std::string& file) :
		VolumetricRenderableObject("VolumeRendering.shader"),
		m_Depth(0), m_Height(0), m_Width(0), m_Tx(0), m_Ty(0), m_Tz(0), m_Scale(1), m_Data(0)
{
	LoadFile(CMediaManager::Instance().FindMedia(file).Fullname());
}

BinvoxModel::~BinvoxModel()
{
}

void BinvoxModel::LoadHeader(std::ifstream *input)
{
	std::string line;

	///////////////////////////
	///// Check balise
	///////////////////////////
	*input >> line; // #binvox
	if (line.compare("#binvox") != 0)
	{
		Logger::Log() << "[Error] : first line reads [" << line
				<< "] instead of [#binvox]\n";
		throw new CException("Header unknow format");
	}
	///////////////////////////
	///// Get version
	///////////////////////////
	int version;
	*input >> version;
	Logger::Log() << "[INFO] [Binvox] reading binvox version " << version << "\n";

	bool done = false;
	while (input->good() && !done)
	{
		*input >> line;
		if (line.compare("data") == 0)
			done = true; /// Stop before data
		else if (line.compare("dim") == 0)
		{
			*input >> m_Depth >> m_Height >> m_Width;
		}
		else if (line.compare("translate") == 0)
		{
			*input >> m_Tx >> m_Ty >> m_Tz;
		}
		else if (line.compare("scale") == 0)
		{
			*input >> m_Scale;
		}
		else
		{
			Logger::Log() << "[Warning] unrecognized keyword [" << line
					<< "], skipping\n";
			char c;
			do
			{ // skip until end of line
				c = input->get();
			} while (input->good() && (c != '\n'));

		}
	}
	if (!done)
	{
		throw new CException("Error reading header");
	}
	if (m_Depth == 0)
	{
		throw new CException("Missing dimensions in header");
	}

	m_Data = new unsigned char[m_Depth * m_Width * m_Height];if
(	!m_Data)
	{
		throw new CException("Allocation error");
	}

	// Fill the array
	for (int i = 0; i < m_Depth * m_Width * m_Height; i++)
		m_Data[i] = 0;
}

void BinvoxModel::LoadVoxels(std::ifstream *input)
{
	unsigned char value;
	unsigned char count;
	int index = 0;
	int end_index = 0;
	int nr_voxels = 0;

	input->unsetf(std::ios::skipws); // need to read every byte now (!)
	*input >> value; // read the linefeed char
	const int size = m_Depth * m_Width * m_Height;

	while ((end_index < size) && input->good())
	{
		*input >> value >> count;

		if (input->good())
		{
			end_index = index + count;
			if (end_index > size)
				return;

			for (int i = index; i < end_index; i++)
				m_Data[i] = value;

			if (value)
				nr_voxels += count;

			index = end_index;
		} // if file still ok

	} // while

	Logger::Log() << "[INFO] [Binvox] Read " << nr_voxels << " voxels\n";
}

void BinvoxModel::Bind()
{
	VolumetricRenderableObject::Bind();
	//m_volumeRenderingShader->SetUniform1i("GridInterpolation", true);
}

glm::ivec2 BinvoxModel::TextureRepeat()
{
	glm::ivec2 repeat;
	int Taille = sqrt((float)m_Depth);
	repeat.x = NearestPowerOfTwo(Taille);
	repeat.y = m_Depth / repeat.x;
	return repeat;
}

glm::ivec2 BinvoxModel::TextureSize()
{
	glm::ivec2 size;
	glm::ivec2 repeat = TextureRepeat();
	size.x = m_Width * repeat.x;
	size.y = m_Height * repeat.y;
	return size;
}

TTexturePtr BinvoxModel::Create2DTexture()
{
	/// Generate the Array
	float* image = new float[m_Width * m_Height * m_Depth];
	glm::ivec2 texSize =TextureSize();
	glm::ivec2 repeat = TextureRepeat();

	for(int rX = 0; rX < repeat.x; rX++)
		for(int rY = 0; rY < repeat.y; rY++)
			for(int i = 0; i < m_Width; i++)
				for(int j = 0; j < m_Height; j++)
				{
					int x = rX*m_Depth+i;
					int y = rY*m_Depth+j;
					image[y*texSize.x + x] = GetData(i,j,rX+rY*repeat.x);
				}

	/// Generate the OpenGL texture
	Texture* tex = new Texture(true);
	Texture2DParams param;
	glBindTexture(GL_TEXTURE_2D,tex->getIdTex());
	param.applyParam();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, texSize.x, texSize.y, 0, GL_ALPHA, GL_FLOAT, image);
	glBindTexture(GL_TEXTURE_2D,0);

	delete[] image;

	return tex;
}

 glm::vec3 BinvoxModel::GridSize() const
{
	return  glm::vec3(m_Width, m_Height, m_Depth);
}

void BinvoxModel::LoadFile(const std::string& file)
{
	std::ifstream *input = new std::ifstream(file.c_str(),
			std::ios::in | std::ios::binary);

	LoadHeader(input);
	LoadVoxels(input);

	input->close();
	delete input;

	// Update data
	Initialise(Create2DTexture(), TextureSize(), TextureRepeat(), GridSize());
}
}
