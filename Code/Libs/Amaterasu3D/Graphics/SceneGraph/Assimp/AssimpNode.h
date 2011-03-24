//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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

#ifndef ASSIMPNODE_H_
#define ASSIMPNODE_H_

#include <Graphics/SceneGraph/Group.h>
#include <System/ResourceManager.h>
#include <Utilities/SmartPtr.h>
#include <assimp.h>


namespace SceneGraph
{

	class AssimpNode : public Group, public IResource
	{
	private:
		const aiScene* m_scene;
	public:
		AssimpNode();
		virtual ~AssimpNode();
		void SetAssimpScene(const aiScene* scene);

		static SceneGraph::AssimpNode* LoadFromFile(const std::string& path);
	};
}

typedef CSmartPtr<SceneGraph::AssimpNode, CResourceCOM> TAssimpNodePtr;

#endif /* ASSIMPNODE_H_ */
