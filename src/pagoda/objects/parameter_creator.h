#pragma once

#include "pagoda/objects/parameter_callback.h"

#include <functional>

namespace pagoda::objects {
class ParameterCreator : public NewParameterCallback {
public:
  ParameterCreator(
      std::function<bool(std::shared_ptr<ParameterBase>)> parameterCreated);

  StringParameterPtr
  StringParameter(std::string *v, const std::string &name,
                  const std::string &label,
                  const std::string &defaultValue = "") override;

  FloatParameterPtr FloatParameter(float *v, const std::string &name,
                                   const std::string &label,
                                   float defaultValue = 0.0f) override;

  IntParameterPtr IntegerParameter(int *v, const std::string &name,
                                   const std::string &label,
                                   int defaultValue = 0) override;

  BooleanParameterPtr BooleanParameter(bool *v, const std::string &name,
                                       const std::string &label,
                                       bool defaultValue = false) override;

  IntParameterPtr EnumParameter(int *v, const std::vector<std::string> &values,
                                const std::string &name,
                                const std::string &label,
                                int defaultValue = 0) override;

  PlaneParameterPtr PlaneParameter(
      math::Plane<float> *v, const std::string &name, const std::string &label,
      math::Plane<float> defaultValue = math::Plane<float>{}) override;

private:
  std::function<bool(std::shared_ptr<ParameterBase>)> m_parameterCreated;
};

} // namespace pagoda::objects
