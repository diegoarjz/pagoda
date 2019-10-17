#include "ast.h"

#include "ast_visitor.h"

namespace sscript
{
namespace ast
{
Float::~Float() {}
void Float::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Float>(shared_from_this())); }

Integer::~Integer() {}
void Integer::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Integer>(shared_from_this())); }

String::~String() {}
void String::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<String>(shared_from_this())); }

Identifier::~Identifier() {}
void Identifier::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Identifier>(shared_from_this())); }

Boolean::~Boolean() {}
void Boolean::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Boolean>(shared_from_this())); }

Null::~Null() {}
void Null::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Null>(shared_from_this())); }

ArithmeticOp::~ArithmeticOp() {}
void ArithmeticOp::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ArithmeticOp>(shared_from_this()));
}

Unary::~Unary() {}
void Unary::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Unary>(shared_from_this())); }

ComparisonOp::~ComparisonOp() {}
void ComparisonOp::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ComparisonOp>(shared_from_this()));
}

LogicOp::~LogicOp() {}
void LogicOp::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<LogicOp>(shared_from_this())); }

Assignment::~Assignment() {}
void Assignment::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Assignment>(shared_from_this())); }

ExpressionStatement::~ExpressionStatement() {}
void ExpressionStatement::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ExpressionStatement>(shared_from_this()));
}

IfStatement::~IfStatement() {}
void IfStatement::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<IfStatement>(shared_from_this())); }

Loop::~Loop() {}
void Loop::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Loop>(shared_from_this())); }

VarDecl::~VarDecl() {}
void VarDecl::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<VarDecl>(shared_from_this())); }

StatementBlock::~StatementBlock() {}
void StatementBlock::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<StatementBlock>(shared_from_this()));
}

Call::~Call() {}
void Call::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Call>(shared_from_this())); }

void Return::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Return>(shared_from_this())); }
Return::~Return() {}

Program::~Program() {}
void Program::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Program>(shared_from_this())); }
}  // namespace ast
}  // namespace sscript
