/*
 * VolumetricRenderableObject.h
 *
 *  Created on: Jul 19, 2011
 *      Author: adrien
 */

#ifndef VOLUMETRICRENDERABLEOBJECT_H_
#define VOLUMETRICRENDERABLEOBJECT_H_

#include <Graphics/Shaders/Shader.h>
#include <Graphics/Camera/CameraAbstract.h>
#include <Graphics/SceneNode/ISimpleRenderableSceneNode.h>
namespace ama3D
{

class VolumetricRenderableObject
{
protected:
	/*
	 * Attributes
	 */
	// Graphics attributes
	TShaderPtr m_CubeShader;
	TShaderPtr m_volumeRenderingShader;
	TTexturePtr m_VolumeTexture;
	FBO * m_FrontFBO;
	FBO * m_BackFBO;
	ISimpleRenderableSceneNode* m_CubeModel;
	// Information attributes
	Math::TVector3F m_GridSize;
	Math::TVector4F m_TextureSpec;
	// Protection
	bool m_Initialized;
public:
	VolumetricRenderableObject(const std::string& shaderfile);
	virtual ~VolumetricRenderableObject();

	void UpdateCubeFBO(CameraAbstract* cam);
	virtual void Render();

protected:
	// Render methods
	virtual void Bind();
	virtual void Unbind();

	// Set data methods
	void Initialise(TTexturePtr tex, const Math::TVector2I& texSize,
			const Math::TVector2I& texRepet, const Math::TVector3F& gridSize);
};

}
#endif /* VOLUMETRICRENDERABLEOBJECT_H_ */
