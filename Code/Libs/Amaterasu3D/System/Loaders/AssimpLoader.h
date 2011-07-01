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

#ifndef ASSIMPLOADER_H_
#define ASSIMPLOADER_H_
#include <Graphics/RenderableObject.h>
#include <Graphics/SceneNode/IMeshSceneNode.h>
#include <System/Loaders/Loader.h>
#include <Utilities/File.h>
#include <assimp.h>
#include "aiScene.h"
#include <map>
#include <string>

namespace ama3D
{
class AssimpLoader: public ILoader<IMeshSceneNode>
{
private:
	// Attributs
	struct aiLogStream m_assimp_stream;
	typedef std::vector<RenderableObject*> CachedAssimpMeshMap;
	CachedAssimpMeshMap m_cached_geom;
public:
	AssimpLoader();
	virtual ~AssimpLoader();
	virtual IMeshSceneNode* LoadFromFile(const std::string& Filename);
private:
	void GetMaterialPropreties(RenderableObject* assimpMesh,
			const struct aiMaterial *mtl);
	void BuildGroup(IMeshSceneNode* group, const aiScene* scene, aiNode* nd,
			const Math::CMatrix4& matrix);
	TTexturePtr LoadTexture(const CFile& name);
};
}

#endif /* ASSIMPLOADER_H_ */
