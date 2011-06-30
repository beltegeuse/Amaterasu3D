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

#include "AssimpLoader.h"
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <Logger/Logger.h>
#include <iostream>
#include <algorithm>
#include <Enum.h>
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

IMeshSceneNode* AssimpLoader::LoadFromFile(const std::string& Filename)
{
	//XXX: Add automatic path append for textures
	// Empty cache
	m_cached_geom.erase(m_cached_geom.begin(), m_cached_geom.end());
	// Load
	const struct aiScene* scene = aiImportFile(Filename.c_str(), aiProcessPreset_TargetRealtime_Fast);
	if(!scene)
		throw CException("assimp library can load model.");
	IMeshSceneNode* group = new IMeshSceneNode(Filename, 0);
	BuildGroup(group, scene, scene->mRootNode, Math::CMatrix4());
	group->SetAssimpScene(scene);
	Logger::Log() << "[INFO] Finish to load : " << Filename << " mesh : "<< scene->mNumMeshes << "\n";
	return group;
}

void AssimpLoader::GetMaterialPropreties(RenderableObject* assimpMesh, const struct aiMaterial *mtl)
{
	Color color;

	// Manage the diffuse color
	struct aiColor4D diffuseColor;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
	{
		color = Color(diffuseColor.r,diffuseColor.g, diffuseColor.b, diffuseColor.a);
		assimpMesh->AddMaterial(DIFFUSE_MATERIAL, color);
	}
	else
	{
		color = Color(1,1,1,1);
		assimpMesh->AddMaterial(DIFFUSE_MATERIAL, color);
	}

	// Manage the specular color
	struct aiColor4D specularColor;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specularColor))
	{
		color = Color(specularColor.r,specularColor.g, specularColor.b, 0);
		// Try to extract Specular shininess
		float shininess, strength;
		unsigned int max = 1;
		if(aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max) == AI_SUCCESS &&
		   aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max) == AI_SUCCESS)
		{
			color.A = strength*shininess;
		}
		assimpMesh->AddMaterial(SPECULAR_MATERIAL, color);
	}
	else
	{
		color = Color(1,1,1,0);
		assimpMesh->AddMaterial(SPECULAR_MATERIAL, color);
	}

	// Manage the Ambiant color
	struct aiColor4D ambiantColor;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambiantColor))
	{
		color = Color(ambiantColor.r,ambiantColor.g, ambiantColor.b, ambiantColor.a);
		assimpMesh->AddMaterial(AMBIANT_MATERIAL, color);
	}
	else
	{
		color = Color(1,1,1,1);
		assimpMesh->AddMaterial(AMBIANT_MATERIAL, color);
	}

	// Manage emission color
	struct aiColor4D emissionColor;
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emissionColor))
	{
		color = Color(emissionColor.r,emissionColor.g, emissionColor.b, emissionColor.a);
		assimpMesh->AddMaterial(EMISSION_MATERIAL, color);
	}
	else
	{
		color = Color(1,1,1,1);
		assimpMesh->AddMaterial(EMISSION_MATERIAL, color);
	}

}

void AssimpLoader::BuildGroup(IMeshSceneNode* meshSceneNode, const aiScene* scene, aiNode* nd, const Math::CMatrix4& matrixUpp)
{
	// Load transformation
	struct aiMatrix4x4 m = nd->mTransformation;
	//aiTransposeMatrix4(&m);
	Math::CMatrix4 matrixLocalTrans = Math::CMatrix4(m.a1,m.a2,m.a3,m.a4,
										   m.b1,m.b2,m.b3,m.b4,
										   m.c1,m.c2,m.c3,m.c4,
										   m.d1,m.d2,m.d3,m.d4);
	Math::CMatrix4 globalTransformation = matrixLocalTrans*matrixUpp;

	// draw all meshes assigned to this node
	for (unsigned int n = 0; n < nd->mNumMeshes; ++n) {
		/*
		 * Construct the Object
		 */
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		if(mesh->mNumFaces == 0)
		{
			Logger::Log() << "[INFO] Skip assimp mesh. No faces. \n";
			continue;
		}
		RenderableObject* assimpMesh = new RenderableObject;
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
		RenderableObject::RenderableBuffer buffer;
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

		// Chargement des materiaux
		GetMaterialPropreties(assimpMesh, scene->mMaterials[mesh->mMaterialIndex]);
		// Check if is an Instance
		bool foundInstance = false;
		//TODO: Reactivativate instance system
//		for(CachedAssimpMeshMap::iterator it = m_cached_geom.begin(); it != m_cached_geom.end(); it++)
//		{
//			if((*it)->IsInstance(*assimpMesh))
//			{
//				(*it)->SetInstance(*assimpMesh);
//				foundInstance = true;
//				break;
//			}
//		}

		if(foundInstance)
		{
			Logger::Log() << "[INFO] Found an Instance ... \n";
			Assert(false);
		}
		else
		{
			// Compile all buffers
			Logger::Log() << "[INFO] Compile all buffers ... \n";
			assimpMesh->CompileBuffers();
			Logger::Log() << "[INFO] Add cached resources : " << nd->mMeshes[n] << "\n";
			m_cached_geom.push_back(assimpMesh);
		}
		// Attach to group
		Logger::Log() << "[INFO] Add to father node... \n";

		meshSceneNode->AddRenderableObject(assimpMesh, globalTransformation);
	}

	// Add all childrens
	for (int n = 0; n < nd->mNumChildren; ++n) {
		BuildGroup(meshSceneNode, scene, nd->mChildren[n], globalTransformation);
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
