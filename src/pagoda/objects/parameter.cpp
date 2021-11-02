#include "parameter.h"

#include <pagoda/pgscript/interpreter/interpreter.h>
#include <pagoda/pgscript/parser/parser.h>

#include <pagoda/common/exception/unimplemented.h>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace pagoda::objects
{
ParameterBase::ParameterBase(const std::string& name)
  : m_name{name}, m_label{name}
{
}

ParameterBase::ParameterBase(const std::string& name, const std::string& label)
  : m_name{name}, m_label{label}
{
}

ParameterBase::~ParameterBase()
{
}

bool ParameterBase::HasFlag(Flag flag) const
{
	return m_flags.test(static_cast<size_t>(flag));
}

void ParameterBase::SetFlag(Flag flag, bool val)
{
	m_flags.set(static_cast<size_t>(flag), val);
}

////////////////////////////////////////
/// \name Getters and Setters
////////////////////////////////////////
const std::string& ParameterBase::GetName() const
{
	return m_name;
}
const std::string& ParameterBase::GetLabel() const
{
	return m_label;
}

////////////////////////////////////////
/// \name Expressions
////////////////////////////////////////
bool ParameterBase::HasExpression() const
{
	return m_expression != nullptr;
}

void ParameterBase::SetExpression(const std::string& e)
{
	m_expression = dynamic::Expression::CreateExpression(e);
}

const std::string& ParameterBase::GetExpression() const
{
	static const std::string& sEmptyExpression{""};
	if (m_expression == nullptr) {
		return sEmptyExpression;
	}
	return m_expression->GetExpressionString();
}

void ParameterBase::EvaluateExpression()
{
	if (m_expression == nullptr) {
		return;
	}

	if (auto value = m_expression->Evaluate()) {
		FromDynamicValue(value);
	}
}

////////////////////////////////////////
/// \name Callbacks
////////////////////////////////////////
void ParameterBase::OnChanged(ParameterBase::OnChangedCallback cb)
{
	m_onChangedCallbacks.push_back(cb);
}

void ParameterBase::changed()
{
	for (const auto& cb : m_onChangedCallbacks) {
		cb(shared_from_this());
	}
}

////////////////////////////////////////
/// \name Conversion and Serialization
////////////////////////////////////////

template<>
dynamic::DynamicValueBasePtr Parameter<std::string>::ToDynamicValue()
{
	return std::make_shared<dynamic::String>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<float>::ToDynamicValue()
{
	return std::make_shared<dynamic::FloatValue>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<int>::ToDynamicValue()
{
	return std::make_shared<dynamic::Integer>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<bool>::ToDynamicValue()
{
	return std::make_shared<dynamic::Boolean>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<math::Plane<float>>::ToDynamicValue()
{
	return std::make_shared<dynamic::DynamicPlane>(*m_valuePtr);
}

template<>
void Parameter<std::string>::FromString(const std::string& value)
{
	SetValue(value);
}

template<>
void Parameter<float>::FromString(const std::string& value)
{
	SetValue(std::atof(value.c_str()));
}

template<>
void Parameter<int>::FromString(const std::string& value)
{
	SetValue(std::atoi(value.c_str()));
}

template<>
void Parameter<bool>::FromString(const std::string& value)
{
	if (value != "true" && value != "false") {
		LOG_ERROR("Invalid value " << value << " for a boolean parameter");
		return;
	}
	SetValue(value == "true");
}

template<>
void Parameter<math::Plane<float>>::FromString(const std::string& value)
{
	using namespace boost::spirit;
	using boost::phoenix::ref;
	using boost::spirit::ascii::space;
	using boost::spirit::qi::float_;
	using boost::spirit::qi::phrase_parse;

	auto first = value.begin();
	auto last = value.end();

	math::Vec3F normal;
	float dist;

	bool r = boost::spirit::qi::phrase_parse(
	  first, last,
	  float_[ref(normal.a[0]) = _1] >> "," >> float_[ref(normal.a[1]) = _1] >>
	    "," >> float_[ref(normal.a[2]) = _1] >> "," >> float_[ref(dist) = _1],
	  space);

	if (r && first == last) {
		SetValue(math::Plane<float>{normal, dist});
	} else {
		LOG_ERROR("Invalid string value '" << value << "' for a Plane parameter");
	}
}

template<>
std::string Parameter<std::string>::ToString() const
{
	return GetValue();
}

template<>
std::string Parameter<float>::ToString() const
{
	return std::to_string(GetValue());
}

template<>
std::string Parameter<int>::ToString() const
{
	return std::to_string(GetValue());
}

template<>
std::string Parameter<bool>::ToString() const
{
	return GetValue() ? "true" : "false";
}

template<>
std::string Parameter<math::Plane<float>>::ToString() const
{
	auto normal = m_valuePtr->GetNormal();
	auto dist = m_valuePtr->GetDistanceToOrigin();
	std::stringstream ss;
	ss << X(normal) << ", " << Y(normal) << ", " << Z(normal) << dist;
	return ss.str();
}
}  // namespace pagoda::objects

