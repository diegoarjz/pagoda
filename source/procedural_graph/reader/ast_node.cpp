#include "ast_node.h"

namespace selector
{
AstNode::AstNode(const Offset_t &startOffset, const Offset_t &endOffset)
	: m_startOffset(startOffset), m_endOffset(endOffset)
{
}

AstNode::AstNode(const NodeRange_t &nodeRange)
	: m_startOffset(std::get<0>(nodeRange)), m_endOffset(std::get<1>(nodeRange))
{
}

AstNode::~AstNode()
{
}

void AstNode::SetStartOfset(const Offset_t &offset)
{
	m_startOffset = offset;
}

void AstNode::SetEndOffset(const Offset_t &offset)
{
	m_endOffset = offset;
}

const Offset_t& AstNode::GetStartOffset() const
{
	return m_startOffset;
}

const Offset_t& AstNode::GetEndOFfset() const
{
	return m_endOffset;
}

void AstNode::SetNodeRange(const NodeRange_t &range)
{
	m_startOffset = std::get<0>(range);
	m_endOffset = std::get<1>(range);
}

NodeRange_t AstNode::GetNodeRange() const
{
	return std::make_pair(m_startOffset, m_endOffset);
}

}