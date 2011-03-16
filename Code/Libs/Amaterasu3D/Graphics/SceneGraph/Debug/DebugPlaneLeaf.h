#ifndef DEBUGPLANELEAF_H_
#define DEBUGPLANELEAF_H_

#include <Graphics/SceneGraph/DrawObject.h>
#include <GL/glew.h>
#include <GL/gl.h>

namespace SceneGraph
{
	class DebugPlaneLeaf : public DrawObject
	{
	private:
		static GLfloat PlaneArray[24];
		static GLuint IndiceArray[6];
		unsigned int m_planebuffers[2];
	public:
		DebugPlaneLeaf();
		virtual ~DebugPlaneLeaf();

		virtual void Draw();
	};
}

#endif /* DEBUGPLANELEAF_H_ */
