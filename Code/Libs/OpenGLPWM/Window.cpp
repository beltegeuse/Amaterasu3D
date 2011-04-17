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
{}

void Window::Create(const WindowMode& mode, const std::string& name)
{
	m_Window = WindowImpl::Create(mode,name);
}

void Window::Display()
{
	m_Window->Display();
}
