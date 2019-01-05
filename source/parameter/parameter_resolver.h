#ifndef SELECTOR_PARAMETER_PARAMETER_RESOLVER_H_
#define SELECTOR_PARAMETER_PARAMETER_RESOLVER_H_

#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace selector
{
class Context;
class ParameterBase;

class ParameterPath
{
public:
	struct PathComponent
	{
		enum class Type
		{
			/// References the root context
			Root,
			/// References the parent context. Same as ".."
			Parent,
			/// References the current context. Same as "."
			Current,
			/// References a name, either context name or parameter name.
			Name,
			/// Invalid
			None
		};

		PathComponent();
		explicit PathComponent(const Type &t);
		explicit PathComponent(const std::string &name);

		Type m_componentType;
		std::string m_name;
	};  // struct PathComponent

	static bool IsValidPath(const std::string &path);
	static ParameterPath CreatePath(const std::string &path);
	const std::vector<PathComponent> &GetComponents() const { return m_pathComponents; }

	friend class ParameterResolver;

private:
	std::vector<PathComponent> m_pathComponents;

	explicit ParameterPath(const std::string &path);

};  // class ParameterPath

class ParameterResolver
{
public:
	explicit ParameterResolver(std::shared_ptr<Context> root_context);

	std::shared_ptr<ParameterBase> Resolve(const std::string &path, std::shared_ptr<Context> current_context);
	std::shared_ptr<ParameterBase> Resolve(const ParameterPath &path, std::shared_ptr<Context> current_context);

private:
	std::shared_ptr<Context> m_rootContext;
};  // class ParameterResolver

}  // namespace selector

#endif
