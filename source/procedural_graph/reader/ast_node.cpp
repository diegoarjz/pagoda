#include "ast_node.h"

namespace pagoda
{
AstNode::AstNode() : AstNode(0,0)
{
}

AstNode::AstNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset)
    : m_startOffset(startOffset), m_endOffset(endOffset)
{
}

AstNode::AstNode(const AstNode::NodeRange_t &nodeRange)
    : m_startOffset(std::get<0>(nodeRange)), m_endOffset(std::get<1>(nodeRange))
{
}

AstNode::~AstNode() {}

void AstNode::SetStartOfset(const AstNode::Offset_t &offset) { m_startOffset = offset; }

void AstNode::SetEndOffset(const AstNode::Offset_t &offset) { m_endOffset = offset; }

const AstNode::Offset_t &AstNode::GetStartOffset() const { return m_startOffset; }

const AstNode::Offset_t &AstNode::GetEndOffset() const { return m_endOffset; }

void AstNode::SetNodeRange(const AstNode::NodeRange_t &range)
{
	m_startOffset = std::get<0>(range);
	m_endOffset = std::get<1>(range);
}

AstNode::NodeRange_t AstNode::GetNodeRange() const { return std::make_pair(m_startOffset, m_endOffset); }

}  // namespace pagoda
