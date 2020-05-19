#ifndef PAGODA_PROCEDURAL_GRAPH_AST_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_AST_NODE_H_

#include <memory>

namespace pagoda::graph::io
{
class AstNodeVisitor;

/**
 * Represents an Abstract Syntax Tree node for the graph file format.
 */
class AstNode : public std::enable_shared_from_this<AstNode>
{
public:
	/// Type pointing to a position within the graph format string.
	using Offset_t = std::size_t;
	/// Type indicating a range between two positions within the graph format string.
	using NodeRange_t = std::pair<Offset_t, Offset_t>;

	AstNode();
	AstNode(const AstNode &) = delete;
	AstNode &operator=(const AstNode &) = delete;

	/**
	 * Constructs the \c AstNode given its start and end offset.
	 */
	AstNode(const Offset_t &startOffset, const Offset_t &endOffset);

	/**
	 * Constructs the \c AstNode given a its \c NodeRange_t.
	 */
	AstNode(const NodeRange_t &nodeRange);

	virtual ~AstNode();

	/**
	 * Sets the Node's start offset.
	 */
	void SetStartOfset(const Offset_t &offset);
	/**
	 * Sets the Node's end offset.
	 */
	void SetEndOffset(const Offset_t &offset);

	/**
	 * Gets the Node's start offset.
	 */
	const Offset_t &GetStartOffset() const;
	/**
	 * Gets the Node's end offset.
	 */
	const Offset_t &GetEndOffset() const;

	/**
	 * Sets the Node's range.
	 */
	void SetNodeRange(const NodeRange_t &range);
	/**
	 * Gets the Node's range.
	 */
	NodeRange_t GetNodeRange() const;

	/**
	 * Allows \c AstNodeVisitor to visit each node type.
	 */
	virtual void AcceptVisitor(AstNodeVisitor *) = 0;

private:
	Offset_t m_startOffset;  ///< Offset to the start of this node in the text
	Offset_t m_endOffset;    ///< Offset to the end of this node in the text
};

using AstNodePtr = std::shared_ptr<AstNode>;

}  // namespace pagoda::graph::io

#endif
