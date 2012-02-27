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

#ifndef IMESHSCENENODE_H_
#define IMESHSCENENODE_H_

#include <Graphics/SceneNode/IRenderableSceneNode.h>
#include <Graphics/RenderableObject.h>
#include <System/Resource.h>
#include <assimp.h>
#include <map>

namespace ama3D
{
class IMeshSceneNode: public IRenderableSceneNode //, public IResource
{
private:
	typedef std::map<RenderableObject*, glm::mat4x4> RenderableWorldObjectsList;
	/*
	 * Attributes
	 */
	RenderableWorldObjectsList m_Objects;
	const aiScene* m_scene;
public:
	IMeshSceneNode(const std::string& name, ISceneNode* parent);
	virtual ~IMeshSceneNode();

	/*
	 * Public methods
	 */
	virtual void Render();
	void SetAssimpScene(const aiScene* scene);
	void AddRenderableObject(RenderableObject* obj,
			const glm::mat4x4& trans);
	RenderableWorldObjectsList& GetRenderableObjects();
	/*
	 * Static methods
	 */
	static IMeshSceneNode* LoadFromFile(const std::string& path,
			const std::string& name, ISceneNode* parent);
};

//typedef CSmartPtr<IMeshSceneNode, CResourceCOM> IMeshPtr;
}

#endif /* IMESHSCENENODE_H_ */
