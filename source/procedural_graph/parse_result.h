#ifndef PAGODA_PROCEDURAL_GRAPH_XML_PARSE_RESULT_H_
#define PAGODA_PROCEDURAL_GRAPH_XML_PARSE_RESULT_H_

namespace pagoda
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
}  // namespace pagoda

#endif
