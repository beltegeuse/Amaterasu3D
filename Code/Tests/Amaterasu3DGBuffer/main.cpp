/*
 * main.cpp
 *
 *  Created on: Mar 9, 2011
 *      Author: Beltegeuse
 */

#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/Camera/CameraFly.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class WindowGBuffer : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
public:
	WindowGBuffer() :
		Window("Amaterasu3DTestApp")
	{
		// Camera Setup
		CameraAbstract* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 4000);
		// Config path
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza/textures");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/GBuffers");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Lighting/Deferred");
		// Load shader
		m_gbuffer_shader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		m_gbuffer_shader->begin();
		m_gbuffer_shader->setUniformMatrix4fv("ProjectionMatrix", m_matrixPerspective);
		m_gbuffer_shader->end();
		// Load GI
		m_GI = new DeferredLighting;
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
		// Create light 1
		PointLight light;
		light.LightColor = Color(1.0,1.0,1.0,0.0);
		light.Position = Math::TVector3F(3,4,2);
		light.LightRaduis = 100.0;
		light.LightIntensity = 1.0;
		m_GI->AddPointLight(light);
		// Load scene
//		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
//		GetSceneRoot().AddChild(node);
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("lion.obj");
		SceneGraph::Group* nodeGroup = new SceneGraph::Group;
		nodeGroup->AddChild(node);
		Math::CMatrix4 mat;
		mat.SetTranslation(-1000.0,0,0);
		nodeGroup->LoadTransformMatrix(mat);
		GetSceneRoot().AddChild(nodeGroup);
	}

	virtual ~WindowGBuffer()
	{
	}

	virtual void OnDraw(double delta)
	{
		m_gbuffer_shader->begin();
		Window::OnDraw(delta);
		m_gbuffer_shader->end();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MatrixManagement::Instance().PushMatrix(m_camera->GetMatrix());
		m_GI->ComputeIllumination();
		MatrixManagement::Instance().PopMatrix();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowGBuffer window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
