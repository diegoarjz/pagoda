#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace pagoda
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

/**
 * Holds a table of \c DynamicValueBase.
 */
class DynamicValueTable
{
public:
	/**
	 * An entry for the table.
	 */
	struct Entry
	{
		DynamicValueBasePtr m_value;
	};

	/**
	 * Don't allow to create a \c DynamicValueTable without a name.
	 */
	DynamicValueTable() = delete;
	/**
	 * Create a \c DynamicValueTable with the given name.
	 */
	explicit DynamicValueTable(const std::string &tableName);
	/**
	 * Create a \c DynamicValueTable with the name and parent \c DynamicValueTable.
	 */
	DynamicValueTable(const std::string &tableName, const std::shared_ptr<DynamicValueTable> &parentTable);
	DynamicValueTable(const DynamicValueTable &) = delete;
	DynamicValueTable &operator=(const DynamicValueTable &) = delete;

	/**
	 * Declare a \c DynamicValueBase in this table with the given \p name.
	 */
	void Declare(const std::string &name, DynamicValueBasePtr value);
	/**
	 * Assign the \p value to the \p name.
	 */
	void Assign(const std::string &name, DynamicValueBasePtr value);
	/**
	 * Gets the \c DynamicValueBase with mapped to the \p name.
	 */
	DynamicValueBasePtr Get(const std::string &name);

	/**
	 * Returns the parent \c DynamicValueTable.
	 */
	std::shared_ptr<DynamicValueTable> GetParent() const;
	/**
	 * Sets the parent \c DynamicValueTable.
	 */
	void SetParent(const std::shared_ptr<DynamicValueTable> &parent);

	/// Iterator over the stored values.
	using iterator = std::unordered_map<std::string, Entry>::iterator;

	iterator begin();
	iterator end();

	void DumpSymbols(std::ostream &out) const;

private:
	Entry &FindValue(const std::string &name);

	std::unordered_map<std::string, Entry> m_values;
	std::weak_ptr<DynamicValueTable> m_parentTable;
	std::weak_ptr<DynamicValueTable> m_globals;
	std::string m_tableName;
};

}  // namespace pagoda
