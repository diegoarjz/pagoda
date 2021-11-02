#include "operation_node.h"

#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

#include <pagoda/common/utils.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/value_not_found.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/operation_factory.h>
#include <pagoda/objects/parameter_creator.h>
#include <pagoda/objects/procedural_operation.h>
#include <pagoda/objects/unknown_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char *OperationNode::name = "Operation";

OperationNode::OperationNode(OperationFactoryPtr operationFactory)
  : m_operationFactory(operationFactory)
{
	auto operationParameter =
	  std::make_shared<StringParameter>(&m_operationName, "operation");
	operationParameter->OnChanged([this](ParameterBasePtr p) {
		const auto operation = m_operationFactory->Create(m_operationName);
		if (operation == nullptr) {
			throw UnknownOperation(m_operationName);
		}
		SetOperation(operation);
	});
	m_parameters["operation"] = operationParameter;
}

OperationNode::~OperationNode()
{
}

class InterfaceCreator : public InterfaceCallback
{
	public:
	void InputInterface(InterfacePtr &interface, const std::string &name,
	                    const std::string &label, Interface::Arity arity) override
	{
		interface =
		  std::make_shared<Interface>(name, Interface::Type::Input, arity);
	}

	void OutputInterface(InterfacePtr &interface, const std::string &name,
	                     const std::string &label,
	                     Interface::Arity arity) override
	{
		interface =
		  std::make_shared<Interface>(name, Interface::Type::Output, arity);
	}
};

void OperationNode::SetOperation(ProceduralOperationPtr operation)
{
	m_operation = operation;
	InterfaceCreator interfaceCreator;
	m_operation->Interfaces(&interfaceCreator);
	ParameterCreator paramCreator([this](ParameterBasePtr p) {
		return m_parameters.insert({p->GetName(), p}).second;
	});
	m_operation->Parameters(&paramCreator);
	RegisterOrSetMember("op", m_operation);
}

void OperationNode::ForEachOperationParameter(
  const std::function<void(const std::string &,
                           const dynamic::DynamicValueBasePtr &)> &f) const
{
	m_operation->ForEachParameter(f);
}

namespace
{
class InterfaceObjectProvider : public InterfaceCallback
{
	public:
	using Inserter_t = std::back_insert_iterator<std::vector<InterfacePtr>>;

	InterfaceObjectProvider(Inserter_t inputs, Inserter_t outputs)
	  : m_inputs{inputs}, m_outputs{outputs}
	{
	}

	void InputInterface(InterfacePtr &interface, const std::string &name,
	                    const std::string &label, Interface::Arity arity) override
	{
		CRITICAL_ASSERT_MSG(interface != nullptr,
		                    "Input Interface not set when executing a node");
		m_inputs = interface;
	}

	void OutputInterface(InterfacePtr &interface, const std::string &name,
	                     const std::string &label,
	                     Interface::Arity arity) override
	{
		CRITICAL_ASSERT_MSG(interface != nullptr,
		                    "Output Interface not set when executing a node");
		m_outputs = interface;
	}

	std::back_insert_iterator<std::vector<InterfacePtr>> m_inputs;
	std::back_insert_iterator<std::vector<InterfacePtr>> m_outputs;
};

template<class T>
void pairInterfaceWithNodes(
  const std::vector<InterfacePtr> &interfaces, const NodeSet &nodes,
  std::unordered_map<InterfacePtr, std::shared_ptr<T>> &pairings)
{
	for (auto &inputInterface : interfaces) {
		auto interfaceName = inputInterface->GetName();
		for (auto &n : nodes) {
			if (auto interfaceNode = std::dynamic_pointer_cast<T>(n)) {
				if (interfaceNode->GetInterfaceName() == interfaceName) {
					pairings.emplace(inputInterface, interfaceNode);
				}
			}
		}
	}
}

void execute(
  ProceduralOperationPtr operation,
  const std::unordered_map<InterfacePtr, OutputInterfaceNodePtr> &outputMap)
{
	operation->Execute();

	for (const auto &out : outputMap) {
		LOG_TRACE(ProceduralGraph, "Copying to " << operation->GetOperationName()
		                                         << " " << out.first->GetName()
		                                         << " interface");
		switch (out.first->GetArity()) {
			case Interface::Arity::One:
				out.second->AddProceduralObject(out.first->Get());
				break;
			case Interface::Arity::Many:
				out.second->AddProceduralObject(out.first->GetNext());
				break;
			case Interface::Arity::All:
				out.first->GetAll([&out](ProceduralObjectPtr &o) {
					out.second->AddProceduralObject(o);
				});
				break;
		};
	}
}
}  // namespace

void OperationNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes)
{
	LOG_TRACE(ProceduralGraph,
	          "Executing OperationNode " << GetName() << "(" << GetId() << ")");

	// Compute all parameters that have an expression
	for (auto &p : m_parameters) {
		if (p.second->HasExpression()) {
			p.second->EvaluateExpression();
		}
	}

	std::vector<InterfacePtr> inputs;
	std::vector<InterfacePtr> outputs;

	// Get the input and output interfaces
	InterfaceObjectProvider provider(std::back_inserter(inputs),
	                                 std::back_inserter(outputs));
	m_operation->Interfaces(&provider);

	// Pair them with the respective nodes
	std::unordered_map<InterfacePtr, InputInterfaceNodePtr> inputMap;
	std::unordered_map<InterfacePtr, OutputInterfaceNodePtr> outputMap;
	pairInterfaceWithNodes<InputInterfaceNode>(inputs, inNodes, inputMap);
	pairInterfaceWithNodes<OutputInterfaceNode>(outputs, outNodes, outputMap);

	// Keep track of the interfaces with Arity::Many and the respective
	// ProceduralObjectPtr. These will be used to compute the cartesian product
	// and associate them with each interface.
	std::vector<InterfacePtr> manyArityInterfaces;
	std::vector<std::vector<ProceduralObjectPtr>> manyArityInterfacesObjects;

	// Go through all input interfaces and set them up as follows:
	//   - For interfaces of Arity::One: expect only one object in the input
	//   interface node and set it in the operation interface
	//   - For interfaces of Arity::All: pass all objects in the interface node
	//   the the operation interface
	//   - For interfaces of Arity::Many: add an entry to
	//   manyArityInterfacesObjects. We will need this afterwards to do a
	//   cartesian product to get all the combination of objects
	for (auto &i : inputs) {
		auto interfaceNode = inputMap[i];
		switch (i->GetArity()) {
			case Interface::Arity::One:
				LOG_TRACE(ProceduralGraph,
				          "Interface " << i->GetName() << " has Arity One.");
				// Set the object in interfaces with arity One
				CRITICAL_ASSERT_MSG(interfaceNode->GetProceduralObjects().size() == 1,
				                    "Expected interface node to only have one object");
				i->Set(interfaceNode->GetProceduralObjects().front());
				break;
			case Interface::Arity::All:
				LOG_TRACE(ProceduralGraph,
				          "Interface " << i->GetName() << " has Arity All.");
				// Set all objects in interfaces with arity All
				for (auto &o : interfaceNode->GetProceduralObjects()) {
					i->Add(o);
				}
				break;
			case Interface::Arity::Many: {
				const auto &objects = interfaceNode->GetProceduralObjects();
				LOG_TRACE(ProceduralGraph,
				          "Interface " << i->GetName() << " has Arity Many.");
				LOG_TRACE(ProceduralGraph, "  It has " << objects.size() << " objects");
				manyArityInterfacesObjects.emplace_back(objects.begin(), objects.end());
				manyArityInterfaces.push_back(i);
				break;
			}
		};
	}

	if (manyArityInterfacesObjects.empty()) {
		// All interfaces are setup so we can execute
		execute(m_operation, outputMap);
	} else {
		// Do a CartesianProduct of the procedural objects in the Arity::Many
		// interfaces and for each result set each object in the respective
		// interface and execute the operation.
		common::CartesianProduct(manyArityInterfacesObjects,
		                         [this, &manyArityInterfaces, &outputMap](
		                           const std::vector<ProceduralObjectPtr> &obj) {
			                         for (auto i = 0; i < obj.size(); ++i) {
				                         manyArityInterfaces[i]->SetNext(obj[i]);
			                         }
			                         execute(m_operation, outputMap);
		                         });
	}

	/*
	// Copy all parameters registered in this node to the operation
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd();
	     ++parIter) {
	  m_operation->RegisterOrSetMember(parIter->first, GetMember(parIter->first));
	}
	*/
}

const char *const OperationNode::GetNodeType()
{
	static const char *const sNodeType = "Operation";
	return sNodeType;
}

}  // namespace pagoda::graph
