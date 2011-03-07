/*
 * AssimpNode.h
 *
 *  Created on: Mar 7, 2011
 *      Author: Adrien
 */

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
