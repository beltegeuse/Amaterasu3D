#ifndef LOGO_H_
#define LOGO_H_

#include <Graphics/Texture.h>

class Logo {
private:
	/*
	 * Attributes
	 */
	TTexturePtr m_Logo; ///< Represent the Amaterasu3d logo

public:
	/*
	 * Constructors and destructors
	 */
	Logo();
	virtual ~Logo();

	virtual void Draw();
};

#endif /* LOGO_H_ */
