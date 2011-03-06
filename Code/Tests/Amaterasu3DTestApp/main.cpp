#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/MatrixManagement.h>

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include "helpers.h"

void SawOpenGL(Math::CMatrix4& mat)
{
	float* m = (float*)mat;
	std::cout << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << " " << std::endl;
	std::cout << m[4] << " " << m[5] << " " << m[6] << " " << m[7] << " " << std::endl;
	std::cout << m[8] << " " << m[9] << " " << m[10] << " " << m[11] << " " << std::endl;
	std::cout << m[12] << " " << m[13] << " " << m[14] << " " << m[15] << " " << std::endl;
}

class ConcreteWindow : public Window
{
private:
	const struct aiScene* m_scene;
	struct aiVector3D m_scene_center;
	Math::CMatrix4 m_matrixPerspective;
	glShader* m_shader;
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
		// OpenGL Flags ...
		GLCheck(glClearColor(0.1f,0.1f,0.1f,1.f));

		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 1000);
		// Path search
		// Load the Shader
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/OldOpenGL");
		//ShaderUnit* testShader = CMediaManager::Instance().LoadMediaFromFile<ShaderUnit>("BasicShaderOld.vert");
		m_shader = CMediaManager::Instance().LoadMediaFromFile<glShader>("BasicShaderOld.shader");
		m_shader->begin();
		m_shader->SetUniformMatrix4fv("ProjectionMatrix", m_matrixPerspective);
		m_shader->end();
		// Create the Cube ...
		CreateCubes();
	}

	SceneGraph::Group* CreateCube(const Math::TVector3F& position)
	{
		SceneGraph::Group* group = new SceneGraph::Group;
		Math::CMatrix4 mat;
		mat.SetTranslation(position.x, position.y, position.z);
		group->LoadTransformMatrix(mat);
		group->AddChild(new DebugCubeLeaf());
		return group;
	}

	void CreateCubes()
	{
		// Multiple cube creation
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 10; j++)
				for(int k = 0; k < 10; k++)
					GetSceneRoot().AddChild(CreateCube(Math::TVector3F(i*10 - 50,j*10 - 50,k*10 - 50)));
	}

	virtual ~ConcreteWindow()
	{
	}

	void DebugMatrix(Math::CMatrix4& ModelViewMatrix)
	{
		std::cout << "ModelViewMatrix : " << std::endl;
		SawOpenGL(ModelViewMatrix);
		std::cout << "ProjectionMatrix : " << std::endl;
		SawOpenGL(m_matrixPerspective);
	}

	virtual void OnDraw()
	{
		// Create matrix lookat
		Math::CMatrix4 matrixLookAt;
		matrixLookAt.LookAt(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0), Math::TVector3F(0,0,1));
		// Compute ModelViewMatrix
		Math::CMatrix4 ModelViewMatrix;
		ModelViewMatrix = matrixLookAt;
		// Send matrix to the shader
		m_shader->begin();
		MatrixManagement::Instance().PushMatrix(ModelViewMatrix);
		// Draw the geometry
		Window::OnDraw();
		// End of the shader
		MatrixManagement::Instance().PopMatrix();
		m_shader->end();

		//recursive_render(m_scene, m_scene->mRootNode);
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	ConcreteWindow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
