#include "ast_printer.h"

namespace sscript
{
AstPrinter::AstPrinter() : m_indentation(0) {}

void AstPrinter::Visit(ast::FloatPtr n)
{
	indent();
	std::cout << indentation() << "<Float:" << n->GetNumber() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::IntegerPtr n)
{
	indent();
	std::cout << indentation() << "<Integer:" << n->GetInteger() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::StringPtr s)
{
	indent();
	std::cout << indentation() << "<String:" << s->GetString() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::IdentifierPtr i)
{
	indent();
	std::cout << indentation() << "<Identifier:" << i->GetIdentifier() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::BooleanPtr b)
{
	indent();
	std::cout << indentation() << "<Boolean:" << (b->GetBoolean() ? "true" : "false") << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::Nullptr)
{
	indent();
	std::cout << indentation() << "<Null>" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::ArithmeticOpPtr a)
{
	static const std::string ops[] = {"+", "-", "*", "/"};
	indent();
	std::cout << indentation() << "<ArithmeticOp:" << ops[static_cast<uint32_t>(a->GetOperationType())] << ">"
	          << std::endl;
	a->GetLhs()->AcceptVisitor(this);
	a->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::UnaryPtr u)
{
	static const std::string ops[] = {"!", "-"};
	indent();
	std::cout << indentation() << "<Unary:" << ops[static_cast<uint32_t>(u->GetOperationType())] << ">" << std::endl;
	u->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ComparisonOpPtr c)
{
	static const std::string ops[] = {"==", "!=", ">", ">=", "<", "<="};
	indent();
	std::cout << indentation() << "<Comparison:" << ops[static_cast<uint32_t>(c->GetOperationType())] << ">"
	          << std::endl;
	c->GetLhs()->AcceptVisitor(this);
	c->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::LogicOpPtr l)
{
	static const std::string ops[] = {"and", "or"};
	indent();
	std::cout << indentation() << "<Logic:" << ops[static_cast<uint32_t>(l->GetOperationType())] << ">" << std::endl;
	l->GetLhs()->AcceptVisitor(this);
	l->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::AssignmentPtr a)
{
	indent();
	std::cout << indentation() << "<Assignment>" << std::endl;
	a->GetIdentifier()->AcceptVisitor(this);
	a->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ExpressionStatementPtr e)
{
	indent();
	std::cout << indentation() << "<Expression>" << std::endl;
	e->GetExpression()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::IfStatementPtr i)
{
	indent();
	std::cout << indentation() << "<IfStatement>" << std::endl;
	i->GetCondition()->AcceptVisitor(this);
	i->GetTrueStatement()->AcceptVisitor(this);
	auto falseStatement = i->GetFalseStatement();
	if (falseStatement)
	{
		falseStatement->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::LoopPtr l)
{
	indent();
	std::cout << indentation() << "<Loop>" << std::endl;
	l->GetCondition()->AcceptVisitor(this);
	l->GetBody()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::VarDeclPtr v)
{
	indent();
	std::cout << indentation() << "<VarDecl>" << std::endl;
	v->GetIdentifier()->AcceptVisitor(this);
	auto rhs = v->GetRhs();
	if (rhs)
	{
		rhs->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::StatementBlockPtr b)
{
	indent();
	std::cout << indentation() << "<StatementBlock>" << std::endl;
	for (auto &s : b->GetStatements())
	{
		s->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::CallPtr c)
{
	indent();
	std::cout << indentation() << "<Call>" << std::endl;
	c->GetCallee()->AcceptVisitor(this);
	for (auto &arg : c->GetArguments())
	{
		arg->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::FunctionDeclarationPtr f)
{
	indent();
	std::cout << indentation() << "<FunctionDeclaration>" << std::endl;
	f->GetIdentifier()->AcceptVisitor(this);
	for (auto &par : f->GetParameters())
	{
		par->AcceptVisitor(this);
	}
	f->GetFunctionBody()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ParameterPtr par)
{
	indent();
	std::cout << indentation() << "<Parameter:" << par->GetIdentifier() << "(";
	if (par->GetParameterType())
	{
		std::cout << *(par->GetParameterType());
	}
	else
	{
		std::cout << "any";
	}
	std::cout << ")>" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::ReturnPtr r)
{
	indent();
	std::cout << indentation() << "<Return>" << std::endl;
	auto expression = r->GetReturnExpression();
	if (expression)
	{
		expression->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::ClassDeclarationPtr classDecl)
{
	indent();
	std::cout << indentation() << "<ClassDecl>" << std::endl;
	classDecl->GetIdentifier()->AcceptVisitor(this);

	for (const auto &m : classDecl->GetMethods())
	{
		m->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::AnonymousMethodPtr a)
{
	indent();
	std::cout << indentation() << "<AnonymousMethod>" << std::endl;
	a->GetInstance()->AcceptVisitor(this);
	a->GetBody()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::GetExpressionPtr e)
{
	indent();
	std::cout << indentation() << "<GetExpression>" << std::endl;
	e->GetLhs()->AcceptVisitor(this);
	e->GetIdentifier()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::SetExpressionPtr e)
{
	indent();
	std::cout << indentation() << "<GetExpression>" << std::endl;
	e->GetLhs()->AcceptVisitor(this);
	e->GetIdentifier()->AcceptVisitor(this);
	e->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ProgramPtr p)
{
	std::cout << "<Program>" << std::endl;
	for (auto &s : p->GetStatements())
	{
		s->AcceptVisitor(this);
	}
}

void AstPrinter::indent() { ++m_indentation; }

std::string AstPrinter::indentation() { return std::string(m_indentation, ' '); }
void AstPrinter::deindent() { --m_indentation; }
}  // namespace sscript
