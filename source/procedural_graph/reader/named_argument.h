#ifndef SELECTOR_PROCEDURAL_GRAPH_NAMED_ARGUMENT_H_
#define SELECTOR_PROCEDURAL_GRAPH_NAMED_ARGUMENT_H_

#include "ast_node.h"

#include <string>

namespace selector
{
/**
 * Represents a named argument (both construction and execution arguments) in the graph format.
 */
class NamedArgument : public AstNode
{
public:
	/**
	 * Types of Arguments.
	 * Should match the types of \c Parameter.
	 */
	enum class ArgumentType
	{
		String,
		Float,
		Expression
	};

	/**
	 * Constructs a \c NamedArgument with the \p name, \p type and \p value.
	 */
	NamedArgument(const std::string &name, const ArgumentType type, const std::string &value);
	/**
	 * Constructs a \c NamedArgument with the \p name, \p type and \p value passing the \p startOffset and \p endOffset
	 * to the respective constructor of \c AstNode.
	 */
	NamedArgument(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset, const std::string &name,
	              const ArgumentType type, const std::string &value);
	/**
	 * Constructs a \c NamedArgument with the \p name, \p type and \p value passing the \p range to the respective
	 * constructor of \c AstNode.
	 */
	NamedArgument(const AstNode::NodeRange_t &range, const std::string &name, const ArgumentType type,
	              const std::string &value);

	virtual ~NamedArgument();

	/**
	 * Getter for the name.
	 */
	const std::string &GetName() const;
	/**
	 * Getter for the name.
	 */
	std::string &GetName();
	/**
	 * Setter for the name.
	 */
	void SetName(const std::string &name);

	/**
	 * Getter for the Argument Type.
	 */
	const ArgumentType &GetArgumentType() const;
	/**
	 * Getter for the Argument Type.
	 */
	ArgumentType &GetArgumentType();
	/**
	 * Setter for the Argument Type.
	 */
	void SetArgumentType(const ArgumentType &type);

	/**
	 * Getter for the Argument Value.
	 */
	const std::string &GetArgumentValue() const;
	/**
	 * Getter for the Argument Value.
	 */
	std::string &GetArgumentValue();
	/**
	 * Setter for the Argument Value.
	 */
	void SetArgumentValue(const std::string &argumentValue);

	/**
	 * Allows \c AstNodeVisitor to visit this node type.
	 */
	void AcceptVisitor(AstNodeVisitor *visitor) override;
private:
	std::string m_name;           ///< The name for this \c NamedArgument
	ArgumentType m_argumentType;  ///< The type of the \c NamedArgument
	std::string m_argumentValue;  ///< The string representation of the value of the \c NamedArgument
};
}  // namespace selector

#endif
