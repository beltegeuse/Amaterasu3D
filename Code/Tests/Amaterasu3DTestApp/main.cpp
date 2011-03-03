#include <iostream>

#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <windows.h>
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <GL/gl.h>
#include <GL/glu.h>

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
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

//		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

//		if(mesh->mNormals == NULL) {
//			glDisable(GL_LIGHTING);
//		} else {
//			glEnable(GL_LIGHTING);
//		}

		if(mesh->mColors[0] != NULL) {
			glEnable(GL_COLOR_MATERIAL);
		} else {
			glDisable(GL_COLOR_MATERIAL);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
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
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
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
		}
	}

	virtual ~ConcreteWindow()
	{
	}

	virtual void OnDraw()
	{
		Window::OnDraw();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);
		glTranslatef(-m_scene_center.x,-m_scene_center.y,-m_scene_center.z);
		recursive_render(m_scene, m_scene->mRootNode);
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
