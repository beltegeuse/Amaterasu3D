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
#include "ShadersLoader.h"
#include <tinyxml.h>
#include <iostream> //#include "Logger.h"
#include <System/MediaManager.h>
#include <TinyXMLHelper.h>
#include <System/SettingsManager.h>

namespace ama3D
{
ShadersLoader::ShadersLoader()
{
}

ShadersLoader::~ShadersLoader()
{
}

void ShadersLoader::LoadMaterials(Shader* shader, TiXmlElement *root)
{
	TiXmlElement *rootMaterial = root->FirstChildElement("Materials");
	if (!rootMaterial)
	{
		std::cout << "[INFO] No Materials is available ... \n";
		return;
	}
	TiXmlElement *materialNode = rootMaterial->FirstChildElement("Material");
	while (materialNode)
	{
		std::string name;
		std::string type;
		TinyXMLGetAttributeValue<std::string>(materialNode, "name", &name);
		TinyXMLGetAttributeValue<std::string>(materialNode, "type", &type);
		if (type == "Diffuse")
		{
			shader->AddMaterialBinding(DIFFUSE_MATERIAL, name);
		}
		else if (type == "Specular")
		{
			shader->AddMaterialBinding(SPECULAR_MATERIAL, name);
		}
		else if (type == "Ambiant")
		{
			shader->AddMaterialBinding(AMBIANT_MATERIAL, name);
		}
		else if (type == "Emissive")
		{
			shader->AddMaterialBinding(EMISSION_MATERIAL, name);
		}
		else
		{
			throw CException("invalid material type");
		}
		materialNode = materialNode->NextSiblingElement();
	}
}

void ShadersLoader::LoadShaderFBO(Shader* shader, TiXmlElement *root)
{
	TiXmlElement *rootFBO = root->FirstChildElement("OutputFrame");
	if (!rootFBO)
	{
		std::cout << "[INFO] No FBO is available ... \n";
		return;
	}
	// Get the depth buffer type
	std::string typeDepthString;
	TinyXMLGetAttributeValue<std::string>(rootFBO, "depthType",
			&typeDepthString);
	std::cout << "   * Depth buffer type " << typeDepthString << "\n";
	FBODepthType typeDepth;
	if (typeDepthString == "None")
	{
		typeDepth = FBODEPTH_NONE;
	}
	else if (typeDepthString == "RenderTarget")
	{
		typeDepth = FBODEPTH_RENDERTARGET;
	}
	else if (typeDepthString == "Texture")
	{
		typeDepth = FBODEPTH_TEXTURE;
	}
	else
		throw CException("invalid buffer type");
	// Get the size of buffer
	std::string typeSize;
	int X = 0;
	int Y = 0;
	TinyXMLGetAttributeValue<std::string>(rootFBO, "size", &typeSize);
	if (typeSize == "Screen")
	{
		glm::ivec2 sizeScreen =
				CSettingsManager::Instance().GetSizeRenderingWindow();
		X = sizeScreen.x;
		Y = sizeScreen.y;
	}
	else if (typeSize == "HalfScreen")
	{
		glm::ivec2 sizeScreen =
				CSettingsManager::Instance().GetSizeRenderingWindow();
		X = sizeScreen.x / 2;
		Y = sizeScreen.y / 2;
	}
	else if (typeSize == "Custom")
	{
		TinyXMLGetAttributeValue<int>(rootFBO, "width", &X);
		TinyXMLGetAttributeValue<int>(rootFBO, "height", &Y);
	}
	else
	{
		throw CException("unknow type of size");
	}
	// Get all buffers
	std::map<std::string, FBOTextureBufferParam> buffers;
	std::cout << "   * Chargement des differents buffers .... \n";
	TiXmlElement *frameNode = rootFBO->FirstChildElement("Frame");
	shader->Begin();
	while (frameNode)
	{
		std::string name;
		std::string typeString;
		TinyXMLGetAttributeValue<std::string>(frameNode, "name", &name);
		TinyXMLGetAttributeValue<std::string>(frameNode, "type", &typeString);
		std::cout << "      * Create buffer : " << name << " ( "
				<< typeString << " ) \n";
		FBOTextureBufferParam param;
		// Mipmapping parsing

		bool mipmapping = false;
		frameNode->Attribute("mipmapping", &mipmapping);
		if (mipmapping)
		{
			std::cout << "        * Mipmapping enable \n";
			param.GenerateMipMapping = true;
			param.MinFiltering = GL_LINEAR_MIPMAP_NEAREST;
			param.MaxFiltering = GL_LINEAR;
		}

		bool bilinear = false;
		frameNode->Attribute("bilinear", &bilinear);
		if(bilinear)
		{
			std::cout << "        * Bilinear enable \n";
			param.MinFiltering = GL_LINEAR;
			param.MaxFiltering = GL_LINEAR;
		}

		if(bilinear && mipmapping)
			throw CException("Impossible to activate Bilinear & mipmapping on the same texture");

		// Parse the Image format
		if (typeString == "RGBA")
		{
			// Nothing to do
		}
		else if (typeString == "RGB16")
		{
			param.InternalFormat = GL_RGB16F;
			param.ExternalFormat = GL_RGB;
		}
		else if (typeString == "RGB32")
		{
			param.InternalFormat = GL_RGB32F;
			param.ExternalFormat = GL_RGB;
		}
		else if (typeString == "RGBA16")
		{
			param.InternalFormat = GL_RGBA16F;
			param.ExternalFormat = GL_RGBA;
		}
		else if (typeString == "RGBA32")
		{
			param.InternalFormat = GL_RGBA32F;
			param.ExternalFormat = GL_RGBA;
		}
		else if (typeString == "L")
		{
			param.InternalFormat = GL_LUMINANCE32F_ARB;
			param.ExternalFormat = GL_LUMINANCE;
		}
		else
			throw CException("unknow buffer type");
		param.Attachment = glGetFragDataLocation(shader->GetProgramObject(),
				name.c_str());
		std::cout << "           * Attachment : " << param.Attachment
				<< "\n";
		buffers[name] = param;
		frameNode = frameNode->NextSiblingElement("Frame");
	}
	shader->End();
	FBODepthBufferParam bufferDepth;
	FBO* fbo = new FBO(glm::ivec2(X, Y), buffers, typeDepth, bufferDepth); // FIXME: Inversion des tailles ???
	shader->SetFBO(fbo);
}

void ShadersLoader::LoadShaderMatrix(Shader* shader, TiXmlElement *root)
{
	TiXmlElement *rootMatrix = root->FirstChildElement("MatrixInput");
	if (!rootMatrix)
	{
		std::cout << "[INFO] No Matrix is available ... \n";
		return;
	}
	TiXmlElement *matrixNode = rootMatrix->FirstChildElement("Matrix");
	std::cout << "[INFO] Matrix : \n";
	while (matrixNode)
	{
		std::string nameMatrix;
		std::string typeMatrix;
		TinyXMLGetAttributeValue<std::string>(matrixNode, "name", &nameMatrix);
		TinyXMLGetAttributeValue<std::string>(matrixNode, "type", &typeMatrix);
		MatrixType type;
		//TODO: Faire une factory ???
		if (typeMatrix == "Model")
		{
			std::cout << "   * Matrix : " << nameMatrix << " (Model) \n";
			type = MODEL_MATRIX;
		}
		else if (typeMatrix == "Projection")
		{
			std::cout << "   * Matrix : " << nameMatrix
					<< " (Projection) \n";
			type = PROJECTION_MATRIX;
		}
		else if (typeMatrix == "Normal")
		{
			std::cout << "   * Matrix : " << nameMatrix << " (Normal) \n";
			type = NORMAL_MATRIX;
		}
		else if (typeMatrix == "View")
		{
			std::cout << "   * Matrix : " << nameMatrix << " (View) \n";
			type = VIEW_MATRIX;
		}
		else
		{
			throw CException("Unknow Matrix : " + typeMatrix);
		}
		shader->AddMatrixBinding(type, nameMatrix);
		matrixNode = matrixNode->NextSiblingElement("Matrix");
	}
}

void ShadersLoader::LoadShaderAttributs(Shader* shader, TiXmlElement *root)
{
	TiXmlElement *rootAttributs = root->FirstChildElement("Attributs");
	if (!rootAttributs)
	{
		std::cout << "[INFO] No Attribut is available ... \n";
		return;
	}
	TiXmlElement *attributNode = rootAttributs->FirstChildElement("Attribut");
	std::cout << "[INFO] Attribut : \n";
	while (attributNode)
	{
		std::string nameAttrib;
		std::string typeAttrib;
		TinyXMLGetAttributeValue<std::string>(attributNode, "name",
				&nameAttrib);
		TinyXMLGetAttributeValue<std::string>(attributNode, "type",
				&typeAttrib);
		int typeID;
		//TODO: Faire une factory ???
		if (typeAttrib == "Vertex")
		{
			std::cout << "   * Attribut : " << nameAttrib << " (Vertex) \n";
			typeID = VERTEX_ATTRIBUT;
		}
		else if (typeAttrib == "Color")
		{
			std::cout << "   * Attribut : " << nameAttrib << " (Color) \n";
			typeID = COLOR_ATTRIBUT;
		}
		else if (typeAttrib == "TexCoord")
		{
			std::cout << "   * Attribut : " << nameAttrib
					<< " (TexCoord) \n";
			typeID = TEXCOORD_ATTRIBUT;
		}
		else if (typeAttrib == "Tangent")
		{
			std::cout << "   * Attribut : " << nameAttrib
					<< " (Tangent) \n";
			typeID = TANGENT_ATTRIBUT;
		}
		else if (typeAttrib == "BiTangent")
		{
			std::cout << "   * Attribut : " << nameAttrib
					<< " (BiTangent) \n";
			typeID = BITANGENT_ATTRIBUT;
		}
		else if (typeAttrib == "Normal")
		{
			std::cout << "   * Attribut : " << nameAttrib << " (Normal) \n";
			typeID = NORMAL_ATTRIBUT;
		}
		else if(typeAttrib == "Custom")
		{
			// TODO: Add Limit number & do verifications
			int idNum;
			TinyXMLGetAttributeValue<int>(attributNode, "id",&idNum);
			std::cout << "   * Attribut : " << nameAttrib << " (Custom " << idNum << ") \n";
			typeID = CUSTOM_ATTRIBUT + idNum;
		}
		else
		{
			throw CException("Unknow attribut : " + typeAttrib);
		}
		shader->AddAttributBinding(typeID, nameAttrib);
		attributNode = attributNode->NextSiblingElement("Attribut");
	}
}

void ShadersLoader::LoadShaderTextures(Shader* shader, TiXmlElement *root)
{
	TiXmlElement *rootTextures = root->FirstChildElement("Textures");
	if (!rootTextures)
	{
		std::cout << "[INFO] No Textures is available ... \n";
		return;
	}
	TiXmlElement *textureNode = rootTextures->FirstChildElement("Texture");
	std::cout << "[INFO] Textures : \n";
	while (textureNode)
	{
		std::string nameAttrib;
		std::string typeAttrib;
		TinyXMLGetAttributeValue<std::string>(textureNode, "name", &nameAttrib);
		TinyXMLGetAttributeValue<std::string>(textureNode, "type", &typeAttrib);
		int typeID;
		int id;
		//TODO: Faire une factory ???
		if (typeAttrib == "Diffuse")
		{
			std::cout << "   * Texture : " << nameAttrib << " (Diffuse) \n";
			typeID = DIFFUSE_TEXTURE;
		}
		else if (typeAttrib == "Normal")
		{
			std::cout << "   * Texture : " << nameAttrib << " (Normal) \n";
			typeID = NORMAL_TEXTURE;
		}
		else if (typeAttrib == "Specular")
		{
			std::cout << "   * Texture : " << nameAttrib
					<< " (Specular) \n";
			typeID = SPECULAR_TEXTURE;
		}
		else if (typeAttrib == "Custom")
		{
			textureNode->Attribute("id", &id);
			std::cout << "   * Texture : " << nameAttrib << " (Custom) ["
					<< id << "]\n";
			typeID = CUSTOM_TEXTURE + id;
		}
		else
		{
			throw CException("Unknow attribut : " + typeAttrib);
		}
		shader->AddTextureUnit(typeID, nameAttrib);
		textureNode = textureNode->NextSiblingElement("Texture");
	}
}

ShaderCompilerConfig ShadersLoader::LoadShaderCompilerConfig(TiXmlElement* root)
{
	ShaderCompilerConfig config;

	// Define extraction
	TiXmlElement *definesNode = root->FirstChildElement("Defines");
	if (definesNode)
	{
		TiXmlElement *defineNode = definesNode->FirstChildElement("Define");
		while (defineNode)
		{
			std::string nameAttrib;
			std::string type;
			std::string defaultValueAttrib;
			TinyXMLGetAttributeValue<std::string>(defineNode, "name", &nameAttrib);
			TinyXMLGetAttributeValue<std::string>(defineNode, "type", &type);
			TinyXMLGetAttributeValue<std::string>(defineNode, "defaultValue", &defaultValueAttrib);

			ShaderCompilerConfig::DefineEntry entry = ShaderCompilerConfig::GetDefineEntry(type, defaultValueAttrib);
			config.AddDefine(nameAttrib, entry);

			defineNode = defineNode->NextSiblingElement();
		}
	}

	return config;
}

Shader* ShadersLoader::LoadFromFile(const std::string& Filename)
{
	TiXmlDocument doc(Filename.c_str());
	if (!doc.LoadFile())
	{
		std::cout << "[ERROR] TinyXML error : " << doc.ErrorDesc() << "\n";
		throw CLoadingFailed(Filename, "unable to load xml with TinyXML");
	}
	// Get the root
	TiXmlHandle hdl(&doc);
	TiXmlElement *root = hdl.FirstChild("Shader").Element();
	// Problem to find the root
	if (!root)
	{
		throw CLoadingFailed(Filename, "unable to find root (Shader)");
	}
	// Get the shader name and display it
	std::string name;
	std::string shaderTypeName;
	TinyXMLGetAttributeValue<std::string>(root, "name", &name);
	TinyXMLGetAttributeValue<std::string>(root, "type", &shaderTypeName);
	std::cout << "[INFO] Load shader : " << name << " ( " << shaderTypeName
			<< " ) ... \n";
	ShaderType shaderType;
	if (shaderTypeName == "Basic")
		shaderType = BASIC_SHADER;
	else if (shaderTypeName == "GBuffer")
		shaderType = GBUFFER_SHADER;
	else
		throw CException("unknow shader type");

	// Load the shader compiler config
	ShaderCompilerConfig config = LoadShaderCompilerConfig(root);

	// Get the 2 files name
	// * Vertex shader
	TiXmlElement *shadername = root->FirstChildElement("VertexShader");
	if (!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string vertexShadername = std::string(
			shadername->Attribute("filename"));
	std::cout << "   * Vertex shader : " << vertexShadername << "\n";
	// * Fragment shader
	shadername = root->FirstChildElement("FragmentShader");
	if (!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string fragmentShadername = std::string(
			shadername->Attribute("filename"));
	std::cout << "   * Fragment shader : " << fragmentShadername << "\n";

	/*
	 * Find full path
	 */
	vertexShadername =
			CMediaManager::Instance().FindMedia(vertexShadername).Fullname();
	fragmentShadername =
			CMediaManager::Instance().FindMedia(fragmentShadername).Fullname();

	Shader* shader = 0;
	shadername = root->FirstChildElement("GeometryShader");
	if (shadername != 0)
	{
		std::string geometryShadername = std::string(
				shadername->Attribute("filename"));
		std::cout << "   * Geometry shader : " << geometryShadername
				<< "\n";
		geometryShadername = CMediaManager::Instance().FindMedia(
				geometryShadername).Fullname();

		std::string in, out;
		TinyXMLGetAttributeValue(shadername, "in", &in);
		TinyXMLGetAttributeValue(shadername, "out", &out);
		shader = CShaderManager::Instance().loadfromFile(
				vertexShadername.c_str(), fragmentShadername.c_str(),
				geometryShadername.c_str(), shaderType, config);
		shader->SetGeometryShaderParameters(OpenGLEnumFromString(in),
				OpenGLEnumFromString(out));
	}
	else
	{
		std::cout << "   * No Geometry shader\n";
		// Shader creation ....
		shader = CShaderManager::Instance().loadfromFile(
				vertexShadername.c_str(), fragmentShadername.c_str(),
				shaderType, config);
	}
	shader->Link();

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
}
