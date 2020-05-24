#include "uniform.h"

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace pagoda::math;
using namespace boost;

namespace pgeditor::rendering
{
Uniform::Uniform(const std::string &name, Uniform::Type type)
    : m_name(name), m_type(type), m_semantics(Uniform::Semantics::Custom), m_isDirty(true)
{
	switch (type)
	{
		case Type::Float:
			m_value = 0.0f;
			break;
		case Type::Vec2:
			m_value = boost::qvm::vec<float, 2>{0, 0};
			break;
		case Type::Vec3:
			m_value = boost::qvm::vec<float, 3>{0, 0, 0};
			break;
		case Type::Vec4:
			m_value = boost::qvm::vec<float, 4>{0, 0, 0, 0};
			break;
		case Type::Integer:
			m_value = 0;
			break;
		case Type::Unsigned:
			m_value = 0u;
			break;
		case Type::Matrix2:
			m_value = boost::qvm::mat<float, 2, 2>(boost::qvm::diag_mat(boost::qvm::vec<float, 2>{1.0f, 1.0f}));
			break;
		case Type::Matrix3:
			m_value = boost::qvm::mat<float, 3, 3>(boost::qvm::diag_mat(boost::qvm::vec<float, 3>{1.0f, 1.0f, 1.0f}));
			break;
		case Type::Matrix4:
			m_value =
			    boost::qvm::mat<float, 4, 4>(boost::qvm::diag_mat(boost::qvm::vec<float, 4>{1.0f, 1.0f, 1.0f, 1.0f}));
			break;
	};
}

Uniform::Uniform(const std::string &name, Uniform::Type type, Uniform::Semantics semantics)
    : m_name(name), m_type(type), m_semantics(semantics), m_isDirty(true)
{
	switch (type)
	{
		case Type::Float:
			m_value = 0.0f;
			break;
		case Type::Vec2:
			m_value = boost::qvm::vec<float, 2>{0, 0};
			break;
		case Type::Vec3:
			m_value = boost::qvm::vec<float, 3>{0, 0, 0};
			break;
		case Type::Vec4:
			m_value = boost::qvm::vec<float, 4>{0, 0, 0, 0};
			break;
		case Type::Integer:
			m_value = 0;
			break;
		case Type::Unsigned:
			m_value = 0u;
			break;
		case Type::Matrix2:
			m_value = boost::qvm::mat<float, 2, 2>(boost::qvm::diag_mat(boost::qvm::vec<float, 2>{1.0f, 1.0f}));
			break;
		case Type::Matrix3:
			m_value = boost::qvm::mat<float, 3, 3>(boost::qvm::diag_mat(boost::qvm::vec<float, 3>{1.0f, 1.0f, 1.0f}));
			break;
		case Type::Matrix4:
			m_value =
			    boost::qvm::mat<float, 4, 4>(boost::qvm::diag_mat(boost::qvm::vec<float, 4>{1.0f, 1.0f, 1.0f, 1.0f}));
			break;
	};
}

void Uniform::SetFloat(float f)
{
	if (std::get<float>(m_value) == f)
	{
		return;
	}
	m_value = f;
	m_isDirty = true;
}

float Uniform::GetFloat() const { return std::get<float>(m_value); }

void Uniform::SetVec2(const boost::qvm::vec<float, 2> &v)
{
	if (std::get<boost::qvm::vec<float, 2>>(m_value) == v)
	{
		return;
	}
	m_value = v;
	m_isDirty = true;
}

boost::qvm::vec<float, 2> Uniform::GetVec2() const { return std::get<boost::qvm::vec<float, 2>>(m_value); }

void Uniform::SetVec3(const boost::qvm::vec<float, 3> &v)
{
	if (std::get<boost::qvm::vec<float, 3>>(m_value) == v)
	{
		return;
	}
	m_value = v;
	m_isDirty = true;
}

boost::qvm::vec<float, 3> Uniform::GetVec3() const { return std::get<boost::qvm::vec<float, 3>>(m_value); }

void Uniform::SetVec4(const boost::qvm::vec<float, 4> &v)
{
	if (std::get<boost::qvm::vec<float, 4>>(m_value) == v)
	{
		return;
	}
	m_value = v;
	m_isDirty = true;
}

boost::qvm::vec<float, 4> Uniform::GetVec4() const { return std::get<boost::qvm::vec<float, 4>>(m_value); }

void Uniform::SetInteger(int32_t i)
{
	if (std::get<int32_t>(m_value) == i)
	{
		return;
	}
	m_value = i;
	m_isDirty = true;
}

int32_t Uniform::GetInteger() const { return std::get<int32_t>(m_value); }

void Uniform::SetUnsigned(uint32_t ui)
{
	if (std::get<uint32_t>(m_value) == ui)
	{
		return;
	}
	m_value = ui;
	m_isDirty = true;
}

uint32_t Uniform::GetUnsigned() const { return std::get<uint32_t>(m_value); }

void Uniform::SetMatrix2(const boost::qvm::mat<float, 2, 2> &m)
{
	if (std::get<boost::qvm::mat<float, 2, 2>>(m_value) == m)
	{
		return;
	}
	m_value = m;
	m_isDirty = true;
}

boost::qvm::mat<float, 2, 2> Uniform::GetMatrix2() const { return std::get<boost::qvm::mat<float, 2, 2>>(m_value); }

void Uniform::SetMatrix3(const boost::qvm::mat<float, 3, 3> &m)
{
	if (std::get<boost::qvm::mat<float, 3, 3>>(m_value) == m)
	{
		return;
	}
	m_value = m;
	m_isDirty = true;
}

boost::qvm::mat<float, 3, 3> Uniform::GetMatrix3() const { return std::get<boost::qvm::mat<float, 3, 3>>(m_value); }

void Uniform::SetMatrix4(const boost::qvm::mat<float, 4, 4> &m)
{
	if (std::get<boost::qvm::mat<float, 4, 4>>(m_value) == m)
	{
		return;
	}
	m_value = m;
	m_isDirty = true;
}

const boost::qvm::mat<float, 4, 4> &Uniform::GetMatrix4() const
{
	return std::get<boost::qvm::mat<float, 4, 4>>(m_value);
}

std::size_t Uniform::GetBindableId() const { return m_bindableId; }
BoundState Uniform::GetBoundState() const { return m_bound; }
void Uniform::Bind(const std::size_t &id)
{
	m_bound = BoundState::Bound;
	m_bindableId = id;
}
void Uniform::Unbind() { m_bound = BoundState::Unbound; }
bool Uniform::IsDirty() const { return m_isDirty; }
void Uniform::SetNotDirty() { m_isDirty = false; }
}  // namespace pgeditor::rendering
