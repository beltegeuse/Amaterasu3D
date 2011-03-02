#include <SDL/SDL.h>
#include <iostream>
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Notre fenêtre et le clavier
    SDL_WindowID fenetre;
    SDL_GLContext contexteOpenGL;

    SDL_Event evenements;
    bool terminer = false;

    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur lors de l'initialisation de la SDL : %s", SDL_GetError());
        SDL_Quit();

        return -1;
    }

    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Création de la fenêtre
    fenetre = SDL_CreateWindow("Triangle avec OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    contexteOpenGL = SDL_GL_CreateContext(fenetre);


    // Synchronisation entre le rafraichissement de l'écran et le Double Buffer
    SDL_GL_SetSwapInterval(1);

    // On initialise GLEW
    GLenum initialisationGLEW = glewInit();

    // Si l'initialisation a échouée :
    if(initialisationGLEW != GLEW_OK)
    {
        // On affiche l'erreur grâce à la fonction : glewGetErrorString(GLenum code)
        printf("Erreur d'initialisation de GLEW : %s", glewGetErrorString(initialisationGLEW));


        // On quitte la SDL
        SDL_GL_DeleteContext(contexteOpenGL);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();

        return -1;
    }

    // Vertices et coodonnées
    float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};

    // Loading the model
    // *** Initialise the library
    struct aiLogStream stream;
    stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
    aiAttachLogStream(&stream);
    // *** Load the Scene
    const struct aiScene* scene = aiImportFile("../Donnees/Model/dwarf.x",aiProcessPreset_TargetRealtime_Quality);
    struct aiVector3D scene_min, scene_max, scene_center;
    if (scene) {
		get_bounding_box(scene,&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
	}

    std::cout << "[INFO] Go to the loop rendering system..." << std::endl;
    // Boucle principale
    while(!terminer)
    {
        // Gestion des évènements
        SDL_WaitEvent(&evenements);
        if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = true;

        // Nettoyage de l'écran
        glClear(GL_COLOR_BUFFER_BIT);

        // On remplie puis on active le tableau Vertex Attrib 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);

        // On affiche le triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // On désactive le tableau Vertex Attrib puisque l'on n'en a plus besoin
        glDisableVertexAttribArray(0);

        // Actualisation de la fenêtre
        SDL_GL_SwapWindow(fenetre);
    }

    std::cout << "[INFO] ... end." << std::endl;
    // cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	aiReleaseImport(scene);

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();

    // On quitte la SDL
    SDL_GL_DeleteContext(contexteOpenGL);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
