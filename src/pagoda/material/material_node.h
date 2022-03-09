#pragma once

#include "shader_source.h"

#include <pagoda/common/types.h>

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace pagoda::material {
using MaterialNodePtr = std::shared_ptr<class MaterialNode>;
using MaterialNodeWeakPtr = std::shared_ptr<class MaterialNode>;

class MaterialNode {
public:
  using Type_t = common::Type;
  using Value_t = common::Value;

  using Hints_t = std::unordered_map<std::string, std::string>;
  struct HintKeys
  {
    static constexpr const char* stageIn = "stage-in";
    static constexpr const char* stageInAttribute = "stage-in-attribute";
    static constexpr const char* stageOut = "stage-out";
    static constexpr const char* stageOutAttribute = "stage-out-attribute";
    static constexpr const char* uniform =  "uniform";
    static constexpr const char* attribute = "attribute";
    static constexpr const char* shaderStage = "shader-stage";
    static constexpr const char* type = "type";
  };

  struct Input {
    Input() = default;

    Input(const std::string &name, const Type_t type,
          const Value_t &defaultValue)
        : name{name}, type{type}, defaultValue{defaultValue} {}

    Input(const std::string &name, const Type_t type,
          const Value_t &defaultValue, const Hints_t &hints)
        : name{name}, type{type}, defaultValue{defaultValue}, hints{hints} {}

    std::string name;
    Type_t type;
    Value_t defaultValue;
    Hints_t hints;
  };

  struct Output {
    std::string name;
    Type_t type;
    Hints_t hints;
  };

  struct Connection {
    std::string input;
    MaterialNodeWeakPtr upstreamNode;
    std::string upstreamOutput;
  };

  struct Parameter {
    Parameter(const std::string &name, const Type_t type,
              const Value_t &defaultValue)
        : name{name}, type{type}, defaultValue{defaultValue} {}

    std::string name;
    Type_t type;
    Value_t value;
    Value_t defaultValue;
  };

  using InputContainer_t = std::unordered_map<std::string, Input>;
  using OutputContainer_t = std::unordered_map<std::string, Output>;
  using ParameterContainer_t = std::unordered_map<std::string, Parameter>;
  using ConnectionContainer_t = std::unordered_map<std::string, Connection>;

  ////////////////////////////////////////
  /// \name Constructors
  ////////////////////////////////////////
  MaterialNode(MaterialNode &&node);
  virtual ~MaterialNode();

  MaterialNode(const MaterialNode &) = delete;
  MaterialNode &operator=(const MaterialNode &) = delete;

  ////////////////////////////////////////
  /// \name Name and ID
  ////////////////////////////////////////
  const std::string &GetName() const { return m_name; }
  const std::string &GetId() const { return m_id; }

  ////////////////////////////////////////
  /// \name Inputs
  ////////////////////////////////////////
  const Input *GetInput(const std::string &name) const;
  const InputContainer_t &GetInputs() const { return m_inputs; }

  void Inputs(std::function<void(const Input &)> f) const;
  const Type_t InputType(const std::string &name) const;
  const Value_t &InputDefaultValue(const std::string &name) const;

  bool HasInput(const std::string &name) const;

  ////////////////////////////////////////
  /// \name Outputs
  ////////////////////////////////////////
  const Output *GetOutput(const std::string &name) const;
  const OutputContainer_t &GetOutputs() const { return m_outputs; }
  const Type_t OutputType(const std::string &name);
  void Outputs(std::function<void(const Output &)> f) const;
  bool HasOutput(const std::string &name) const;

  ////////////////////////////////////////
  /// \name Connections
  ////////////////////////////////////////
  bool ConnectInput(const std::string &inputName, const MaterialNodePtr &node,
                    const std::string &upstreamName);

  bool CanConnectInput(const std::string &inputName,
                       const MaterialNodePtr &node,
                       const std::string &upstreamName);

  void DisconnectInput(const std::string &inputName);

  const ConnectionContainer_t &GetConnections() const;

  void Connections(std::function<void(const Connection &)> f) const;

  bool IsInputConnected(const std::string &inputName) const;

  ////////////////////////////////////////
  /// \name Parameters
  ////////////////////////////////////////
  const Parameter *GetParameter(const std::string &name) const;

  void Parameters(std::function<void(const Parameter &p)> f) const;
  bool IsParameterDefault(const std::string &name) const;
  const Type_t ParameterType(const std::string &name) const;
  const Value_t &ParameterValue(const std::string &name) const;
  const Value_t &ParameterDefaultValue(const std::string &name) const;

  void ResetParameter(const std::string &name);

  ////////////////////////////////////////
  /// \name Hints
  ////////////////////////////////////////
  const Hints_t &GetHints() const;

  ////////////////////////////////////////
  /// \name Source Generation
  ////////////////////////////////////////
  virtual const std::vector<std::string> &InputOrder() = 0;
  virtual bool SourceCode(ShaderSource &source) = 0;

  ////////////////////////////////////////
  /// \name Hash
  ////////////////////////////////////////
  void AppendHash(std::size_t &hash) const;

protected:
  friend class MaterialNetwork;

  MaterialNode(const std::string &nodeId);
  MaterialNode(const std::string &nodeId, const std::string &name);

  std::string m_name;
  std::string m_id;
  InputContainer_t m_inputs;
  OutputContainer_t m_outputs;
  ConnectionContainer_t m_connections;
  ParameterContainer_t m_parameters;
  Hints_t m_hints;
};

} // namespace pagoda::material
