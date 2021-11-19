#pragma once

#include "pagoda/dynamic/boolean_value.h"
#include "pagoda/dynamic/dynamic_plane.h"
#include "pagoda/dynamic/dynamic_value_base.h"
#include "pagoda/dynamic/expression.h"
#include "pagoda/dynamic/float_value.h"
#include "pagoda/dynamic/get_value_as.h"
#include "pagoda/dynamic/integer_value.h"
#include "pagoda/dynamic/string_value.h"

#include "pagoda/math/plane.h"

#include <bitset>
#include <string>

namespace pagoda::objects
{
/**
 * Base class for Parameter
 * Should not be instantiated directly.
 *
 * Parameters hold a pointer to a value that is stored elsewhere, usually in a
 * ProceduralOperation.
 */
class ParameterBase : public std::enable_shared_from_this<ParameterBase>
{
	public:
	enum class Flag
	{
		NoWrite,
		FlagCount
	};

	////////////////////////////////////////
	/// \name Getters and Setters
	////////////////////////////////////////
	/**
	 * Returns the Parameter's name.
	 */
	const std::string& GetName() const;

	/**
	 * Returns the Parameter's Label.
	 */
	const std::string& GetLabel() const;
	////////////////////////////////////////
	/// \name Parameter Flags
	////////////////////////////////////////

	/**
	 * Returns whether the \p flag is set.
	 */
	bool HasFlag(Flag flag) const;

	/**
	 * Sets the \p flag to \p val.
	 */
	void SetFlag(Flag flag, bool val);

	////////////////////////////////////////
	/// \name Expressions
	////////////////////////////////////////

	using Variable_t =
	  std::variant<dynamic::DynamicValueBasePtr, std::shared_ptr<ParameterBase>>;

	class VariableProvider
	{
public:
		virtual bool GetVariable(const std::list<std::string>& path,
		                         Variable_t& outVar) = 0;
	};

	/**
	 * Returns whether or not this parameter has an Expression.
	 */
	bool HasExpression() const;

	/**
	 * Sets the parameter to use the expression in \p e.
	 */
	void SetExpression(const std::string& e);

	/**
	 * Returns a string containing the expression.
	 */
	const std::string& GetExpression() const;

	/**
	 * Evaluates the expression and sets the value.
	 */
	void EvaluateExpression(VariableProvider* provider = nullptr);

	////////////////////////////////////////
	/// \name Callbacks
	////////////////////////////////////////

	/// Used in ParameterBase::OnChanged
	using OnChangedCallback = std::function<void(std::shared_ptr<ParameterBase>)>;

	/**
	 * Add a OnChangedCallback to the list of callbacks that will be called
	 * whenever a Parameter changes its value.
	 */
	void OnChanged(OnChangedCallback cb);

	////////////////////////////////////////
	/// \name Conversion and Serialization
	////////////////////////////////////////

	/**
	 * Sets the Parameter value from a DynamicValueBase.
	 */
	virtual void FromDynamicValue(
	  const dynamic::DynamicValueBasePtr& dynamicValue) = 0;

	/**
	 * Returns the DynamicValueBase equivalent of this Parameter.
	 */
	virtual dynamic::DynamicValueBasePtr ToDynamicValue() = 0;

	virtual void FromString(const std::string& value) = 0;
	virtual std::string ToString() const = 0;

	protected:
	ParameterBase(const std::string& name);
	ParameterBase(const std::string& name, const std::string& label);
	virtual ~ParameterBase();

	void changed();

	private:
	std::string m_name;
	std::string m_label;
	std::bitset<static_cast<size_t>(Flag::FlagCount)> m_flags;

	std::vector<OnChangedCallback> m_onChangedCallbacks;

	std::shared_ptr<dynamic::Expression> m_expression;
};

template<typename VALUE>
class Parameter : public ParameterBase
{
	public:
	/** Constructs a Parameter with a \p value and a \p name.
	 * The label will be set to be equal to the name.
	 */
	Parameter(VALUE* value, const std::string& name)
	  : ParameterBase(name), m_valuePtr{value}
	{
	}

	/**
	 * Constructs a Parameter with \p value, a \p name and a \p name.
	 */
	Parameter(VALUE* value, const std::string& name, const std::string& label)
	  : ParameterBase(name, label), m_valuePtr{value}
	{
	}

	~Parameter() override
	{
	}

	////////////////////////////////////////
	/// \name Getters and Setters
	////////////////////////////////////////
	/**
	 * Returns the value.
	 */
	const VALUE& GetValue() const
	{
		return *m_valuePtr;
	}

	/**
	 * Returns the value.
	 */
	VALUE& GetValue()
	{
		return *m_valuePtr;
	}

	/**
	 * Sets the value.
	 */
	void SetValue(const VALUE& v)
	{
		if (*m_valuePtr != v) {
			*m_valuePtr = v;
			changed();
		}
	}

	/**
	 * Moves the value.
	 */
	void SetValue(VALUE&& v)
	{
		if (*m_valuePtr != v) {
			*m_valuePtr = std::move(v);
			changed();
		}
	}

	////////////////////////////////////////
	/// \name Conversion and Serialization
	////////////////////////////////////////

	void FromDynamicValue(
	  const dynamic::DynamicValueBasePtr& dynamicValue) override
	{
		VALUE v = dynamic::get_value_as<VALUE>(*dynamicValue);
		SetValue(v);
	}

	dynamic::DynamicValueBasePtr ToDynamicValue() override
	{
		return nullptr;
	}

	void FromString(const std::string& value) override;
	std::string ToString() const override;

	private:
	VALUE* m_valuePtr;  ///< The pointer to the value.
};

using StringParameter = Parameter<std::string>;
using StringParameterPtr = std::shared_ptr<StringParameter>;

using FloatParameter = Parameter<float>;
using FloatParameterPtr = std::shared_ptr<FloatParameter>;

using IntParameter = Parameter<int>;
using IntParameterPtr = std::shared_ptr<IntParameter>;

using BooleanParameter = Parameter<bool>;
using BooleanParameterPtr = std::shared_ptr<BooleanParameter>;

using PlaneParameter = Parameter<math::Plane<float>>;
using PlaneParameterPtr = std::shared_ptr<PlaneParameter>;

}  // namespace pagoda::objects
