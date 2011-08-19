#ifndef VOLUMETRICRENDERABLEOBJECT_H_
#define VOLUMETRICRENDERABLEOBJECT_H_
#include <glm/glm.hpp>
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
	glm::vec3 m_GridSize;
	glm::vec4 m_TextureSpec;
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
	void Initialise(TTexturePtr tex, const glm::ivec2& texSize,
			const glm::ivec2& texRepet, const glm::vec3& gridSize);
};

}
#endif /* VOLUMETRICRENDERABLEOBJECT_H_ */
