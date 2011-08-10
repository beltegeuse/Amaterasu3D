#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <Math/Matrix4.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/SceneNode/SimpleRenderable/Rectangle2D.h>
#include <Addons/FPS/FPS.h>
#include <tinyxml.h>
#include <TinyXMLHelper.h>
#include <Debug/Exceptions.h>
class NewFont
{
private:
	class NewFontCharacter
	{
	private:
		/*
		 * Attributes
		 */
		// Char description
		int m_ID;
		ama3D::Math::TVector2I m_Size;
		ama3D::Math::TVector2I m_Position;
		ama3D::Math::CVector2<double> m_Offset;
		double m_xadvance;
		// Char drawable
		ama3D::RenderableObject m_Buffer;
	public:
		NewFontCharacter(TiXmlElement* element, const ama3D::Math::TVector2F & texSize)
		{
			// Get element definition
			TinyXMLGetAttributeValue<int>(element,"id",&m_ID);
			TinyXMLGetAttributeValue<int>(element,"width",&m_Size.x);
			TinyXMLGetAttributeValue<int>(element,"height",&m_Size.y);
			TinyXMLGetAttributeValue<int>(element,"x",&m_Position.x);
			TinyXMLGetAttributeValue<int>(element,"y",&m_Position.y);
			TinyXMLGetAttributeValue<double>(element,"xoffset",&m_Offset.x);
			TinyXMLGetAttributeValue<double>(element,"yoffset",&m_Offset.y);
			TinyXMLGetAttributeValue<double>(element,"xadvance",&m_xadvance);

			// Fill buffer
			float* vertexBuffer = new float[8];
			// TODO: See how to do
			vertexBuffer[0] = 0;
			vertexBuffer[1] = 0;
			vertexBuffer[2] = 0;
			vertexBuffer[3] = m_Size.y;
			vertexBuffer[4] = m_Size.x;
			vertexBuffer[5] = m_Size.y;
			vertexBuffer[6] = m_Size.x;
			vertexBuffer[7] = 0;
			ama3D::RenderableObject::RenderableBuffer buffer;
			buffer.buffer = vertexBuffer;
			buffer.dimension = 2;
			buffer.size = 8;
			buffer.owner = true;
			m_Buffer.AddBuffer(buffer, ama3D::VERTEX_ATTRIBUT);
			// Create Texcoord buffer
			float* uvBuffer = new float[8];
			uvBuffer[0] = (m_Position.x/texSize.x);
			uvBuffer[1] = 1 - (m_Position.y/texSize.y);
			uvBuffer[2] = (m_Position.x/texSize.x);
			uvBuffer[3] = 1 - ((m_Position.y+m_Size.y)/texSize.y);
			uvBuffer[4] = ((m_Position.x+m_Size.x)/texSize.x);
			uvBuffer[5] = 1 - ((m_Position.y+m_Size.y)/texSize.y);
			uvBuffer[6] = ((m_Position.x+m_Size.x)/texSize.x);
			uvBuffer[7] = 1 - (m_Position.y/texSize.y);
			buffer.buffer = uvBuffer;
			m_Buffer.AddBuffer(buffer, ama3D::TEXCOORD_ATTRIBUT);
			// Create Index buffer
			unsigned int* indiceBuffer = new unsigned int[6];
			indiceBuffer[0] = 0;
			indiceBuffer[1] = 1;
			indiceBuffer[2] = 2;
			indiceBuffer[3] = 0;
			indiceBuffer[4] = 2;
			indiceBuffer[5] = 3;
			m_Buffer.SetIndiceBuffer(indiceBuffer, 6);
			m_Buffer.CompileBuffers();
		}

