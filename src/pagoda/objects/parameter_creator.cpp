#include "parameter_creator.h"

namespace pagoda::objects {
namespace {
template <class PAR_TYPE, class VAL_TYPE>
std::shared_ptr<PAR_TYPE>
createParameter(VAL_TYPE *v, const std::string &name, const std::string &label,
                const VAL_TYPE &defaultValue,
                std::function<bool(std::shared_ptr<ParameterBase>)> &f) {
  auto p = std::make_shared<PAR_TYPE>(v, name, label);
  p->SetValue(defaultValue);
  if (!f(p)) {
    return nullptr;
  }
  return p;
}
} // namespace

ParameterCreator::ParameterCreator(
    std::function<bool(std::shared_ptr<ParameterBase>)> parameterCreated)
    : m_parameterCreated{parameterCreated} {}

StringParameterPtr
ParameterCreator::StringParameter(std::string *v, const std::string &name,
                                  const std::string &label,
                                  const std::string &defaultValue) {
  return createParameter<objects::StringParameter>(v, name, label, defaultValue,
                                                   m_parameterCreated);
}

FloatParameterPtr ParameterCreator::FloatParameter(float *v,
                                                   const std::string &name,
                                                   const std::string &label,
                                                   float defaultValue) {
  return createParameter<objects::FloatParameter>(v, name, label, defaultValue,
                                                  m_parameterCreated);
}

IntParameterPtr ParameterCreator::IntegerParameter(int *v,
                                                   const std::string &name,
                                                   const std::string &label,
                                                   int defaultValue) {
  return createParameter<objects::IntParameter>(v, name, label, defaultValue,
                                                m_parameterCreated);
}

BooleanParameterPtr ParameterCreator::BooleanParameter(bool *v,
                                                       const std::string &name,
                                                       const std::string &label,
                                                       bool defaultValue) {
  return createParameter<objects::BooleanParameter>(
      v, name, label, defaultValue, m_parameterCreated);
}

IntParameterPtr
ParameterCreator::EnumParameter(int *v, const std::vector<std::string> &values,

                                const std::string &name,
                                const std::string &label, int defaultValue) {
  return createParameter<objects::IntParameter>(v, name, label, defaultValue,
                                                m_parameterCreated);
}

PlaneParameterPtr
ParameterCreator::PlaneParameter(math::Plane<float> *v, const std::string &name,
                                 const std::string &label,
                                 math::Plane<float> defaultValue) {
  return createParameter<objects::PlaneParameter>(v, name, label, defaultValue,
                                                  m_parameterCreated);
}
} // namespace pagoda::objects
