#include "uniform.h"

#include "gl_debug.h"
#include "shader_program.h"

#include <pagoda/common/debug/logger.h>

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <GL/glew.h>

using namespace pagoda::math;
using namespace boost;

namespace pgeditor::rendering
{
Uniform::Uniform(std::shared_ptr<ShaderProgram> program, const std::string &name, Uniform::Type type)
    : m_name(name), m_type(type), m_semantics(Uniform::Semantics::Custom), m_isDirty(true), m_shaderProgram(program)
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

Uniform::Uniform(std::shared_ptr<ShaderProgram> program, const std::string &name, Uniform::Type type,
                 Uniform::Semantics semantics)
    : m_name(name), m_type(type), m_semantics(semantics), m_isDirty(true), m_shaderProgram(program)
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

Uniform::~Uniform() {}

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

void Uniform::DoLoad(Renderer *r)
{
	START_PROFILE;

	CHECK_GL_ERROR(m_uniformId = glGetUniformLocation(m_shaderProgram->GetShaderProgramId(), m_name.c_str()));
	LOG_DEBUG("Binding Uniform '" << m_name << "' to location " << m_uniformId);
}

void Uniform::DoRender(Renderer *r)
{
	START_PROFILE;

	if (!IsDirty())
	{
		return;
	}

	switch (GetType())
	{
		case Uniform::Type::Float:
			break;
		case Uniform::Type::Vec2:
			break;
		case Uniform::Type::Vec3:
		{
			CHECK_GL_ERROR(glUniform3fv(m_uniformId, 1, GetVec3().a));
			break;
		}
		case Uniform::Type::Vec4:
			break;
		case Uniform::Type::Integer:
			break;
		case Uniform::Type::Unsigned:
			break;
		case Uniform::Type::Matrix2:
			break;
		case Uniform::Type::Matrix3:
			break;
		case Uniform::Type::Matrix4:
		{
			auto m = GetMatrix4();
			// clang-format off
					GLfloat mat[16] = {
                        A00(m), A10(m), A20(m), A30(m),
                        A01(m), A11(m), A21(m), A31(m),
                        A02(m), A12(m), A22(m), A32(m),
                        A03(m), A13(m), A23(m), A33(m),
                    };
			// clang-format on
			CHECK_GL_ERROR(glUniformMatrix4fv(m_uniformId, 1, GL_FALSE, mat));
			break;
		}
	};

	SetNotDirty();
}

void Uniform::DoDispose(Renderer *r)
{
	//
}

bool Uniform::IsDirty() const { return m_isDirty; }
void Uniform::SetNotDirty() { m_isDirty = false; }
}  // namespace pgeditor::rendering
