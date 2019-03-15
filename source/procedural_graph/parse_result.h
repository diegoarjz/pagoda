#ifndef SELECTOR_PROCEDURAL_GRAPH_XML_PARSE_RESULT_H_
#define SELECTOR_PROCEDURAL_GRAPH_XML_PARSE_RESULT_H_

namespace selector
{
struct ParseResult
{
	enum class Status
	{
		Ok,
		FileNotFound,
		IOError,
		OutOfMemory,
		InternalError,
		UnrecognizedTag,
		BadElement,
		EndElementMismatch,
		NoDocumentElement,

		VersionMismatch,
		GraphElementNotFound,
		VersionElementNotFound,
		DuplicatedNodeId,
		UnknownNodeType,
		InvalidParameter,

		UnknownError,
	};
	Status status;
	uint32_t offset;
};
}  // namespace selector

#endif
