#pragma once

#include "base_value.h"

#include <type_traits>

namespace sscript
{
struct Boolean;
struct Float;
struct Integer;
struct String;
struct NullObject;
struct Function;
class TypeInfo;
class Class;
class Instance;

template<typename R>
struct ValueVisitor
{
	using return_type = R;
};

struct ValueVisitorBase
{
	virtual void Visit(Boolean*) = 0;
	virtual void Visit(Float*) = 0;
	virtual void Visit(Integer*) = 0;
	virtual void Visit(String*) = 0;
	virtual void Visit(NullObject*) = 0;
	virtual void Visit(Function*) = 0;
	virtual void Visit(TypeInfo*) = 0;
	virtual void Visit(Class*) = 0;
	virtual void Visit(Instance*) = 0;
};

template<typename R, class V>
struct value_visitor : public ValueVisitorBase
{
	explicit value_visitor(V& visitor) : m_visitor(visitor), m_returnValue() {}

	void Visit(Boolean* b) override { m_returnValue = m_visitor(*b); }
	void Visit(Float* f) override { m_returnValue = m_visitor(*f); }
	void Visit(Integer* f) override { m_returnValue = m_visitor(*f); }
	void Visit(String* s) override { m_returnValue = m_visitor(*s); }
	void Visit(NullObject* n) override { m_returnValue = m_visitor(*n); }
	void Visit(Function* c) override { m_returnValue = m_visitor(*c); }
	void Visit(TypeInfo* t) override { m_returnValue = m_visitor(*t); }
	void Visit(Class* c) override { m_returnValue = m_visitor(*c); }
	void Visit(Instance* v) override { m_returnValue = m_visitor(*v); }

	R get_return_value() { return m_returnValue; }

	V& m_visitor;
	R m_returnValue;
};

template<class V>
struct value_visitor<void, V> : public ValueVisitorBase
{
	explicit value_visitor(V& visitor) : m_visitor(visitor) {}

	void Visit(Boolean* b) override { m_visitor(*b); }
	void Visit(Float* f) override { m_visitor(*f); }
	void Visit(Integer* f) override { m_visitor(*f); }
	void Visit(String* s) override { m_visitor(*s); }
	void Visit(NullObject* n) override { m_visitor(*n); }
	void Visit(Function* c) override { m_visitor(*c); }
	void Visit(TypeInfo* t) override { m_visitor(*t); }
	void Visit(Class* c) override { m_visitor(*c); }
	void Visit(Instance* v) override { m_visitor(*v); }

	void get_return_value() {}

	V& m_visitor;
};

template<class VisitorType>
typename VisitorType::return_type apply_visitor(VisitorType& vis, BaseValuePtr value)
{
	value_visitor<typename VisitorType::return_type, VisitorType> v(vis);
	value->AcceptVisitor(&v);
	return v.get_return_value();
}

template<typename T>
struct value_type_visitor : public ValueVisitor<typename std::remove_reference<T>::type>
{
	T& operator()(T& t) { return t; }

	template<typename V>
	T& operator()(V&)
	{
		throw std::runtime_error("Unable");
	}
};

template<typename T>
typename std::remove_reference<T>::type value_as(BaseValuePtr v)
{
	value_type_visitor<T> visitor;
	return apply_visitor(visitor, v);
}

}  // namespace sscript
