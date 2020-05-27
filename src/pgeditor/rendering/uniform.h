#pragma once

#include "bound_state.h"
#include "renderable.h"

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>
#include <memory>

#include <string>
#include <variant>

#include <GL/glew.h>

namespace pgeditor::rendering
{
class ShaderProgram;

class Uniform : public Renderable
{
public:
	enum class Type
	{
		Float,
		Vec2,
		Vec3,
		Vec4,
		Integer,
		Unsigned,
		Matrix2,
		Matrix3,
		Matrix4
	};

	enum class Semantics
	{
		ModelMatrix,
		ViewMatrix,
		ProjectionMatrix,
		Custom,
		MAX
	};

	Uniform(std::shared_ptr<ShaderProgram> program, const std::string &name, Type type);
	Uniform(std::shared_ptr<ShaderProgram> program, const std::string &name, Type type, Semantics semantics);
	virtual ~Uniform();

	const std::string &GetName() const { return m_name; }
	Type GetType() const { return m_type; }
	Semantics GetSemantics() const { return m_semantics; }

	void SetFloat(float f);
	float GetFloat() const;
	void SetVec2(const boost::qvm::vec<float, 2> &v);
	boost::qvm::vec<float, 2> GetVec2() const;
	void SetVec3(const boost::qvm::vec<float, 3> &v);
	boost::qvm::vec<float, 3> GetVec3() const;
	void SetVec4(const boost::qvm::vec<float, 4> &v);
	boost::qvm::vec<float, 4> GetVec4() const;
	void SetInteger(int32_t i);
	int32_t GetInteger() const;
	void SetUnsigned(uint32_t ui);
	uint32_t GetUnsigned() const;
	void SetMatrix2(const boost::qvm::mat<float, 2, 2> &m);
	boost::qvm::mat<float, 2, 2> GetMatrix2() const;
	void SetMatrix3(const boost::qvm::mat<float, 3, 3> &m);
	boost::qvm::mat<float, 3, 3> GetMatrix3() const;
	void SetMatrix4(const boost::qvm::mat<float, 4, 4> &m);
	const boost::qvm::mat<float, 4, 4> &GetMatrix4() const;

	bool IsDirty() const;
	void SetNotDirty();

protected:
	void DoLoad(Renderer *r) override;
	void DoRender(Renderer *r) override;
	void DoDispose(Renderer *r) override;

private:
	// clang-format off
    using uniform_value = std::variant<
        float,
        boost::qvm::vec<float, 2>,
        boost::qvm::vec<float, 3>,
        boost::qvm::vec<float, 4>,
        int32_t,
        // TODO i2, i3, i4
        uint32_t,
        // TODO ui2, ui3, ui4
        boost::qvm::mat<float, 2, 2>,
        boost::qvm::mat<float, 3, 3>,
        boost::qvm::mat<float, 4, 4>
    >;
	// clang-format on

	std::string m_name;
	uniform_value m_value;
	Type m_type;
	Semantics m_semantics;
	bool m_isDirty;

	GLuint m_uniformId;
	std::shared_ptr<ShaderProgram> m_shaderProgram;
};
}  // namespace pgeditor::rendering
