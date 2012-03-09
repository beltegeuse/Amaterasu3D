#ifndef GRAPHICSDEVICE_H_
#define GRAPHICSDEVICE_H_

#include <Exceptions.h>

namespace ama3D
{
struct GraphicsDeviceException: public CException
{
	GraphicsDeviceException(const std::string& reason) :
			CException("[GrapicsDevice] " + reason)
	{
	}
};

class GraphicsDevice
{
public:
	/*
	 * Constructors & destructors
	 */
	GraphicsDevice();
	virtual ~GraphicsDevice();

	/*
	 * Public methods
	 */
	void CheckSystem();
};

}

#endif /* GRAPHICSDEVICE_H_ */
