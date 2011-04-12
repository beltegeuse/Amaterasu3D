/*
 * Skydome.h
 *
 *  Created on: Apr 12, 2011
 *      Author: Beltegeuse
 */

#ifndef SKYDOME_H_
#define SKYDOME_H_

#include <GL/glew.h>


#include <Graphics/MatrixManagement.h>
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <System/EventListeners.h>

class Skydome : public SceneGraph::DrawObject, public FrameListener
{
private:
	GLUquadric* m_Obj;
	TShaderPtr m_SkyShader;
	TTexturePtr m_TextureCloud;
	double m_Time;
public:
	Skydome();
	virtual ~Skydome();

	virtual void Draw();
	virtual void FrameStarted(double delta);
	virtual void FrameEnded() {}
};

#endif /* SKYDOME_H_ */
