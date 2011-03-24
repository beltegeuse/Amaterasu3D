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
#include "ShadersLoader.h"
#include <tinyxml.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>

ShadersLoader::ShadersLoader()
{
}

ShadersLoader::~ShadersLoader()
{
}

void ShadersLoader::LoadMaterials(glShader* shader, TiXmlElement *root)
{
	TiXmlElement *rootMaterial = root->FirstChildElement("Materials");
	if(!rootMaterial)
	{
		Logger::Log() << "[INFO] No Materials is available ... \n";
		return;
	}
	TiXmlElement *materialNode = rootMaterial->FirstChildElement("Material");
	while(materialNode)
	{
		std::string name = std::string(materialNode->Attribute("name"));
		std::string type = std::string(materialNode->Attribute("type"));
		if(type == "Diffuse")
		{
			shader->addMaterialBinding(DIFFUSE_MATERIAL, name);
		}
		else if(type == "Specular")
		{
			shader->addMaterialBinding(SPECULAR_MATERIAL, name);
		}
		else if(type == "Ambiant")
		{
			shader->addMaterialBinding(AMBIANT_MATERIAL, name);
		}
		else if(type == "Emissive")
		{
			shader->addMaterialBinding(EMISSION_MATERIAL, name);
		}
		else
		{
			throw CException("invalid material type");
		}
		materialNode = materialNode->NextSiblingElement();
	}
}

void ShadersLoader::LoadShaderFBO(glShader* shader, TiXmlElement *root)
{
	TiXmlElement *rootFBO = root->FirstChildElement("OutputFrame");
	if(!rootFBO)
	{
		Logger::Log() << "[INFO] No FBO is available ... \n";
		return;
	}
	// Get the depth buffer type
	std::string typeDepthString = std::string(rootFBO->Attribute("depthType"));
	Logger::Log() << "   * Depth buffer type " << typeDepthString << "\n";
	FBODepthType typeDepth;
	if(typeDepthString == "None")
	{
		typeDepth = FBODEPTH_NONE;
	}
	else if(typeDepthString == "RenderTarget")
	{
		typeDepth = FBODEPTH_RENDERTARGET;
	}
	else if(typeDepthString == "Texture")
	{
		typeDepth = FBODEPTH_TEXTURE;
	}
	else
		throw CException("invalid buffer type");
	// Get all buffers
	std::map<std::string, FBOTextureBufferParam> buffers;
	Logger::Log() << "   * Chargement des differents buffers .... \n";
	TiXmlElement *frameNode = rootFBO->FirstChildElement("Frame");
	shader->begin();
//	int i = 0;
	while(frameNode)
	{
		std::string name = std::string(frameNode->Attribute("name"));
		std::string typeString = std::string(frameNode->Attribute("type"));
		Logger::Log() << "      * Create buffer : " << name << " ( " << typeString << " ) \n";
		FBOTextureBufferParam param;
		if(typeString == "RGBA")
		{
			// Nothing to do
		}
		else if(typeString == "RGB16")
		{
			param.InternalFormat = GL_RGB16F;
			param.ExternalFormat = GL_RGB;
		}
		else if(typeString == "RGB32")
		{
			param.InternalFormat = GL_RGB32F;
			param.ExternalFormat = GL_RGB;
		}
		else if(typeString == "RGBA16")
		{
			param.InternalFormat = GL_RGBA16F;
			param.ExternalFormat = GL_RGBA;
		}
		else if(typeString == "RGBA32")
		{
			param.InternalFormat = GL_RGBA32F;
			param.ExternalFormat = GL_RGBA;
		}
		else
			throw CException("unknow buffer type");
//		glBindFragDataLocation(shader->GetProgramObject(),i,name.c_str());
		param.Attachment = glGetFragDataLocation(shader->GetProgramObject(),name.c_str());
		Logger::Log() << "           * Attachment : " << param.Attachment << "\n";
		buffers[name] = param;
		frameNode = frameNode->NextSiblingElement("Frame");
//		i++;
	}
	shader->end();
	FBODepthBufferParam bufferDepth;
	FBO* fbo = new FBO(Math::TVector2I(600,800), buffers, typeDepth, bufferDepth); // FIXME
	shader->SetFBO(fbo);
}

