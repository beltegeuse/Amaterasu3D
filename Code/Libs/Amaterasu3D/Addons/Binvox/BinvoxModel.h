/*
 * BinvoxModel.h
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#ifndef BINVOXMODEL_H_
#define BINVOXMODEL_H_

#include <string>

#include <Graphics/Texture.h>
#include <Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

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
	/// Grid informations
	Math::TVector3F GridSize() const;
	/// Texture methods
	Math::TVector2I TextureRepeat();
	Math::TVector2I TextureSize();
	TTexturePtr Create2DTexture();
	/// Model methods
	ISimpleRenderableSceneNode* CreateDebugPointModel();
	DebugCubeLeaf* CreateCoordinateCubeModel();

private:
	/**
	 * Privates methods
	 */
	void LoadFile(const std::string& file);
	void LoadHeader(std::ifstream *input);
	void LoadVoxels(std::ifstream *input);
	float GetData(int x, int y, int z)
	{
		return m_Data[x * m_Width*m_Height + z * m_Width + y];
	}
};

#endif /* BINVOXMODEL_H_ */
