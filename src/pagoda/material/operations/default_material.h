#pragma once

#include <pagoda/objects/procedural_object_system.h>
#include <pagoda/objects/procedural_operation.h>

namespace pagoda::material
{
  using MaterialNetworkPtr = std::shared_ptr<class MaterialNetwork>;
}

namespace pagoda::material::operations {
class DefaultMaterial : public objects::ProceduralOperation {
public:
  DefaultMaterial(objects::ProceduralObjectSystemPtr objectSystem);

  const std::string& GetOperationName() const override;
	void SetParameters(objects::ParameterCallback* cb) override {};
  void Interfaces(objects::InterfaceCallback* cb) override;

  void DoWork() override;
private:
  objects::InterfacePtr m_output;
};
} // namespace pagoda::material::operations
