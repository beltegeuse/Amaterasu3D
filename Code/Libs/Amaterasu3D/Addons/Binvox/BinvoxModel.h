/*
 * BinvoxModel.h
 *
 *  Created on: May 5, 2011
 *      Author: adrien
 */

#ifndef BINVOXMODEL_H_
#define BINVOXMODEL_H_

#include <string>
#include "glm/glm.hpp"
#include <Graphics/Texture.h>
#include <Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.h>
#include <Addons/Binvox/VolumetricRenderableObject.h>

namespace ama3D
{
class BinvoxModel : public VolumetricRenderableObject
{
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

	virtual void Bind();

private:
	/**
	 * Privates methods
	 */
	void LoadFile(const std::string& file);
	void LoadHeader(std::ifstream *input);
	void LoadVoxels(std::ifstream *input);
	float GetData(int x, int y, int z)
	{
		return m_Data[x * m_Width * m_Height + z * m_Width + y];
	}

	/// Grid informations
	 glm::vec3 GridSize() const;
	/// Texture methods
	glm::ivec2 TextureRepeat();
	glm::ivec2 TextureSize();
	TTexturePtr Create2DTexture();
};
}

#endif /* BINVOXMODEL_H_ */
