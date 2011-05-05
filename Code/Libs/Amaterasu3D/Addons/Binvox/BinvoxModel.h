/*
 * BinvoxModel.h
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#ifndef BINVOXMODEL_H_
#define BINVOXMODEL_H_

#include <string>

class BinvoxModel {
private:
	///// Attributes
	// * Grid dimension
	int m_Depth, m_Height, m_Width;
	// * Grid position
	float m_Tx, m_Ty, m_Tz, m_Scale;
	// * Grid data
	unsigned char * m_Data;
public:
	// Constructor & Destuctors
	BinvoxModel(const std::string& file);
	virtual ~BinvoxModel();

private:
	/**
	 * Privates methods
	 */
	void LoadFile(const std::string& file);
	void LoadHeader(std::ifstream *input);
	void LoadVoxels(std::ifstream *input);
};

#endif /* BINVOXMODEL_H_ */
