#include "set_material_attribute.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/common/exception/exception.h>

#include <pagoda/material/material_component.h>
#include <pagoda/material/material_system.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/dynamic/expression.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

#include <memory>
#include <string>
#include <unordered_map>

namespace pagoda::material::operations
{
using namespace dynamic;
using namespace geometry;
using namespace objects;
using namespace common::exception;

const std::string SetMaterialAttribute::inputObject("in");
const std::string SetMaterialAttribute::outputObject("out");

SetMaterialAttribute::SetMaterialAttribute(
  objects::ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}

SetMaterialAttribute::~SetMaterialAttribute()
{
}

void SetMaterialAttribute::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	// RegisterMember("name", cb->
}

const std::string& SetMaterialAttribute::GetOperationName() const
{
	static const std::string sName{"SetMaterialAttribute"};
	return sName;
}

void SetMaterialAttribute::Interfaces(InterfaceCallback* cb)
{
}

struct evaluator : public ValueVisitor<DynamicValueBasePtr>
{
	DynamicValueBasePtr operator()(ExpressionPtr& e)
	{
		return e->Evaluate();
	}
};

void SetMaterialAttribute::DoWork()
{
	START_PROFILE;

	auto materialSystem =
	  m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();

  /*
	while (HasInput(inputObject)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputObject);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject);
		std::shared_ptr<MaterialComponent> materialComponent =
		  materialSystem->GetComponentAs<MaterialComponent>(outObject);
		if (materialComponent == nullptr) {
			materialComponent =
			  materialSystem->CreateComponentAs<MaterialComponent>(outObject);
		}

		UpdateValue("name");
		UpdateValue("value");

		DynamicValueBasePtr v = GetValue("value");
		ExpressionPtr e = std::dynamic_pointer_cast<Expression>(v);
		if (e != nullptr) {
			v = e->Evaluate();
		}

		std::string name = get_value_as<std::string>(*GetValue("name"));
		materialComponent->GetMaterial().SetAttribute(name, v);
	}
  */
}
}  // namespace pagoda::material::operations
