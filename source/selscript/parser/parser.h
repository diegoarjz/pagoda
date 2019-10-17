#pragma once

#include <memory>

namespace sscript
{
namespace ast
{
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

class Parser
{
public:
	Parser();

	ast::ProgramPtr Parse(const std::string &source);

private:
};
}  // namespace sscript
