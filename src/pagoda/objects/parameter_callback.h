#pragma once

#include "parameter.h"

#include <memory>
#include <string>

namespace pagoda::dynamic
{
using StringPtr = std::shared_ptr<class String>;
using FloatValuePtr = std::shared_ptr<class FloatValue>;
using IntegerPtr = std::shared_ptr<class Integer>;
using BooleanPtr = std::shared_ptr<class Boolean>;
using DynamicPlanePtr = std::shared_ptr<class DynamicPlane>;
using DynamicValueBasePtr = std::shared_ptr<class DynamicValueBase>;
}  // namespace pagoda::dynamic

namespace pagoda::objects
{
class Node;

class ParameterCallback
{
	public:
	virtual dynamic::DynamicValueBasePtr StringArgument(
	  const char* const name, const char* const label,
	  const std::string& defaultValue = "") = 0;

	virtual dynamic::DynamicValueBasePtr FloatArgument(
	  const char* const name, const char* const label,
	  float defaultValue = 0.0f) = 0;

	virtual dynamic::DynamicValueBasePtr IntegerArgument(
	  const char* const name, const char* const label, int defaultValue = 0) = 0;

	virtual dynamic::DynamicValueBasePtr BooleanArgument(
	  const char* const name, const char* const label,
	  bool defaultValue = false) = 0;

	virtual dynamic::DynamicValueBasePtr PlaneArgument(
	  const char* const name, const char* const label,
	  math::Plane<float> defaultValue = math::Plane<float>{}) = 0;
};

class NewParameterCallback
{
	public:
	virtual StringParameterPtr StringParameter(
	  std::string* v, const std::string& name, const std::string& label,
	  const std::string& defaultValue = "") = 0;

	virtual FloatParameterPtr FloatParameter(float* v, const std::string& name,
	                                         const std::string& label,
	                                         float defaultValue = 0.0f) = 0;

	virtual IntParameterPtr IntegerParameter(int* v, const std::string& name,
	                                         const std::string& label,
	                                         int defaultValue = 0) = 0;

	virtual BooleanParameterPtr BooleanParameter(bool* v, const std::string& name,
	                                             const std::string& label,
	                                             bool defaultValue = false) = 0;

	virtual PlaneParameterPtr PlaneParameter(
	  math::Plane<float>* v, const std::string& name, const std::string& label,
	  math::Plane<float> defaultValue = math::Plane<float>{}) = 0;
};
}  // namespace pagoda::objects
