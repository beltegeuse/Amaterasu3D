#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/Shader.h>
#include <windows.h>
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>


// Dessin de la geometrie
GLfloat CubeArray[48] = {
1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f,
1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f,
1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f,
0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,
1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f
};
GLuint IndiceArray[36] = {
0,1,2,2,1,3,
4,5,6,6,5,7,
3,1,5,5,1,7,
0,2,6,6,2,4,
6,7,0,0,7,1,
2,3,4,4,3,5
};

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// Assimp functions ....
void get_bounding_box_for_node (const struct aiScene* scene,
		const struct aiNode* nd,
		struct aiVector3D* min,
		struct aiVector3D* max,
		struct aiMatrix4x4* trafo
){
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(scene, nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------

void get_bounding_box (const struct aiScene* scene, struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene, scene->mRootNode,min,max,&trafo);
}

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
void Color4f(const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

void recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{
	int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	std::cout << "Draw Mesh : " << nd->mNumMeshes << std::endl;
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		std::cout << "Draw Faces : " <<  mesh->mNumFaces << std::endl;
		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			std::cout << face->mNumIndices << std::endl;
			switch(face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

class ConcreteWindow : public Window
{
private:
	const struct aiScene* m_scene;
	struct aiVector3D m_scene_center;
	float m_factor;
	Math::CMatrix4 m_matrixPerspective;
	Shader m_shader;
	unsigned int m_cubeBuffers[2];
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
		// OpenGL Flags ...
		GLCheck(glClearColor(0.1f,0.1f,0.1f,1.f));
		GLCheck(glEnable(GL_DEPTH_TEST));
		// Projection Matrix
		GLCheck(glMatrixMode(GL_PROJECTION));
		GLCheck(glLoadIdentity());
		GLCheck(gluPerspective(70, (double)800/600, 1, 1000));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 1000);
		// XXX docs say all polygons are emitted CCW, but tests show that some aren't.
//		if(getenv("MODEL_IS_BROKEN"))
//			glFrontFace(GL_CW);
//
//		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		// Path search
		MediaManager::Instance()->AddPathAndChilds("../Donnees");
		// model Load
		m_scene = aiImportFile(MediaManager::Instance()->GetPath("dwarf.x").c_str(),aiProcessPreset_TargetRealtime_Quality);
		// Search the midle of the scene
		struct aiVector3D scene_min, scene_max;
		if (m_scene) {
			get_bounding_box(m_scene,&scene_min,&scene_max);
			m_scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
			m_scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
			m_scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
			std::cout << "[INFO] Center info : " << m_scene_center.x << "x" << m_scene_center.y << "x" << m_scene_center.z << std::endl;
		}
		m_factor = scene_max.x-scene_min.x;
		m_factor = aisgl_max(scene_max.y - scene_min.y,m_factor);
		m_factor = aisgl_max(scene_max.z - scene_min.z,m_factor);
		m_factor = 1.f / m_factor;
		std::cout << "[INFO] Factor info : " << m_factor << std::endl;
		// Load the Shader
		m_shader = Shader(MediaManager::Instance()->GetPath("BasicShaderOld.vert"),MediaManager::Instance()->GetPath("BasicShaderOld.frag"));
		// Create the Cube ...
		CreateCube();
	}

	void CreateCube()
	{
		// Génération des buffers
		std::cout << "[INFO] Gen Buffer ..." << std::endl;
		GLCheck(glGenBuffers( 2, m_cubeBuffers ));
		// Buffer d'informations de vertex
		std::cout << "[INFO] Fill Array Buffer ..." << std::endl;
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
		GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(CubeArray), CubeArray, GL_STATIC_DRAW));
		// Buffer d'indices
		std::cout << "[INFO] Fill Element Array Buffer ..." << std::endl;
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
		GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndiceArray), IndiceArray, GL_STATIC_DRAW));
	}

	virtual ~ConcreteWindow()
	{
	}

	virtual void OnDraw()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
//		glTranslatef(0,0,-m_scene_center.z);
		gluLookAt(3,4,2,0,0,0,0,0,1);
		Window::OnDraw();
		// Create matrix lookat
		Math::CMatrix4 matrixLookAt;
		matrixLookAt.LookAt(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0), Math::TVector3F(0,0,1));
		// Create scale martix
		Math::CMatrix4 matrixScale;
		matrixScale.SetScaling(m_factor,m_factor,m_factor);
		// Create translate Matrix
		Math::CMatrix4 matrixTranslate;
		//matrixTranslate.SetTranslation(-m_scene_center.x,-m_scene_center.y,-m_scene_center.z);
		matrixTranslate.SetTranslation(0,0,-m_scene_center.z);
		// Compute ModelViewMatrix
		Math::CMatrix4 ModelViewMatrix;
		ModelViewMatrix = matrixLookAt;//matrixTranslate;//matrixLookAt;//*matrixScale*matrixTranslate;
		// Send matrix to the shader
		m_shader.Begin();
		m_shader.SetUniformMatrix4fv("ModelViewMatrix", ModelViewMatrix);
		m_shader.SetUniformMatrix4fv("ProjectionMatrix", m_matrixPerspective);
		// Draw the geometry
		//  * Les differents blindings ...
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
		GLCheck(glVertexPointer( 3, GL_FLOAT, 6 * sizeof(float), ((float*)NULL + (3)) ));
		GLCheck(glColorPointer( 3, GL_FLOAT, 6 * sizeof(float), 0 ));
		//GLCheck(glEnableVertexAttribArray(m_shader.GetAttribLocation("VertexPosition")));
		//GLCheck(glVertexAttribPointer (m_shader.GetAttribLocation("VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0) );
		// * le dessins en lui meme
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
		GLCheck(glEnableClientState( GL_VERTEX_ARRAY ));
		GLCheck(glEnableClientState( GL_COLOR_ARRAY ));
		GLCheck(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
		//GLCheck(glDisableVertexAttribArray(m_shader.GetAttribLocation("VertexPosition")));
		GLCheck(glDisableClientState( GL_COLOR_ARRAY ));
		GLCheck(glDisableClientState( GL_VERTEX_ARRAY ));
		// End of the shader
		m_shader.End();

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
