#ifndef PAGODA_COMMON_DEBUG_STATISTICS_H_
#define PAGODA_COMMON_DEBUG_STATISTICS_H_

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>

namespace pagoda::common::instrument
{
/**
 * Main class responsible for holding statistics about execution.
 *
 * StatisticsManager is a singleton class.
 *
 * It's composed of a number of StatsGroup which, in turn, are composed
 * of StatsContainer which are reponsible for logging and registering
 * values of a determined statistic.
 *
 * New StatsGroup should subclass StatsGroup which will register itself
 * in the StatisticsManager class. StatsGroup is responsible for writing
 * out the respective statistics.
 */
class StatisticsManager
{
	public:
	struct StatsGroup;

	using NameType = std::string;
	using StatsGroupContainerType = std::map<NameType, StatsGroup *>;

	/// Returns the StatisticsManager instance.
	static StatisticsManager *Instance();

	/**
	 * Base class for holding a statistic value.
	 *
	 * Holds a name for a stat and subclasses are responsible for
	 * holding values and writing out the values to a stream.
	 */
	struct StatsContainer
	{
		explicit StatsContainer(const NameType &name) : m_name(name) {}
		virtual ~StatsContainer() {}
		NameType m_name;

		virtual void Write(std::ostream &outStream) = 0;
	};  // struct StatsContainer

	/**
	 * StatsContainer that accumulates (sums) values every time
	 * a stat is logged.
	 */
	template<typename ValType>
	struct AccumulatorContainer : public StatsContainer
	{
		explicit AccumulatorContainer(const NameType &name) : StatsContainer(name) {}
		void Log(const ValType &val) { m_data += val; }
		ValType m_data;

		void Write(std::ostream &outStream) override { outStream << "\"" << m_data << "\""; }
	};  // struct AccumulatorContainer

	/**
	 * Base class for a group of statistics.
	 *
	 * Mainly used for organising statistics.
	 */
	struct StatsGroup
	{
		virtual void Write(std::ostream &outStream) = 0;
		virtual std::string Name() const = 0;
	};  // struct StatsGroup

	/**
	 * Registers a StatsGroup with the StatisticsManager.
	 */
	void AddGroup(StatsGroup *group);
	/**
	 * Writes the statistics to outStream in YAML format.
	 *
	 * Delegates specifics to each registered StatsGroup.
	 */
	void Write(std::ostream &outStream);

	private:
	StatsGroupContainerType m_statsGroup;
};  // class StatisticsManager

/**
 * Special StatsGroup for OneShotProfiler.
 *
 * Each OneShotProfiler will register a new StatsContainer to this
 * class.
 */
struct OneShotProfilerStats : public StatisticsManager::StatsGroup
{
	public:
	static OneShotProfilerStats *Instance();

	void Write(std::ostream &outStream) final;
	std::string Name() const override { return "OneShotProfilerStats"; }

	template<class StatsContainerType>
	std::shared_ptr<StatsContainerType> AddContainer(const std::string &containerName)
	{
		// TODO: variadic templates for container construction
		auto iter = m_stats.find(containerName);

		std::shared_ptr<StatsContainerType> container;
		if (iter == m_stats.end()) {
			container = std::make_shared<StatsContainerType>(containerName);
			m_stats.emplace(containerName, container);
		} else {
			container = std::dynamic_pointer_cast<StatsContainerType>(m_stats[containerName]);
		}

		return container;
	}
	std::map<std::string, std::shared_ptr<StatisticsManager::StatsContainer>> m_stats;

	private:
	OneShotProfilerStats() { StatisticsManager::Instance()->AddGroup(this); }
};  // struct OneShotProfilerStats

#ifdef PAGODA_STATISTICS_ENABLED

#define LOG_STATISTICS(STAT_INSTRUCTION) (STAT_INSTRUCTION);

#else

#define LOG_STATISTICS(STAT_INSTRUCTION)

#endif

}  // namespace pagoda::common::instrument

#endif
