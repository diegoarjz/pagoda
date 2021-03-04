#pragma once

#include <memory>
#include <ostream>

namespace pagoda::graph
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
}  // namespace pagoda::graph

namespace pagoda::graph::io
{
/**
 * Writes a \c Graph to strings.
 */
class GraphWriter
{
	public:
	GraphWriter(GraphPtr graph);
	~GraphWriter();

	/**
	 * Write the graph to the given \p outStream.
	 */
	void Write(std::ostream& outStream);

	private:
	struct Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pagoda::graph::io
