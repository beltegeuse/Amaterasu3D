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

#include "SceneManager.h"
#include <Graphics/SceneNode/SimpleRenderable/DebugCubeLeaf.h>
#include <Graphics/SceneNode/SimpleRenderable/DebugPlaneLeaf.h>

namespace ama3D
{
SINGLETON_IMPL(CSceneManager)

CSceneManager::CSceneManager() :
		m_Root(new ISceneNode("Root", 0))
{
}

CSceneManager::~CSceneManager()
{
	delete m_Root;
}

ISceneNode* CSceneManager::CreateSceneNode(const std::string& name,ISceneNode* parent)
{
	return new ISceneNode(name, parent);
}

ILightSceneNode* CSceneManager::CreateLightNode(const std::string& name, ISceneNode* parent)
{
	ILightSceneNode* light = new ILightSceneNode(name, parent);
	Assert(m_Lights.find(name) == m_Lights.end()); // Absente de la liste
	m_Lights[name] = light;
	return light;
}

ICameraSceneNode* CSceneManager::CreateCameraNode(const std::string& name, ISceneNode* parent)
{
	return new ICameraSceneNode(name, parent);
}

IMeshSceneNode* CSceneManager::LoadMesh(const std::string& file,
		ISceneNode* parent)
{
	IMeshSceneNode* mesh = IMeshSceneNode::LoadFromFile(file, file, parent);
	m_Meshs.push_back(mesh);
	return mesh;
}

ISimpleRenderableSceneNode* CSceneManager::CreateSimpleMesh(
		const SimpleMesh& type, const std::string& name, ISceneNode* node)
{
	ISimpleRenderableSceneNode* obj = 0;
	switch (type)
	{
	case MESH_CUBE:
		obj = new DebugCubeLeaf(name, node);
		break;
	case MESH_PLANE:
		obj = new DebugPlaneLeaf(name, node);
		break;
	default:
		throw new CException("Unknow Mesh type");
	}
	m_Meshs.push_back(obj);
	return obj;
}

void CSceneManager::AddScenegraphRoot(ISceneNode* node)
{
	m_Root->AddChild(node);
}

void CSceneManager::RenderAll()
{
	// Update Transformation Cache
	for(std::vector<ISceneNode*>::iterator it = m_NodeNeedsUpdate.begin();
		m_NodeNeedsUpdate.end() != it; ++it)
		(*it)->UpdateTransformations();
	m_NodeNeedsUpdate.clear();

	// Render all Mesh
	for (RenderableList::iterator it = m_Meshs.begin(); it != m_Meshs.end();
			++it)
			{
		(*it)->Render();
	}
}

void CSceneManager::NeedUpdateNextRender(ISceneNode* node)
{
	m_NodeNeedsUpdate.push_back(node);
}

}
