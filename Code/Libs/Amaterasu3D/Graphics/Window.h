#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>
#include <string>
#include <Math/Vector2.h>
#include <Graphics/SceneGraph/Group.h>

class Window
{
private:
	// Attributs
	SDL_WindowID m_fenetre;
	SDL_GLContext m_contexteOpenGL;
	bool m_isRunning;
	SceneGraph::Group m_root;

public:
	// Constructor & Destructor
	Window(const std::string& name = "OpenGL Renderer", const Math::TVector2I& windowSize = Math::TVector2I(800,600));
	virtual ~Window();

	// Public methods
	void Run();
	SceneGraph::Group& GetSceneRoot();

	// Virtual methods
	virtual void OnDraw();
	virtual void OnEvent(SDL_Event& events);
};

#endif /* WINDOW_H_ */