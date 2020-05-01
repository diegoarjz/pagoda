#ifndef PAGODA_PARAMETER_VARIABLE_H_
#define PAGODA_PARAMETER_VARIABLE_H_

#include <list>
#include <string>

namespace pagoda
{
/**
 * Represents a \c Variable in an \c Expression.
 *
 * Variables can be simple, having only one identifier, or compound, having multiple
 * identifiers separated by a dot (e.g., "object.scope.x")
 */
class Variable
{
public:
	/**
	 * Hashes the \c Variable so that it can be used in unordered_sets.
	 */
	struct Hash
	{
		size_t operator()(const Variable &v) const { return std::hash<std::string>()(v.ToString()); }
	};

	/**
	 * Creates a \c Variable given its string representation.
	 * Can be a compound variable.
	 */
	explicit Variable(const std::string &compoundVariable);
	/**
	 * Creates a \c Variable given its list of identifiers.
	 */
	explicit Variable(const std::list<std::string> &identifiers);

    Variable(const Variable& v);

	~Variable();

	/**
	 * Returns the list of identifiers.
	 */
	const std::list<std::string> &GetIdentifiers() const;

	/**
	 * Creates a string repreenting this variable.
	 */
	const std::string ToString() const;

	/**
	 * Adds an identifier to this \v Variable.
	 */
	void AddIdentifier(const std::string &i);

	bool IsCompound() const;

	bool operator==(const Variable &v) const;
	bool operator!=(const Variable &v) const;
	bool operator==(const std::string &v) const;
	bool operator!=(const std::string &v) const;

private:
	std::list<std::string> m_identifiers;  ///< The list of identifiers.
};
}  // namespace pagoda

#endif
