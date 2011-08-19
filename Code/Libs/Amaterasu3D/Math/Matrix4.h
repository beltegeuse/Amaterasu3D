#ifndef MATRIX4_H
#define MATRIX4_H

//==========================================================
// En-tetes
//==========================================================
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Vector2.h>
#include <Math/Matrix3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace ama3D
{
namespace Math
{
////////////////////////////////////////////////////////////
/// Classe des matrices ��� 4 composantes r���elles
////////////////////////////////////////////////////////////

typedef glm::mat4 CMatrix4;

} // namespace Math
}
#endif // MATRIX4_H
