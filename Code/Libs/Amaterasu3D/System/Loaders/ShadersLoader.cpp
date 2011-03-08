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
		TextureType type;
		//TODO: Faire une factory ???
		if(typeAttrib == "Diffuse")
		{
			Logger::Log() << "   * Texture : " << nameAttrib << " (Diffuse) \n";
			type = DIFFUSE_TEXTURE;
		}
		else
		{
			throw CException("Unknow attribut : "+typeAttrib);
		}
		shader->addTextureUnit(type, nameAttrib);
		textureNode = textureNode->NextSiblingElement("Attribut");
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
	Logger::Log() << "[INFO] Load shader : " << name << " ... \n";
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
	glShader* shader = glShaderManager::Instance().loadfromFile(vertexShadername.c_str(),fragmentShadername.c_str());
	// Attrib blinding ...
	LoadShaderAttributs(shader, root);
	// Textures attributs
	LoadShaderTextures(shader, root);
	// Update all bindings
	glShaderManager::Instance().Push(shader);
	shader->updateAttributBlinding();
	shader->updateTextureUnitsBlinding();
	glShaderManager::Instance().Pop();
	return shader;
}
