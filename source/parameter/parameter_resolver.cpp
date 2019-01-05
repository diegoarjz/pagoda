#include "parameter_resolver.h"

#include "context.h"
#include "parameter.h"
#include "parameter_validation.h"

#include <vector>

#include <boost/algorithm/string.hpp>

#include <iostream>

namespace selector
{
ParameterPath::PathComponent::PathComponent() : m_componentType(ParameterPath::PathComponent::Type::None), m_name("") {}

ParameterPath::PathComponent::PathComponent(const Type &t) : m_componentType(t), m_name("") {}

ParameterPath::PathComponent::PathComponent(const std::string &name)
    : m_componentType(ParameterPath::PathComponent::Type::Name), m_name(name)
{
}

bool ParameterPath::IsValidPath(const std::string &path)
{
	return std::regex_match(path, ParameterValidation::path_regex);
}

ParameterPath ParameterPath::CreatePath(const std::string &path)
{
	// TODO: path validation
	return ParameterPath(path);
}

ParameterPath::ParameterPath(const std::string &path)
{
	START_PROFILE;

	std::vector<std::string> components;
	boost::split(components, path, [](char c) { return c == '/'; });
	uint32_t start = 0;

	if (path[0] == '/')
	{
		m_pathComponents.resize(components.size());
		m_pathComponents[0] = PathComponent(PathComponent::Type::Root);
		start++;
	}
	else
	{
		m_pathComponents.resize(components.size());
	}

	for (auto i = start; i < components.size() - 1; ++i)
	{
		auto c = components[i];

		if (c.empty())
		{
			continue;
		}

		if (c == "..")
		{
			m_pathComponents[i] = PathComponent(PathComponent::Type::Parent);
		}
		else if (c == ".")
		{
			m_pathComponents[i] = PathComponent(PathComponent::Type::Current);
		}
		else
		{
			m_pathComponents[i] = PathComponent(c);
		}
	}

	m_pathComponents.back() = PathComponent(components.back());
}

ParameterResolver::ParameterResolver(std::shared_ptr<Context> root_context) : m_rootContext(root_context) {}

std::shared_ptr<ParameterBase> ParameterResolver::Resolve(const std::string &path,
                                                          std::shared_ptr<Context> current_context)
{
	START_PROFILE;

	std::vector<std::string> path_components;
	boost::split(path_components, path, [](char c) { return c == '/'; });

	// TODO: path validation
	std::shared_ptr<Context> current;
	if (path[0] == '/')
	{
		current = m_rootContext;
	}
	else
	{
		current = current_context;
	}

	for (auto i = 0u; i < path_components.size() - 1; ++i)
	{
		auto c = path_components[i];

		if (c.empty())
		{
			continue;
		}

		if (c == "..")
		{
			current = current->Parent();
		}
		else if (c != ".")
		{
			current = current->GetSubContext(c);
		}

		if (current == nullptr)
		{
			return nullptr;
		}
	}

	return current->GetParameter(path_components.back());
}

std::shared_ptr<ParameterBase> ParameterResolver::Resolve(const ParameterPath &path,
                                                          std::shared_ptr<Context> current_context)
{
	START_PROFILE;

	std::shared_ptr<Context> current = current_context;
	auto &components = path.GetComponents();

	for (auto i = 0u; i < components.size() - 1; ++i)
	{
		auto &c = components[i];

		switch (c.m_componentType)
		{
			case ParameterPath::PathComponent::Type::Root:
				current = m_rootContext;
				break;
			case ParameterPath::PathComponent::Type::Parent:
				current = current->Parent();
				break;
			case ParameterPath::PathComponent::Type::Current:
				break;
			case ParameterPath::PathComponent::Type::Name:
				current = current->GetSubContext(c.m_name);
				break;
			default:
				return nullptr;
		}

		if (current == nullptr)
		{
			return nullptr;
		}
	}

	return current->GetParameter(components.back().m_name);
}

}  // namespace selector
