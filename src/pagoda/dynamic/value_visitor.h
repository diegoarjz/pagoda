#pragma once

#include "boolean_value.h"
#include "dynamic_class.h"
#include "dynamic_instance.h"
#include "dynamic_plane.h"
#include "dynamic_value_base.h"
#include "expression.h"
#include "float_value.h"
#include "function.h"
#include "integer_value.h"
#include "null_object_value.h"
#include "string_value.h"
#include "type_info.h"
#include "vector3.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/objects/procedural_operation.h>

#include <stdexcept>
#include <type_traits>

namespace pagoda::dynamic
{
template<typename R>
struct ValueVisitor
{
	using return_type = R;
};

class ValueVisitorBase
{
	public:
	virtual void Visit(Boolean&) = 0;
	virtual void Visit(FloatValue&) = 0;
	virtual void Visit(Integer&) = 0;
	virtual void Visit(String&) = 0;
	virtual void Visit(NullObject&) = 0;
	virtual void Visit(TypeInfo&) = 0;
	virtual void Visit(Vector3&) = 0;
	virtual void Visit(DynamicPlane&) = 0;
	virtual void Visit(Function&) = 0;
	virtual void Visit(DynamicClass&) = 0;
	virtual void Visit(DynamicInstance&) = 0;
	virtual void Visit(Expression&) = 0;
	virtual void Visit(objects::ProceduralOperation&) = 0;
};

template<typename R, class V>
class value_visitor : public ValueVisitorBase
{
	public:
	explicit value_visitor(V& visitor) : m_visitor(visitor), m_returnValue() {}

	void Visit(Boolean& b) override { m_returnValue = m_visitor(b); }
	void Visit(FloatValue& f) override { m_returnValue = m_visitor(f); }
	void Visit(Integer& f) override { m_returnValue = m_visitor(f); }
	void Visit(String& s) override { m_returnValue = m_visitor(s); }
	void Visit(NullObject& n) override { m_returnValue = m_visitor(n); }
	void Visit(TypeInfo& t) override { m_returnValue = m_visitor(t); }
	void Visit(Vector3& v) override { m_returnValue = m_visitor(v); }
	void Visit(DynamicPlane& p) override { m_returnValue = m_visitor(p); }
	void Visit(Function& f) override { m_returnValue = m_visitor(f); }
	void Visit(DynamicClass& c) override { m_returnValue = m_visitor(c); }
	void Visit(DynamicInstance& i) override { m_returnValue = m_visitor(i); }
	void Visit(Expression& e) override { m_returnValue = m_visitor(e); }
	void Visit(objects::ProceduralOperation& p) override { m_returnValue = m_visitor(p); }

	R get_return_value() { return m_returnValue; }

	V& m_visitor;
	R m_returnValue;
};

template<class V>
class value_visitor<void, V> : public ValueVisitorBase
{
	public:
	explicit value_visitor(V& visitor) : m_visitor(visitor) {}

	void Visit(Boolean& b) override { m_visitor(b); }
	void Visit(FloatValue& f) override { m_visitor(f); }
	void Visit(Integer& f) override { m_visitor(f); }
	void Visit(String& s) override { m_visitor(s); }
	void Visit(NullObject& n) override { m_visitor(n); }
	void Visit(TypeInfo& t) override { m_visitor(t); }
	void Visit(Vector3& v) override { m_visitor(v); }
	void Visit(DynamicPlane& p) override { m_visitor(p); }
	void Visit(Function& f) override { m_visitor(f); }
	void Visit(DynamicClass& c) override { m_visitor(c); }
	void Visit(DynamicInstance& i) override { m_visitor(i); }
	void Visit(Expression& e) override { m_visitor(e); }
	void Visit(objects::ProceduralOperation& p) override { m_visitor(p); }

	void get_return_value() {}

	V& m_visitor;
};

template<class VisitorType>
typename VisitorType::return_type apply_visitor(VisitorType& vis, DynamicValueBase& value)
{
	value_visitor<typename VisitorType::return_type, VisitorType> v(vis);
	value.AcceptVisitor(v);
	return v.get_return_value();
}

template<typename T>
class value_type_visitor : public ValueVisitor<typename std::remove_reference<T>::type>
{
	public:
	T& operator()(T& t) { return t; }

	template<typename V>
	T& operator()(V&)
	{
		throw common::exception::Exception("Unable to apply the value type visitor.");
	}
};

}  // namespace pagoda::dynamic
