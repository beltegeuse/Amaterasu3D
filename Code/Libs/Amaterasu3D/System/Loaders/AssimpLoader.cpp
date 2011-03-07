/*
 * AssimpLoader.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Adrien
 */

#include "AssimpLoader.h"
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <Logger/Logger.h>
#include <iostream>
#include <algorithm>
#include <Enum.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>

AssimpLoader::AssimpLoader()
{
	// Log initialisation
	m_assimp_stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&m_assimp_stream);
}

AssimpLoader::~AssimpLoader()
{
	aiDetachAllLogStreams();
}

SceneGraph::AssimpNode* AssimpLoader::LoadFromFile(const std::string& Filename)
{
	// Empty cache
	m_cached_geom.erase(m_cached_geom.begin(), m_cached_geom.end());
	// Load
	const struct aiScene* scene = aiImportFile(Filename.c_str(),aiProcessPreset_TargetRealtime_Quality);
	if(!scene)
		throw CException("assimp library can load model.");
	SceneGraph::AssimpNode* group = new SceneGraph::AssimpNode;
	BuildGroup(group, scene, scene->mRootNode);
	group->SetAssimpScene(scene);
	Logger::Log() << "[INFO] Finish to load : " << Filename << " mesh : "<< scene->mNumMeshes << "\n";
	return group;
}

void AssimpLoader::BuildGroup(SceneGraph::AssimpNode* group, const aiScene* scene, aiNode* nd)
{
	// Load transformation
	struct aiMatrix4x4 m = nd->mTransformation;
	//aiTransposeMatrix4(&m);
	Math::CMatrix4 matrix = Math::CMatrix4(m.a1,m.a2,m.a3,m.a4,
										   m.b1,m.b2,m.b3,m.b4,
										   m.c1,m.c2,m.c3,m.c4,
										   m.d1,m.d2,m.d3,m.d4);
	group->LoadTransformMatrix(matrix);

	// draw all meshes assigned to this node
	for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		if(mesh->mNumFaces == 0)
		{
			Logger::Log() << "[INFO] Skip assimp mesh. No faces. \n";
			continue;
		}
		if(m_cached_geom.find(nd->mMeshes[n]) != m_cached_geom.end())
		{
			Logger::Log() << "[INFO] Find cached resources : " << nd->mMeshes[n] << "\n";
			group->AddChild(m_cached_geom[nd->mMeshes[n]]);
			continue;
		}
		SceneGraph::AssimpMesh* assimpMesh = new SceneGraph::AssimpMesh;
//		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
//
//		if(mesh->mNormals == NULL) {
//			glDisable(GL_LIGHTING);
//		} else {
//			glEnable(GL_LIGHTING);
//		}
//
//		if(mesh->mColors[0] != NULL) {
//			glEnable(GL_COLOR_MATERIAL);
//		} else {
//			glDisable(GL_COLOR_MATERIAL);
//		}

		// Build the indice faces
		std::vector<unsigned int> indicesVector;
		std::vector<float> vertexVector;
		unsigned int maxIndice = 0;
		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			// Get a face elements
			const struct aiFace* face = &mesh->mFaces[t];
			if(face->mNumIndices != 3)
			{
				Logger::Log() << "[Warning] More or less indices faces (" << face->mNumIndices << "). \n";
				continue;
			}
			// Get all informations
			for(int i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];
				indicesVector.push_back(index);
				maxIndice = std::max(maxIndice, face->mIndices[i]);
//				vertexVector.push_back(mesh->mVertices[index].x);
//				vertexVector.push_back(mesh->mVertices[index].y);
//				vertexVector.push_back(mesh->mVertices[index].z);
//
//				if(mesh->mColors[0] != NULL)
//					Color4f(&mesh->mColors[0][index]);
//				if(mesh->mNormals != NULL)
//					glNormal3fv(&mesh->mNormals[index].x);
//				glVertex3fv(&mesh->mVertices[index].x);
			}
		}
		// Create the indice array
		unsigned int * indiceArray = new unsigned int[indicesVector.size()];
		float * vertexArray = new float[vertexVector.size()];
		for(unsigned int i = 0; i < indicesVector.size(); i++)
		{
			indiceArray[i] = indicesVector[i];
//			vertexArray[i*3] = vertexVector[i*3];
//			vertexArray[i*3+1] = vertexVector[i*3+1];
//			vertexArray[i*3+2] = vertexVector[i*3+2];
		}
		// Set all buffers
		Logger::Log() << "[INFO] Add indice buffer : " << indicesVector.size() << "\n";
		assimpMesh->SetIndiceBuffer(indiceArray, indicesVector.size());
		Logger::Log() << "[INFO] Add Vertex buffer ... \n";
		SceneGraph::AssimpMeshBuffer bufferVertex;
		bufferVertex.buffer = &mesh->mVertices[0].x;
		bufferVertex.dimension = 3;
		bufferVertex.size = maxIndice*3+3;
		Logger::Log() << "   * size : " << bufferVertex.size << "\n";
		assimpMesh->AddBuffer(bufferVertex, VERTEX_ATTRIBUT);
		// Compile all buffers
		Logger::Log() << "[INFO] Compile all buffers ... \n";
		assimpMesh->CompileBuffers();
		// Attach to group
		Logger::Log() << "[INFO] Add to father node... \n";
		group->AddChild(assimpMesh);
		Logger::Log() << "[INFO] Add cached resources : " << nd->mMeshes[n] << "\n";
		m_cached_geom[nd->mMeshes[n]] = assimpMesh;

	}

	// Add all childrens
	for (int n = 0; n < nd->mNumChildren; ++n) {
		SceneGraph::AssimpNode* assimpNode = new SceneGraph::AssimpNode;
		BuildGroup(assimpNode, scene, nd->mChildren[n]);
		group->AddChild(assimpNode);
	}

}
