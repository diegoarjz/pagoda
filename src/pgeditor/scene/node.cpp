#include "node.h"

#include <pagoda/common/instrument/profiler.h>

#include <boost/qvm/map_mat_vec.hpp>

using namespace pagoda::math;
using namespace boost;

namespace pgeditor::scene
{
Node::Node() : m_localTransformation{}, m_worldTransformation{}, m_worldTransformationIsDirty(true) {}

Node::~Node() {}

void Node::SetParent(std::shared_ptr<Node> p)
{
	if (!m_parentNode.expired())
	{
		m_parentNode.lock()->m_children.erase(shared_from_this());
	}

	m_parentNode = p;

	if (p != nullptr)
	{
		p->m_children.insert(shared_from_this());
	}

	setWorldTransformationDirty();
}

std::shared_ptr<Node> Node::GetParent() const { return m_parentNode.lock(); }

const std::unordered_set<std::shared_ptr<Node>> &Node::GetChildren() const { return m_children; }

Transformation &Node::GetWorldTransformation()
{
	START_PROFILE;

	if (m_worldTransformationIsDirty)
	{
		if (m_parentNode.expired())
		{
			m_worldTransformation = m_localTransformation;
		}
		else
		{
			m_worldTransformation = m_parentNode.lock()->GetWorldTransformation() * m_localTransformation;
		}
		m_worldTransformationIsDirty = false;
	}
	return m_worldTransformation;
}

void Node::setWorldTransformationDirty()
{
	START_PROFILE;

	m_worldTransformationIsDirty = true;
	for (auto &n : m_children)
	{
		n->setWorldTransformationDirty();
	}
}

void Node::SetPosition(const float &x, const float &y, const float &z)
{
	START_PROFILE;

	m_localTransformation.SetPosition(x, y, z);
	setWorldTransformationDirty();
}

void Node::SetPosition(const boost::qvm::vec<float, 3> &pos)
{
	START_PROFILE;

	m_localTransformation.SetPosition(pos);
	setWorldTransformationDirty();
}

void Node::SetWorldPosition(const boost::qvm::vec<float, 3> &pos)
{
	START_PROFILE;

	auto posInParentWorldTransform = GetParent()->GetWorldTransformation().ConvertWorldToLocalCoordinates(pos);
	SetPosition(posInParentWorldTransform);
}

void Node::SetWorldPosition(const float &x, const float &y, const float &z)
{
	START_PROFILE;
	SetWorldPosition(boost::qvm::vec<float, 3>{x, y, z});
}

const boost::qvm::vec<float, 3> &Node::GetPosition() const
{
	START_PROFILE;
	return m_localTransformation.GetPosition();
}
const boost::qvm::vec<float, 3> &Node::GetWorldPosition()
{
	START_PROFILE;
	return GetWorldTransformation().GetPosition();
}

void Node::Translate(const float &x, const float &y, const float &z)
{
	START_PROFILE;
	m_localTransformation.Translate(x, y, z);
	setWorldTransformationDirty();
}

void Node::Translate(const boost::qvm::vec<float, 3> &translation)
{
	START_PROFILE;
	m_localTransformation.Translate(translation);
	setWorldTransformationDirty();
}

void Node::TranslateWorldCoordinates(const float &x, const float &y, const float &z) {}

void Node::TranslateWorldCoordinates(const boost::qvm::vec<float, 3> &translation) {}

void Node::SetScale(const float &scale)
{
	START_PROFILE;
	m_localTransformation.SetScale(scale);
	setWorldTransformationDirty();
}
void Node::SetScale(const float &x, const float &y, const float &z)
{
	START_PROFILE;
	m_localTransformation.SetScale(x, y, z);
	setWorldTransformationDirty();
}
void Node::SetScale(const boost::qvm::vec<float, 3> &s)
{
	START_PROFILE;
	m_localTransformation.SetScale(s);
	setWorldTransformationDirty();
}
const boost::qvm::vec<float, 3> &Node::GetScale() const
{
	START_PROFILE;
	return m_localTransformation.GetScale();
}

void Node::Yaw(const Radians<float> &angle)
{
	START_PROFILE;
	m_localTransformation.Yaw(angle);
	setWorldTransformationDirty();
}
void Node::Pitch(const Radians<float> &angle)
{
	START_PROFILE;
	m_localTransformation.Pitch(angle);
	setWorldTransformationDirty();
}
void Node::Roll(const Radians<float> &angle)
{
	START_PROFILE;
	m_localTransformation.Roll(angle);
	setWorldTransformationDirty();
}

void Node::Rotate(const boost::qvm::mat<float, 3, 3> &r)
{
	START_PROFILE;
	m_localTransformation.Rotate(r);
	setWorldTransformationDirty();
}

void Node::Attach(std::shared_ptr<IAttachable> attachable) { m_attachables.insert(attachable); }

void Node::Detach(std::shared_ptr<IAttachable> attachable) { m_attachables.erase(attachable); }

const std::unordered_set<std::shared_ptr<IAttachable>> &Node::GetAttachables() const { return m_attachables; }

}  // namespace pgeditor::scene
