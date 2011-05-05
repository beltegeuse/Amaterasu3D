/*
 * BinvoxModel.h
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#ifndef BINVOXMODEL_H_
#define BINVOXMODEL_H_

#include <string>

#include <Graphics/SceneGraph/Model.h>
#include <Math/Vector2.h>

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

	/*
	 * Public methods
	 */
	/// Texture methods
	Math::TVector2I TextureRepeat();
	Math::TVector2I TextureSize();
	TTexturePtr Create2DTexture();
	/// Model methods
	SceneGraph::Model* CreateDebugPointModel();

private:
	/**
	 * Privates methods
	 */
	void LoadFile(const std::string& file);
	void LoadHeader(std::ifstream *input);
	void LoadVoxels(std::ifstream *input);
};

#endif /* BINVOXMODEL_H_ */
