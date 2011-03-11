#include "Window.h"
#include <Debug/Exceptions.h>
#include <Graphics/MatrixManagement.h>
#include <Logger/Logger.h>
#include <Logger/LoggerDebug.h>
#include <System/Loaders/Loaders.h>
#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif
#include <GL/gl.h>
#include <Debug/OpenGLDebug.h>

Window::Window(const std::string& name, const Math::TVector2I& windowSize, bool syncVertical) :
	m_isRunning(false),
	m_camera(NULL)
{
	// **************************************
	// ******* Logger initialisation ********
	// **************************************
	Logger::SetLogger(new LoggerDebug);
	// **************************************
	// ******* Media initialisation *********
	// **************************************
	Loaders::RegisterAllLoaders();
	// **************************************
	// ********* SDL initialisation *********
	// **************************************
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "[ERROR] On the SDL initialisation : " << SDL_GetError() << std::endl;
		SDL_Quit();
		throw CException("Can't initialize SDL Context.");
	}
	// OpenGL Version ....
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// OpenGL Buffer instructions
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Creation of the SDL Window
	m_fenetre = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);
	// Sync between double buffer and the screen
	if(syncVertical)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);

	// **************************************
	// ********* GLEW initialisation ********
	// **************************************
	#ifdef WIN32
	GLenum initialisationGLEW = glewInit();
	// If there is an error
	if(initialisationGLEW != GLEW_OK)
	{
		// Print the error
		std::cout << "[Error] Can't initialize GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
		// Destroy SDL Window ...
		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();
		throw CException("Can't initialize GLEW.");
	}
	#endif

	// *******************************
	// ******** OpenGL initialisation
	// *******************************
	GLCheck(glEnable(GL_DEPTH_TEST));
}

Window::~Window()
{
	// Kill SDL Window
	SDL_GL_DeleteContext(m_contexteOpenGL);
	SDL_DestroyWindow(m_fenetre);
	SDL_Quit();
	// Camera delete
	if(m_camera)
		delete m_camera;
}

void Window::Run()
{
	SDL_Event evenements;
	m_isRunning = true;
	int lastTime = SDL_GetTicks();
	// Compteur de FPS
	m_FPS = 0;
	m_timeEslapse = 0.0;
	while(m_isRunning)
	{
		double delta = (SDL_GetTicks() - lastTime) / 1000.0;
		lastTime += delta*1000.0;
		// Events ......
		while(SDL_PollEvent(&evenements)) {
			OnEvent(evenements, delta);
		}
		// Mise a jour des FPS
		m_timeEslapse += delta;
		m_FPS += 1;
		if(m_timeEslapse > 1.0)
		{
			std::cout << "[INFO] FPS : " << m_FPS << std::endl;
			m_timeEslapse = m_timeEslapse - 1.0;
			m_FPS = 0;
		}
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    // Draw all the Scene ...
	    OnDraw(delta);
	    // Swap buffers
	    SDL_GL_SwapWindow(m_fenetre);
	}
}

void Window::OnEvent(SDL_Event& events, double delta)
{
	if(events.window.event == SDL_WINDOWEVENT_CLOSE)
		m_isRunning = false;
	if(m_camera)
		m_camera->OnEvent(events, delta);
}

void Window::OnDraw(double DeltaTime)
{
	////////////////////////////////////////////////
	// G-Buffer stage
	////////////////////////////////////////////////
	int numberMatrix = MatrixManagement::Instance().StackSize();

	if(m_camera)
	{
		m_camera->ComputeMatrix(DeltaTime);
		MatrixManagement::Instance().PushMatrix(m_camera->GetMatrix());
	}
	// Do all graphics part here
	// Draw the SceneGraph
	m_root.Draw();

	if(m_camera)
		MatrixManagement::Instance().PopMatrix();

	// Check if there is leak of matrix stack
	if(numberMatrix != MatrixManagement::Instance().StackSize())
		throw CException("Leak matrix is detected");
}

SceneGraph::Group& Window::GetSceneRoot()
{
	return m_root;
}

void Window::SetCamera(CameraAbstract* camera)
{
	if(m_camera)
		delete m_camera;
	m_camera = camera;
}
