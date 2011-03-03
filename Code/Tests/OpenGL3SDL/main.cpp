#include <SDL/SDL.h>

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Notre fenêtre et le clavier

    SDL_WindowID fenetre;
    SDL_GLContext contexteOpenGL;

    SDL_Event evenements;
    char terminer = 0;


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


    #ifdef WIN32

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

    #endif


    // Vertices et coodonnées

    float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};


    // Boucle principale

    while(!terminer)
    {
        // Gestion des évènements
        SDL_WaitEvent(&evenements);
        if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
            terminer = 1;


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


    // On quitte la SDL

    SDL_GL_DeleteContext(contexteOpenGL);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}

