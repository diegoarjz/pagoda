#pragma once

#include "transformation.h"

#include <pagoda/math/vec_base.h>

#include <memory>
#include <unordered_set>

namespace pgeditor::scene
{
class IAttachable;

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node();
	virtual ~Node();

	void SetParent(std::shared_ptr<Node> p);
	std::shared_ptr<Node> GetParent() const;
	const std::unordered_set<std::shared_ptr<Node>> &GetChildren() const;

	Transformation &GetWorldTransformation();

	void SetPosition(const float &x, const float &y, const float &z);
	void SetPosition(const boost::qvm::vec<float, 3> &pos);
	void SetWorldPosition(const float &x, const float &y, const float &z);
	void SetWorldPosition(const boost::qvm::vec<float, 3> &pos);
	const boost::qvm::vec<float, 3> &GetPosition() const;
	const boost::qvm::vec<float, 3> &GetWorldPosition();

	void Translate(const float &x, const float &y, const float &z);
	void Translate(const boost::qvm::vec<float, 3> &translation);
	void TranslateWorldCoordinates(const float &x, const float &y, const float &z);
	void TranslateWorldCoordinates(const boost::qvm::vec<float, 3> &translation);

	void SetScale(const float &scale);
	void SetScale(const float &x, const float &y, const float &z);
	void SetScale(const boost::qvm::vec<float, 3> &s);
	const boost::qvm::vec<float, 3> &GetScale() const;

	void Yaw(const pagoda::math::Radians<float> &angle);
	void Pitch(const pagoda::math::Radians<float> &angle);
	void Roll(const pagoda::math::Radians<float> &angle);

	void Rotate(const boost::qvm::mat<float, 3, 3> &r);

	void Attach(std::shared_ptr<IAttachable> attachable);
	void Detach(std::shared_ptr<IAttachable> attachable);

	const std::unordered_set<std::shared_ptr<IAttachable>> &GetAttachables() const;

private:
	void setWorldTransformationDirty();

	Transformation m_localTransformation;
	Transformation m_worldTransformation;
	bool m_worldTransformationIsDirty;

	std::weak_ptr<Node> m_parentNode;
	std::unordered_set<std::shared_ptr<Node>> m_children;

	std::unordered_set<std::shared_ptr<IAttachable>> m_attachables;
};
}  // namespace pgeditor::scene
