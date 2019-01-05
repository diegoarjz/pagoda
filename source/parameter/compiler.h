#ifndef SELECTOR_PARAMETER_COMPILER_H_
#define SELECTOR_PARAMETER_COMPILER_H_

#include "context.h"
#include "parameter_resolver.h"

#include <cstdint>

namespace selector
{
class ParameterExecutionNode
{
public:
	explicit ParameterExecutionNode(std::shared_ptr<ParameterBase> &parameter);

	void AddNextParameter(std::shared_ptr<ParameterExecutionNode> next_node);
	const std::list<std::weak_ptr<ParameterExecutionNode>> GetDependantNodes() const { return m_nextParameters; }
	void Evaluate();

	friend class Compiler;

private:
	std::shared_ptr<ParameterBase> m_parameter;
	std::list<std::weak_ptr<ParameterExecutionNode>> m_nextParameters;
};

class Expression;
class Compiler
{
public:
	enum class CompilationFailReason
	{
		UnableToResolveParameter,
		InvalidSyntax,
		CompilationSuccessful
	};

	struct CompilationErrorInfo
	{
		CompilationFailReason reason;
		std::shared_ptr<ParameterBase> offending_parameter;
	};

	explicit Compiler(std::shared_ptr<Context> &root_context);

	bool Compile();
	const CompilationErrorInfo &GetErrorInfo() const { return m_compilationError; }
	const std::list<std::shared_ptr<ParameterExecutionNode>> &GetRootNodes() const { return m_rootNodes; }

private:
	bool CompileContext(std::shared_ptr<Context> context);
	bool CompileExpression(std::shared_ptr<Context> context, std::shared_ptr<ParameterBase> parameter);
	bool CompileFloatExpression(std::shared_ptr<Expression> expression, std::shared_ptr<ParameterBase> parameter,
	                            const std::string &pre_processed_expression);

	std::shared_ptr<Context> m_rootContext;
	ParameterResolver m_parameteResolver;
	std::list<std::shared_ptr<ParameterExecutionNode>> m_rootNodes;
	std::unordered_map<ParameterBase *, std::shared_ptr<ParameterExecutionNode>> m_parameterLookup;
	CompilationErrorInfo m_compilationError;
};  // class Compiler

}  // namespace selector

#endif
