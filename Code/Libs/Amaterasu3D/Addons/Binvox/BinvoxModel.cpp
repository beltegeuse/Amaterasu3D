/*
 * BinvoxModel.cpp
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#include "BinvoxModel.h"

#include <Logger/Logger.h>
#include <Debug/Exceptions.h>
#include <System/MediaManager.h>
#include <Math/Vector3.h>
#include <Utilities/Util.h>

#include <vector>

BinvoxModel::BinvoxModel(const std::string& file) :
m_Depth(0),m_Height(0),m_Width(0),
m_Tx(0), m_Ty(0), m_Tz(0), m_Scale(1),
m_Data(0)
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
	*input >> line;  // #binvox
	if (line.compare("#binvox") != 0)
	{
		Logger::Log() << "[Error] : first line reads [" << line << "] instead of [#binvox]\n";
		throw new CException("Header unknow format");
	}
	///////////////////////////
	///// Get version
	///////////////////////////
	int version;
	*input >> version;
	Logger::Log() << "[INFO] reading binvox version " << version << "\n";

	bool done = false;
	while(input->good() && !done)
	{
		*input >> line;
		if (line.compare("data") == 0) done = true; /// Stop before data
		else if (line.compare("dim") == 0)
		{
			*input >> m_Depth >> m_Height >> m_Width;
		}
		else if (line.compare("translate") == 0)
		{
			*input >> m_Tx >> m_Ty >> m_Tz;
		}
		else if (line.compare("scale") == 0) {
			*input >> m_Scale;
		}
		else
		{
			Logger::Log() << "[Warning] unrecognized keyword [" << line << "], skipping\n";
			char c;
			do
			{  // skip until end of line
				c = input->get();
			} while(input->good() && (c != '\n'));

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

	m_Data = new unsigned char[m_Depth*m_Width*m_Height];
	if (!m_Data)
	{
		throw new CException("Allocation error");
	}

	// Fill the array
	for(int i = 0; i < m_Depth*m_Width*m_Height; i++)
		m_Data[i] = 0;
}

void BinvoxModel::LoadVoxels(std::ifstream *input)
{
	unsigned char value;
	unsigned char count;
	int index = 0;
	int end_index = 0;
	int nr_voxels = 0;

	input->unsetf(std::ios::skipws);  // need to read every byte now (!)
	*input >> value;  // read the linefeed char
	const int size = m_Depth*m_Width*m_Height;

	while((end_index < size) && input->good()) {
		*input >> value >> count;

		if (input->good())
		{
			end_index = index + count;
			if (end_index > size)
				return;

			for(int i=index; i < end_index; i++)
				m_Data[i] = value;

			if (value)
				nr_voxels += count;

			index = end_index;
		}  // if file still ok

	}  // while

	Logger::Log() << "[INFO] Read " << nr_voxels << " voxels\n";
}

SceneGraph::Model* BinvoxModel::CreateDebugPointModel()
{
	int size = m_Depth*m_Width*m_Height;

	// Extract only fill voxels
	std::vector<Math::TVector3I> voxels;
	int wxh = m_Width*m_Height;
	for(int x = 0; x < m_Width; x++)
		for(int y = 0; y < m_Height; y++)
			for(int z = 0; z < m_Depth; z++)
				if(m_Data[x * wxh + z * m_Width + y] != 0)
					voxels.push_back(Math::TVector3I(x,y,z));

	float * vertexBuffer = new float[3*voxels.size()];
	float * colorBuffer = new float[3*voxels.size()];
	unsigned int* indiceBuffer = new unsigned int[voxels.size()];

	for(int i = 0; i < voxels.size(); i++)
	{
		vertexBuffer[3*i] = voxels[i].x;
		vertexBuffer[3*i+1] = voxels[i].y;
		vertexBuffer[3*i+2] = voxels[i].z;
		colorBuffer[3*i] = 1.0f;
		colorBuffer[3*i+1] = 1.0f;
		colorBuffer[3*i+2] = 1.0f;
	}

	for(int l=0; l < voxels.size(); l++)
	{
		indiceBuffer[l] = l;
	}

	SceneGraph::Model* model = new SceneGraph::Model;
	model->SetDrawMode(GL_POINTS);
	model->SetIndiceBuffer(indiceBuffer, voxels.size());
	SceneGraph::ModelBuffer buffer;
	buffer.buffer = vertexBuffer;
	buffer.size = 3*voxels.size();
	buffer.dimension = 3;
	buffer.owner = true;
	model->AddBuffer(buffer, VERTEX_ATTRIBUT);
	buffer.buffer = colorBuffer;
	model->AddBuffer(buffer, COLOR_ATTRIBUT);
	model->CompileBuffers();
	//model->AddMaterial(DIFFUSE_MATERIAL,color);

	return model;
}

Math::TVector2I BinvoxModel::TextureRepeat()
{
	Math::TVector2I repeat;
	int Taille = sqrt(m_Depth);
	repeat.x = NearestPowerOfTwo(Taille);
	repeat.y = m_Depth/repeat.x;
	return repeat;
}

Math::TVector2I BinvoxModel::TextureSize()
{
	Math::TVector2I size;
	Math::TVector2I repeat = TextureRepeat();
	size.x = m_Width*repeat.x;
	size.y = m_Height*repeat.y;
	return size;
}

TTexturePtr BinvoxModel::Create2DTexture()
{
	// Find how make and fill texture.
}

void BinvoxModel::LoadFile(const std::string& file)
{
	std::ifstream *input = new std::ifstream(file.c_str(), std::ios::in | std::ios::binary);

	LoadHeader(input);
	LoadVoxels(input);

	input->close();
	delete input;
}