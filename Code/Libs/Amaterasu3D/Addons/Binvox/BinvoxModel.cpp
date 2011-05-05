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

void BinvoxModel::LoadFile(const std::string& file)
{
	std::ifstream *input = new std::ifstream(file.c_str(), std::ios::in | std::ios::binary);

	LoadHeader(input);
	LoadVoxels(input);

	input->close();
	delete input;
}
