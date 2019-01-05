#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_TYPE_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_TYPE_H

namespace selector
{
enum class NodeType
{
	InputInterface,
	OutputInterface,
	Operation,
	MAX,
	Custom
};
}
#endif
