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

#include "AssimpLoader.h"
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <Logger/Logger.h>
#include <iostream>
#include <algorithm>
#include <Enum.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/Texture.h>
#include <Utilities/File.h>

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
	//TODO: Add automatic path append for textures
	// Empty cache
	m_cached_geom.erase(m_cached_geom.begin(), m_cached_geom.end());
	// Load
	const struct aiScene* scene = aiImportFile(Filename.c_str(),aiProcessPreset_TargetRealtime_Quality | aiProcess_TransformUVCoords | aiProcess_FindInstances | aiProcess_Triangulate );
	if(!scene)
		throw CException("assimp library can load model.");
	SceneGraph::AssimpNode* group = new SceneGraph::AssimpNode;
	BuildGroup(group, scene, scene->mRootNode);
	group->SetAssimpScene(scene);
	Logger::Log() << "[INFO] Finish to load : " << Filename << " mesh : "<< scene->mNumMeshes << "\n";
	return group;
}

void AssimpLoader::GetMaterialPropreties(SceneGraph::AssimpMesh* assimpMesh, const struct aiMaterial *mtl)
{
	struct aiColor4D diffuseColor;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
	{

	}
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
		if(m_cached_geom.find(nd->mMeshes[n]) != m_cached_geom.end())
		{
			Logger::Log() << "[INFO] Found an instance ... Skip loading \n";
			group->AddChild(m_cached_geom.find(nd->mMeshes[n])->second );
		}
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
			}
		}
		// Create the indice array
		unsigned int * indiceArray = new unsigned int[indicesVector.size()];
		for(unsigned int i = 0; i < indicesVector.size(); i++)
		{
			indiceArray[i] = indicesVector[i];
		}
		// Set all buffers
		// * Indice Buffer
		Logger::Log() << "[INFO] Add indice buffer : " << indicesVector.size() << "\n";
		assimpMesh->SetIndiceBuffer(indiceArray, indicesVector.size());
		// * Vertex buffer
		Logger::Log() << "[INFO] Add Vertex buffer ... \n";
		SceneGraph::AssimpMeshBuffer buffer;
		buffer.buffer = &mesh->mVertices[0].x;
		buffer.dimension = 3;
		buffer.size = maxIndice*3+3;
		buffer.owner = false;
		Logger::Log() << "   * size : " << buffer.size << "\n";
		assimpMesh->AddBuffer(buffer, VERTEX_ATTRIBUT);
		//  * Normal buffer
		if(mesh->HasNormals())
		{
			buffer.buffer = &mesh->mNormals[0].x;
			assimpMesh->AddBuffer(buffer, NORMAL_ATTRIBUT);
		}
		//  * Tangentes and bitangantes
		if(mesh->HasTangentsAndBitangents())
		{
			buffer.buffer = &mesh->mTangents[0].x;
			assimpMesh->AddBuffer(buffer, TANGENT_ATTRIBUT);
			buffer.buffer = &mesh->mBitangents[0].x;
			assimpMesh->AddBuffer(buffer, BITANGENT_ATTRIBUT);
		}
		// * Couleurs
		if(mesh->GetNumColorChannels() > 0) //FIXME : Verfier le Loader ...
		{
			// TODO: Gestion des couleurs multiples
			if(mesh->GetNumColorChannels() > 1)
			{
				throw CException("Too Color Channels");
			}
			else
			{
				buffer.size = maxIndice*3+3;
				buffer.owner = true;
				buffer.dimension = 3;
				float * ColorArray = new float[maxIndice*3+3];
				for(int i = 0; i < maxIndice+1; i++)
				{
					ColorArray[i*3] =   mesh->mColors[0][i].r;
					ColorArray[i*3+1] = mesh->mColors[0][i].g;
					ColorArray[i*3+2] = mesh->mColors[0][i].b;
				}
				buffer.buffer = ColorArray;
				assimpMesh->AddBuffer(buffer, COLOR_ATTRIBUT);
			}
		}
		//  * UV Coords
		if(mesh->GetNumUVChannels() > 0)
		{
			// TODO: Gestion des materiaux multiples
			if(mesh->GetNumUVChannels() > 1)
			{
				throw CException("Too UV Channels");
			}
			else
			{
				buffer.size = maxIndice*2+2;
				buffer.owner = true;
				buffer.dimension = 2;
				float * UVArray = new float[maxIndice*2+2];
				for(int i = 0; i < maxIndice+1; i++)
				{
					UVArray[i*2] = mesh->mTextureCoords[0][i].x;
					UVArray[i*2+1] = mesh->mTextureCoords[0][i].y;
				}
				buffer.buffer = UVArray;
				assimpMesh->AddBuffer(buffer, TEXCOORD_ATTRIBUT);
				// Load diffuse image
				const struct aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				if(material->GetTextureCount(aiTextureType_DIFFUSE)>0)
				{
					aiString AiPath;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &AiPath);
					CFile texturePath = std::string(AiPath.data);
					Logger::Log() << "[INFO] Diffuse texture : " << texturePath.Filename() << "\n";
					assimpMesh->AddTextureMap(DIFFUSE_TEXTURE, LoadTexture(texturePath));
				}
				if(material->GetTextureCount(aiTextureType_SPECULAR)>0)
				{
					aiString AiPath;
					material->GetTexture(aiTextureType_SPECULAR, 0, &AiPath);
					CFile texturePath = std::string(AiPath.data);
					Logger::Log() << "[INFO] Specular Texture : " << texturePath.Filename() << "\n";
					assimpMesh->AddTextureMap(SPECULAR_TEXTURE, LoadTexture(texturePath));
				}
				//TODO: Desactivation du bump mapping
//				if(material->GetTextureCount(aiTextureType_HEIGHT)>0)
//				{
//					aiString AiPath;
//					material->GetTexture(aiTextureType_HEIGHT, 0, &AiPath);
//					CFile texturePath = std::string(AiPath.data);
//					Logger::Log() << "[INFO] Normal texture : " << texturePath.Filename() << "\n";
//					assimpMesh->AddTextureMap(NORMAL_TEXTURE, LoadTexture(texturePath));
//				}


			}
		}
		//FIXME : Faire le rechargement des materiaux
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

TTexturePtr AssimpLoader::LoadTexture(const CFile& file)
{
	TTexturePtr texturePtr;
	try
	{
		texturePtr = Texture::LoadFromFile(file.Filename());
	}
	catch(CException e)
	{
		texturePtr = Texture::LoadFromFile("unknowTexture.tga");
	}
	return texturePtr;
}
