#pragma once

#include "attachable.h"
#include "lens.h"
#include "transformation.h"

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

namespace pgeditor::scene
{
class Camera final : public IAttachable
{
public:
	Camera();

	void SetPosition(const boost::qvm::vec<float, 3> &pos);
	void SetViewDirection(const boost::qvm::vec<float, 3> &dir);
	void SetTransformation(const Transformation &t);

	const boost::qvm::vec<float, 3> &GetPosition() const;
	const boost::qvm::vec<float, 3> &GetViewDirection() const;
	boost::qvm::vec<float, 3> GetRightVector() const;
	boost::qvm::vec<float, 3> GetUpVector() const;

	const boost::qvm::mat<float, 4, 4> &GetViewMatrix();
	const boost::qvm::mat<float, 4, 4> &GetProjectionMatrix();

	void SetLens(const Lens &lens);
	Lens &GetLens();

	void AcceptVisitor(IAttachableVisitor *visitor) override;

private:
	boost::qvm::vec<float, 3> m_position;
	boost::qvm::vec<float, 3> m_viewDirection;

	boost::qvm::mat<float, 4, 4> m_viewMatrix;
	Lens m_lens;
	bool m_viewMatrixDirty;
};
}  // namespace pgeditor::scene
