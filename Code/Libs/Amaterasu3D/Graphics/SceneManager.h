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

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

// Amaterasu3D includes
#include <Singleton.h>
#include <Graphics/SceneNode/ISceneNode.h>
#include <Graphics/SceneNode/ILightSceneNode.h>
#include <Graphics/SceneNode/ICameraSceneNode.h>
#include <Graphics/SceneNode/IMeshSceneNode.h>

// STL includes
#include <map>
#include <vector>
#include <list>

class CSceneManager : public CSingleton<CSceneManager>
{
	MAKE_SINGLETON(CSceneManager)
protected:
	/*
	 * Typedef
	 */
	typedef std::map<std::string, ILightSceneNode*> LightList;
	typedef std::map<std::string, ICameraSceneNode*> CameraList;
	typedef std::list<IMeshSceneNode*> MeshList;

	/*
	 * Attributes
	 */
	// Scenegraph knows
	LightList m_Lights;
	CameraList m_Cameras;
	MeshList m_Meshs;

	// Root of the scenegraph
	ISceneNode * m_Root;

public:
	/*
	 * Constructors & Destructors
	 */
	CSceneManager();
	virtual ~CSceneManager();


	/*
	 * Public methods
	 */
	// Creators
	ISceneNode* CreateSceneNode(ISceneNode* parent = 0);
	ILightSceneNode* CreateLightNode(ISceneNode* parent = 0);
	ICameraSceneNode* CreateCameraNode(ISceneNode* parent = 0);
	IMeshSceneNode* LoadMesh(const std::string file, ISceneNode* parent = 0);

	// Add Root
	void AddScenegraphRoot(ISceneNode* node);
	// Render all in scenegraph
	void RenderAll();

};

#endif /* SCENEMANAGER_H_ */
