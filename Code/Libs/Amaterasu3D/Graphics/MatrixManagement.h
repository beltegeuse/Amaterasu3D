#ifndef MATRIXMANAGEMENT_H_
#define MATRIXMANAGEMENT_H_

#include <Math/Matrix4.h>
#include <vector>
#include <Singleton.h>

class MatrixManagement : public CSingleton<MatrixManagement>
{
	MAKE_SINGLETON(MatrixManagement);
private:
	// Attributs
	int m_maxMatrix;
	std::vector<Math::CMatrix4> m_matrix;
	Math::CMatrix4 m_projectionMatrix;
	Math::CMatrix4 m_viewMatrix;
	Math::CMatrix4 m_identityMatrix;

	// Singleton implements
	MatrixManagement(int maxMatrix = 50);
	// Destructor ...
	virtual ~MatrixManagement();
public:

	/**
	 * ModelView Matrix
	 */
	// Public methods
	void PushMatrix(const Math::CMatrix4& matrix);
	void PopMatrix();
	// States of the stack
	bool IsEmpty() const;
	int StackSize() const;
	// Get Matrix
	const Math::CMatrix4& GetMatrix() const;

	//XXX Study if it's more interesting to get an observer pattern to submit matrix transform
	/**
	 * Projection matrix
	 */
	void SetProjectionMatrix(const Math::CMatrix4& matrix);
	const Math::CMatrix4& GetProjectionMatrix() const;

	/**
	 * View matrix
	 */
	void SetViewMatrix(const Math::CMatrix4& matrix);
	const Math::CMatrix4& GetViewMatrix() const;
};

#endif /* MATRIXMANAGEMENT_H_ */
