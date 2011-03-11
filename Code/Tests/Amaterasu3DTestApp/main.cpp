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

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

unsigned int IndiceArray[36] = {
0,1,2,2,1,3,
4,5,6,6,5,7,
3,1,5,5,1,7,
0,2,6,6,2,4,
6,7,0,0,7,1,
2,3,4,4,3,5
};

float CubeColorArray[24] = {
1.0f, 0.0f, 0.0f,
1.0f, 0.0f, 1.0f,
1.0f, 1.0f, 1.0f,
0.0f, 0.0f, 1.0f,
0.0f, 1.0f, 0.0f,
0.0f, 1.0f, 1.0f,
1.0f, 1.0f, 0.0f,
1.0f, 1.0f, 1.0f
};

float CubeVertexArray[24] = {
 -1.0f, 1.0f, -1.0f,
 -1.0f, -1.0f, -1.0f,
 -1.0f, 1.0f, 1.0f,
 -1.0f, -1.0f, 1.0f,
 1.0f, 1.0f, 1.0f,
 1.0f, -1.0f, 1.0f,
 1.0f, 1.0f, -1.0f,
 1.0f, -1.0f, -1.0f
};

float CubeUVArray[24] = {
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 1.0f, 1.0f
};




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
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_shader;
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
//		Logger::SetLogger(new LoggerFile("Log.out"));
		//TODO: Mettre dans OnEvent
//		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		// Camera
		CameraAbstract* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		SetCamera(cam);
		// OpenGL Flags ...
		GLCheck(glClearColor(0.1f,0.1f,0.1f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 10000);
		// Path search
		// Load the Shader
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza/textures");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/OldOpenGL");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/BasicShaders");
		m_shader = glShaderManager::Instance().LoadShader("BasicTextureShader.shader");
//		m_shader = glShaderManager::Instance().LoadShader("BasicShader.shader");
		m_shader->begin();
		m_shader->setUniformMatrix4fv("ProjectionMatrix", m_matrixPerspective);
		m_shader->end();
		// Create the Cube ...
		//CreateCubes();
//		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("dwarf.x");
//		GetSceneRoot().AddChild(node);
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		GetSceneRoot().AddChild(node);
//		CreateCubes();
		std::cout << "FINISH BUILD SCENE" << std::endl;
	}

	SceneGraph::Group* CreateCube(const Math::TVector3F& position)
	{
		SceneGraph::Group* group = new SceneGraph::Group;
		Math::CMatrix4 mat;
		mat.SetTranslation(position.x, position.y, position.z);
		group->LoadTransformMatrix(mat);
		SceneGraph::AssimpMesh* mesh = new SceneGraph::AssimpMesh;
		mesh->SetIndiceBuffer(IndiceArray,36);
		SceneGraph::AssimpMeshBuffer buffer;
		buffer.buffer = CubeVertexArray;
		buffer.dimension = 3;
		buffer.owner = false;
		buffer.size = 24;
		mesh->AddBuffer(buffer, VERTEX_ATTRIBUT);
		buffer.buffer = CubeUVArray;
		buffer.dimension = 2;
		buffer.size = 16;
		mesh->AddBuffer(buffer, TEXCOORD_ATTRIBUT);
		mesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("unknowTexture.tga"));
		mesh->CompileBuffers();
		group->AddChild(mesh);
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

	virtual void OnDraw(double delta)
	{
		// Compute ModelViewMatrix
		// Send matrix to the shader
		m_shader->begin();
		// Draw the geometry
		Window::OnDraw(delta);
		// End of the shader
		m_shader->end();
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
