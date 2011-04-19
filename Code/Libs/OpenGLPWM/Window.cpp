/*
 * Window.cpp
 *
 *  Created on: Apr 17, 2011
 *      Author: Beltegeuse
 */

#include <Window.h>
#include <WindowImpl.h>
Window::Window() :
	m_Window(NULL)
{}

Window::~Window()
{
	Close();
}

void Window::Create(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings)
{
	m_Window = WindowImpl::Create(mode,name,settings);
}

void Window::Display()
{
	m_Window->Display();
}

bool Window::PoolEvent(Event& event)
{
	if(!m_Window)
		return false;
	return m_Window->GetEvent(event);
}
