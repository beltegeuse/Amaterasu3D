#ifndef LOGO_H_
#define LOGO_H_

#include <Graphics/Texture.h>
#include <Graphics/SceneGraph/Rectangle2D.h>
#include <Graphics/Shaders/Shader.h>
class Logo {
private:
	/*
	 * Attributes
	 */
	Rectangle2D* m_Rectangle;
	TShaderPtr m_2DShader;
public:
	/*
	 * Constructors and destructors
	 */
	Logo();
	virtual ~Logo();

	virtual void Draw();
};

#endif /* LOGO_H_ */
