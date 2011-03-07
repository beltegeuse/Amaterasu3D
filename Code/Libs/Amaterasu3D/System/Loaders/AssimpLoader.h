/*
 * AssimpLoader.h
 *
 *  Created on: Mar 7, 2011
 *      Author: Adrien
 */

#ifndef ASSIMPLOADER_H_
#define ASSIMPLOADER_H_
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/SceneGraph/Assimp/AssimpNode.h>
#include <System/Loaders/Loader.h>
#include <assimp.h>
#include "aiScene.h"
#include <map>
#include <string>

class AssimpLoader : public ILoader<SceneGraph::AssimpNode>
{
private:
	// Attributs
	struct aiLogStream m_assimp_stream;
	typedef std::map<int, SceneGraph::AssimpMesh*> CachedAssimpMeshMap;
	CachedAssimpMeshMap m_cached_geom;
public:
	AssimpLoader();
	virtual ~AssimpLoader();
	virtual SceneGraph::AssimpNode* LoadFromFile(const std::string& Filename);
private:
	void BuildGroup(SceneGraph::AssimpNode* group, const aiScene* scene, aiNode* nd);
};

#endif /* ASSIMPLOADER_H_ */