		// Accesseurs
		int GetID() { return m_ID; }
		const ama3D::Math::TVector2I& GetSize() { return m_Size; }
		int GetXStep() { return m_xadvance; }
		void Render()
		{
			m_Buffer.Draw();
		}
	};
public: // TODO: Need to be private
	/*
	 * Attributes
	 */
	ama3D::TTexturePtr m_FontsTex;
	std::map<int, NewFontCharacter*> m_Characteres;
	ama3D::TShaderPtr m_FontShader;
	/*
	 * Private methods
	 */
	void LoadFile(const ama3D::CFile& file)
	{
		ama3D::Logger::Log() << "[INFO] Load Font file : " << file.Fullname() << "\n";
		TiXmlDocument doc(file.Fullname().c_str());
		if (!doc.LoadFile())
		{
			ama3D::Logger::Log() << "[ERROR] TinyXML error : " << doc.ErrorDesc()
					<< "\n";
			throw ama3D::CLoadingFailed(file.Fullname(), "unable to load xml with TinyXML");
		}

		TiXmlHandle hdl(&doc);
		TiXmlElement *fontNode = hdl.FirstChild("Font").ToElement();

		std::string fileNameTex;
		int texWidth, texHeight;
		TinyXMLGetAttributeValue<std::string>(fontNode,"filename",&fileNameTex);
		TinyXMLGetAttributeValue<int>(fontNode,"width",&texWidth);
		TinyXMLGetAttributeValue<int>(fontNode,"height",&texHeight);

		ama3D::Logger::Log() << "      * Load Texture : " << fileNameTex<< "\n";
		m_FontsTex = ama3D::Texture::LoadFromFile(fileNameTex);

		ama3D::Logger::Log() << "      * Load Fonts ... ";
		TiXmlElement *facesNode = fontNode->FirstChildElement("Faces");
		TiXmlElement *faceNode = facesNode->FirstChildElement("Face");
		int i = 0;
		while(faceNode)
		{
			NewFontCharacter * face = new NewFontCharacter(faceNode, ama3D::Math::TVector2F(texWidth, texHeight));
			m_Characteres[face->GetID()] = face;
			faceNode = faceNode->NextSiblingElement();
			i++;
		}
		ama3D::Logger::Log() << i << " Font loaded ! \n";
	}
public:
	/*
	 * Constructors & Destructors
	 */
	NewFont(const std::string& filename)
	{
		ama3D::CFile file = ama3D::CMediaManager::Instance().FindMedia(filename);
		LoadFile(file);

		// Shader loading ...
		m_FontShader = ama3D::CShaderManager::Instance().LoadShader("2DDrawFont.shader");
	}

	void Render(char * text, float x, float y)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_FontShader->Begin();
		m_FontShader->SetUniform1i("FontEffectType", 1);
		m_FontShader->SetUniformVector("FontColor", ama3D::Math::TVector4F(0.0,0.0,1.0,1.0));
		m_FontShader->SetUniformVector("FontEffectColor", ama3D::Math::TVector4F(1.0,0.0,0.0,1.0));
		m_FontsTex->activateMultiTex(ama3D::CUSTOM_TEXTURE+0);
		char* ch;
		std::map<int, NewFontCharacter*>::const_iterator element;
		NewFontCharacter* currentElement;
		ama3D::Math::CMatrix4 transMat;
		float xOff = 0.0;
		for (ch = text; *ch; ch++)
		{
			element = m_Characteres.find((int)*ch);
			if(element == m_Characteres.end())
				currentElement = m_Characteres[32]; // Not found
			else
				currentElement = element->second;
			transMat.SetTranslation(x+xOff,y,0);
			ama3D::CMatrixManager::Instance().PushMatrix(transMat);
			currentElement->Render();
			ama3D::CMatrixManager::Instance().PopMatrix();
			xOff += currentElement->GetXStep();
		}
		m_FontsTex->desactivateMultiTex(ama3D::CUSTOM_TEXTURE+0);
		m_FontShader->End();
		glDisable(GL_BLEND);
	}

};

class TestCustomAttributs : public ama3D::Application
{
protected:
	/*
	 * Attributes
	 */
	ama3D::CameraFPS* m_Camera;
	ama3D::TShaderPtr m_ShaderTest;
	ama3D::Rectangle2D* m_Plane;
	NewFont* m_Font;
public:
	TestCustomAttributs()
	{
	}

	virtual ~TestCustomAttributs()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new ama3D::CameraFPS(ama3D::Math::TVector3F(30,40,20), ama3D::Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Create Font
		m_Font = new NewFont("Impact.ttf.xml");
		m_Plane = new ama3D::Rectangle2D(ama3D::Math::TVector2I(0, 0),ama3D::Math::TVector2I(800,600));
		m_Plane->GetObject().AddTextureMap(ama3D::DIFFUSE_TEXTURE,m_Font->m_FontsTex);
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
		if(event.Type == C3::Event::KeyPressed)
		{
			 switch(event.Key.Code)
			 {
				 case C3::Key::F1:
					 glEnable(GL_ALPHA_TEST);
					 break;
				 case C3::Key::F2:
					 glDisable(GL_ALPHA_TEST);
					 break;
			 }
		}
	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(ama3D::MATRIX_3D);

		MatrixManager.SetModeMatrix(ama3D::MATRIX_2D);
		m_Font->Render("Usually, an application wants to load a glyph image", 100,100);
	}
};

//#ifdef WIN32
//#include <windows.h>
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//#else
int main(int argc, char *argv[])
//#endif
{

	ama3D::CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	ama3D::CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
	std::cout << "[INFO] Begin ..." << std::endl;
	TestCustomAttributs window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;


	return 0;
}
