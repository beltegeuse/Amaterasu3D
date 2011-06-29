/*
 * IMeshSceneNode.h
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#ifndef IMESHSCENENODE_H_
#define IMESHSCENENODE_H_

#include <Graphics/SceneNode/IRenderableSceneNode.h>
#include <Graphics/RenderableObject.h>
#include <System/Resource.h>
#include <assimp.h>
#include <map>


class IMeshSceneNode : public IRenderableSceneNode //, public IResource
{
private:
	typedef std::map<RenderableObject*, Math::CMatrix4> RenderableWorldObjectsList;
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
	void AddRenderableObject(RenderableObject* obj, const Math::CMatrix4& trans);
	/*
	 * Static methods
	 */
	static IMeshSceneNode* LoadFromFile(const std::string& path, const std::string& name, ISceneNode* parent);
};

//typedef CSmartPtr<IMeshSceneNode, CResourceCOM> IMeshPtr;

#endif /* IMESHSCENENODE_H_ */
