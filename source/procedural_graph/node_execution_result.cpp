#include "node_execution_result.h"

namespace selector
{
class NodeExecutionResult::Impl
{
public:
	explicit Impl(NodeExecutionResult::ResultStatus status) : m_status(status), m_result(nullptr) {}
	~Impl()
	{
		if (m_deleteFunction)
		{
			m_deleteFunction(m_result);
		}
	}

	ResultStatus GetResultStatus() const { return m_status; }

	const void* GetResult() const { return m_result; }
	void SetResult(void* result, std::function<void(void*)> deleteFunction)
	{
		if (m_deleteFunction)
		{
			m_deleteFunction(m_result);
		}

		m_result = result;
		m_deleteFunction = deleteFunction;
	}

private:
	NodeExecutionResult::ResultStatus m_status;
	void* m_result;
	std::function<void(void*)> m_deleteFunction;
};

NodeExecutionResult::NodeExecutionResult(NodeExecutionResult::ResultStatus status)
    : m_implementation(std::make_unique<NodeExecutionResult::Impl>(status))
{
}

NodeExecutionResult::~NodeExecutionResult() {}

NodeExecutionResult::ResultStatus NodeExecutionResult::GetResultStatus() const
{
	return m_implementation->GetResultStatus();
}

const void* NodeExecutionResult::GetResult() const { return m_implementation->GetResult(); }

void NodeExecutionResult::SetResult(void* result, std::function<void(void*)> deleteFunction)
{
	m_implementation->SetResult(result, deleteFunction);
}
}  // namespace selector
