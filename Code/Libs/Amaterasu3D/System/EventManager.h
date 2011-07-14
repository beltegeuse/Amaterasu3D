//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <Singleton.h>
#include <System/EventListeners.h>
#include <vector>
#include <map>
#include <algorithm>
#include <Debug/Exceptions.h>
#include <Utilities/GenHierarchies.h>

namespace ama3D
{
////////////////////////////////////////////////////////////
/// Methods to add listeners manager
////////////////////////////////////////////////////////////
class CListenerManagerAbstract
{
public:
	virtual ~CListenerManagerAbstract()
	{
	}
	/*
	 * Pure virtual functions
	 */
	virtual void OnEvent(C3::Event& event) = 0;
	virtual void OnUpdate(double delta) = 0;
	virtual void OnEndRender() = 0;
};

template<class T>
class CListenerManager: public CListenerManagerAbstract
{
protected:
	typedef std::vector<T*> TListenerVector;
	TListenerVector m_Listeners; ///< To have listeners
public:
	/*
	 * Constructors and destructors
	 */
	CListenerManager()
	{
	}
	virtual ~CListenerManager()
	{
	}

	/*
	 * Public functions
	 */
	void AddListener(T* listener)
	{
		m_Listeners.push_back(listener);
	}
	void RemoveListener(T* listener)
	{
		typename TListenerVector::iterator it;
		it = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
		if (it == m_Listeners.end())
			throw CException("Enable to find listener to delete");
		m_Listeners.erase(it);
	}
};

////////////////////////////////////////////////////////////
/// Definition des differents listeners manager
////////////////////////////////////////////////////////////
class CFrameListenerManager: public CListenerManager<FrameListener>
{
public:
	virtual void OnEvent(C3::Event& event)
	{
	}
	virtual void OnUpdate(double delta)
	{
		for (TListenerVector::iterator it = m_Listeners.begin();
				it != m_Listeners.end(); it++)
				{
			(*it)->FrameStarted(delta);
		}
	}
	virtual void OnEndRender()
	{
		for (TListenerVector::iterator it = m_Listeners.begin();
				it != m_Listeners.end(); it++)
				{
			(*it)->FrameEnded();
		}
	}
};

template<class T>
class CNoFrameListenerManager: public CListenerManager<T>
{
public:
	virtual void OnUpdate(double delta)
	{
	}
	virtual void OnEndRender()
	{
	}
};

class CMouseListenerManager: public CNoFrameListenerManager<MouseListener>
{
public:
	virtual void OnEvent(C3::Event& event)
	{
		if (event.Type == C3::Event::MouseButtonPressed)
		{
			for (TListenerVector::iterator it = m_Listeners.begin();
					it != m_Listeners.end(); it++)
			{
				(*it)->MousePressed();
			}
		}
		else if (event.Type == C3::Event::MouseButtonReleased)
		{
			for (TListenerVector::iterator it = m_Listeners.begin();
					it != m_Listeners.end(); it++)
			{
				(*it)->MouseReleased();
			}
		}
	}
};

class CKeyListenerManager: public CNoFrameListenerManager<KeyListener>
{
public:
	virtual void OnEvent(C3::Event& event)
	{
		if (event.Type == C3::Event::KeyPressed)
		{
			for (TListenerVector::iterator it = m_Listeners.begin();
					it != m_Listeners.end(); it++)
					{
				(*it)->KeyPressed(event.Key.Code);
			}
		}
		else if (event.Type == C3::Event::KeyReleased)
		{
			for (TListenerVector::iterator it = m_Listeners.begin();
					it != m_Listeners.end(); it++)
					{
				(*it)->KeyReleased(event.Key.Code);
			}
		}
	}
};

class CMouseMotionListenerManager: public CNoFrameListenerManager<
		MouseMotionListener>
{
public:
	virtual void OnEvent(C3::Event& event)
	{
		if (event.Type== C3::Event::MouseMoved)
		{
			for (TListenerVector::iterator it = m_Listeners.begin();
					it != m_Listeners.end(); it++)
					{
				(*it)->MouseMoved(event.MouseMove.X, event.MouseMove.Y);
			}
		}
	}
};

//XXX: Not implemented
class CWindowListenerManager: public CNoFrameListenerManager<MouseMotionListener>
{

};

////////////////////////////////
// Class manage all Events
////////////////////////////////
class CEventManager: public CSingleton<ama3D::CEventManager>
{
MAKE_SINGLETON(ama3D::CEventManager)
private:
	typedef std::map<std::string, CListenerManagerAbstract*> TListenerManagerMap;
	TListenerManagerMap m_Managers;
public:
	/*
	 * Constructors and destructors
	 */
	CEventManager()
	{
		m_Managers["FrameListener"] = new CFrameListenerManager;
		m_Managers["MouseListener"] = new CMouseListenerManager;
		m_Managers["KeyListener"] = new CKeyListenerManager;
		m_Managers["MouseMotionListener"] = new CMouseMotionListenerManager;
		//TODO: m_Managers.push_back(new CWindowListenerManager);
	}
	virtual ~CEventManager()
	{
	}

	/*
	 * Methodes de mise a jour
	 */
	void OnEvent(C3::Event& event);
	void OnUpdate(double delta);
	void OnEndRender();

	template<typename T> void AddListener(const std::string& name, T* listener)
	{
		dynamic_cast<CListenerManager<T>*>(m_Managers[name])->AddListener(
				listener);
	}
	template<typename T> void RemoveListener(const std::string& name,
			T* listener)
	{
		dynamic_cast<CListenerManager<T>*>(m_Managers[name])->RemoveListener(
				listener);
	}
};

}

#endif /* EVENTMANAGER_H_ */
