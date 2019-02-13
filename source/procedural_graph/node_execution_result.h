#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_EXECUTION_RESULT_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_EXECUTION_RESULT_H_

#include <functional>
#include <memory>

namespace selector
{
/**
 * @brief Result of executing a \c Node.
 *
 * When a \c Node object is executed the value returned by its \c NodeExecution object's
 * Execute method is stored in the \c Node and can be accessed by other \c Node objects.
 *
 * This class takes ownership of the value passed in through SetResult and the contained value
 * is destroyed whevener a new value is set or when the \c NodeExecutionResult is destroyed.
 */
class NodeExecutionResult
{
public:
	/**
	 * Status after execution.
	 */
	enum class ResultStatus
	{
		Success,  ///< Reports a successful \c Node execution.
		Failure   ///< Reports a failed \c Node execution.
	};

	/**
	 * Constructs a \c NodeExecution with the given \c ResultStatus.
	 *
	 * @param [in] status The \c ResultStatus for the execution.
	 */
	explicit NodeExecutionResult(ResultStatus status);

	/**
	 * Destroys the \c NodeExecution.
	 *
	 * The deleteFunction parameter in the SetResult member function is called to
	 * destroy the contained value.
	 */
	~NodeExecutionResult();

	/**
	 * Returns the \c ResultStatus.
	 */
	ResultStatus GetResultStatus() const;

	/**
	 * Returns a pointer to the contained result value.
	 *
	 * If no result has been set, it returns a nullptr.
	 *
	 * @return Pointer to the contained result value if any, nullptr otherwise.
	 */
	const void* GetResult() const;

	/**
	 * Sets the contained value to \p result.
	 *
	 * The \p deleteFunction argument is used to delete the value when a new value is set
	 * or the \c NodeExecutionResult is destroyed.
	 *
	 * @param [in] result         A void pointer to the result.
	 * @param [in] deleteFunction A function that is used to delete the contained value.
	 */
	void SetResult(void* result, std::function<void(void*)> deleteFunction);

	template<typename T>
	void SetResultAs(T* result)
	{
		SetResult(result, [](void* value) { delete static_cast<T*>(value); });
	}

	template<typename T>
	const T* GetResultAs() const
	{
		return static_cast<const T*>(GetResult());
	}

private:
	explicit NodeExecutionResult(const NodeExecutionResult&) = delete;
	NodeExecutionResult& operator=(const NodeExecutionResult&) = delete;

	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace selector

#endif