void ShadersLoader::LoadShaderMatrix(glShader* shader, TiXmlElement *root)
{
	TiXmlElement *rootMatrix = root->FirstChildElement("MatrixInput");
	if(!rootMatrix)
	{
		Logger::Log() << "[INFO] No Matrix is available ... \n";
		return;
	}
	TiXmlElement *matrixNode = rootMatrix->FirstChildElement("Matrix");
	Logger::Log() << "[INFO] Matrix : \n";
	while(matrixNode)
	{
		//TODO: Faire les exceptions si attributs absent
		std::string nameMatrix = std::string(matrixNode->Attribute("name"));
		std::string typeMatrix= std::string(matrixNode->Attribute("type"));
		MatrixType type;
		//TODO: Faire une factory ???
		if(typeMatrix == "Model")
		{
			Logger::Log() << "   * Matrix : " << nameMatrix << " (Model) \n";
			type = MODEL_MATRIX;
		}
		else if(typeMatrix == "Projection")
		{
			Logger::Log() << "   * Matrix : " << nameMatrix << " (Projection) \n";
			type = PROJECTION_MATRIX;
		}
		else if(typeMatrix == "Normal")
		{
			Logger::Log() << "   * Matrix : " << nameMatrix << " (Normal) \n";
			type = NORMAL_MATRIX;
		}
		else if(typeMatrix == "View")
		{
			Logger::Log() << "   * Matrix : " << nameMatrix << " (View) \n";
			type = VIEW_MATRIX;
		}
		else
		{
			throw CException("Unknow Matrix : "+typeMatrix);
		}
		shader->addMatrixBinding(type, nameMatrix);
		matrixNode = matrixNode->NextSiblingElement("Matrix");
	}
}

void ShadersLoader::LoadShaderAttributs(glShader* shader, TiXmlElement *root)
{
	TiXmlElement *rootAttributs = root->FirstChildElement("Attributs");
	if(!rootAttributs)
	{
		Logger::Log() << "[INFO] No Attribut is available ... \n";
		return;
	}
	TiXmlElement *attributNode = rootAttributs->FirstChildElement("Attribut");
	Logger::Log() << "[INFO] Attribut : \n";
	while(attributNode)
	{
		//TODO: Faire les exceptions si attributs absent
		std::string nameAttrib = std::string(attributNode->Attribute("name"));
		std::string typeAttrib = std::string(attributNode->Attribute("type"));
		ShaderAttributType type;
		//TODO: Faire une factory ???
		if(typeAttrib == "Vertex")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (Vertex) \n";
			type = VERTEX_ATTRIBUT;
		}
		else if(typeAttrib == "Color")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (Color) \n";
			type = COLOR_ATTRIBUT;
		}
		else if(typeAttrib == "TexCoord")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (TexCoord) \n";
			type = TEXCOORD_ATTRIBUT;
		}
		else if(typeAttrib == "Tangent")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (Tangent) \n";
			type = TANGENT_ATTRIBUT;
		}
		else if(typeAttrib == "BiTangent")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (BiTangent) \n";
			type = BITANGENT_ATTRIBUT;
		}
		else if(typeAttrib == "Normal")
		{
			Logger::Log() << "   * Attribut : " << nameAttrib << " (Normal) \n";
			type = NORMAL_ATTRIBUT;
		}
		else
		{
			throw CException("Unknow attribut : "+typeAttrib);
		}
		shader->addAttributBlinding(type, nameAttrib);
		attributNode = attributNode->NextSiblingElement("Attribut");
	}
}

