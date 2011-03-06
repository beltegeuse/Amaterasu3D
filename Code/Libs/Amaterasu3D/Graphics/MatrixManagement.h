#ifndef MATRIXMANAGEMENT_H_
#define MATRIXMANAGEMENT_H_
#include <Enum.h>
#include <Math/Matrix4.h>
#include <vector>
#include <Singleton.h>
#include <sigc++/signal.h>

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

	sigc::signal<void, MatrixType, const Math::CMatrix4&> m_signal_event;

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

	//** to subscribe to the signal
	sigc::signal<void, MatrixType, const Math::CMatrix4&>& GetSignalEvent();
};

#endif /* MATRIXMANAGEMENT_H_ */