void ShadersLoader::LoadShaderTextures(glShader* shader, TiXmlElement *root)
{
	TiXmlElement *rootTextures = root->FirstChildElement("Textures");
	if(!rootTextures)
	{
		Logger::Log() << "[INFO] No Textures is available ... \n";
		return;
	}
	TiXmlElement *textureNode = rootTextures->FirstChildElement("Texture");
	Logger::Log() << "[INFO] Textures : \n";
	while(textureNode)
	{
		//TODO: Faire les exceptions si attributs absent
		std::string nameAttrib = std::string(textureNode->Attribute("name"));
		std::string typeAttrib = std::string(textureNode->Attribute("type"));
		int typeID;
	    int id;
		//TODO: Faire une factory ???
		if(typeAttrib == "Diffuse")
		{
			Logger::Log() << "   * Texture : " << nameAttrib << " (Diffuse) \n";
			typeID = DIFFUSE_TEXTURE;
		}
		else if(typeAttrib == "Normal")
		{
			Logger::Log() << "   * Texture : " << nameAttrib << " (Normal) \n";
			typeID = NORMAL_TEXTURE;
		}
		else if(typeAttrib == "Specular")
		{
			Logger::Log() << "   * Texture : " << nameAttrib << " (Specular) \n";
			typeID = SPECULAR_TEXTURE;
		}
		else if(typeAttrib == "Custom")
		{
			textureNode->Attribute("id",&id);
			Logger::Log() << "   * Texture : " << nameAttrib << " (Custom) [" << id << "]\n";
			typeID = CUSTOM_TEXTURE+id;
		}
		else
		{
			throw CException("Unknow attribut : "+typeAttrib);
		}
		shader->addTextureUnit(typeID, nameAttrib);
		textureNode = textureNode->NextSiblingElement("Texture");
	}
}

glShader* ShadersLoader::LoadFromFile(const std::string& Filename)
{
	TiXmlDocument doc( Filename.c_str() );
	if(!doc.LoadFile())
	{
		Logger::Log() << "[ERROR] TinyXML error : " <<  doc.ErrorDesc() << "\n";
		throw CLoadingFailed(Filename, "unable to load xml with TinyXML");
	}
	// Get the root
	TiXmlHandle hdl(&doc);
	TiXmlElement *root = hdl.FirstChild("Shader").Element();
	// Problem to find the root
	if(!root)
	{
		throw CLoadingFailed(Filename, "unable to find root (Shader)");
	}
	// Get the shader name and display it
	std::string name = std::string(root->Attribute("name"));
	std::string shaderTypeName = std::string(root->Attribute("type"));
	Logger::Log() << "[INFO] Load shader : " << name << " ( " << shaderTypeName << " ) ... \n";
	ShaderType shaderType;
	if(shaderTypeName == "Basic")
		shaderType = BASIC_SHADER;
	else if(shaderTypeName == "GBuffer")
		shaderType = GBUFFER_SHADER;
	else
		throw CException("unknow shader type");

	// Get the 2 files name
	// * Vertex shader
	TiXmlElement *shadername = root->FirstChildElement("VertexShader");
	if(!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string vertexShadername = std::string(shadername->Attribute("filename"));
	Logger::Log() << "   * Vertex shader : " << vertexShadername << "\n";
	// * Fragment shader
	shadername = root->FirstChildElement("FragmentShader");
	if(!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string fragmentShadername = std::string(shadername->Attribute("filename"));
	Logger::Log() << "   * Fragment shader : " << fragmentShadername << "\n";

	vertexShadername = CMediaManager::Instance().FindMedia(vertexShadername).Fullname();
	fragmentShadername = CMediaManager::Instance().FindMedia(fragmentShadername).Fullname();
	// Shader creation ....
	glShader* shader = glShaderManager::Instance().loadfromFile(vertexShadername.c_str(),fragmentShadername.c_str(), shaderType);
	// Attrib blinding ...
	LoadShaderAttributs(shader, root);
	// Textures uniform
	LoadShaderTextures(shader, root);
	// Matrix uniform
	LoadShaderMatrix(shader, root);
	// FBO
	LoadShaderFBO(shader, root);
	// Materials
	LoadMaterials(shader, root);
	// Update all bindings
	// * Warning : Need to relink after
	shader->UpdateAll();
	return shader;
}
