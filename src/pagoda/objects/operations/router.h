#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::objects::operations
{
class Router : public ProceduralOperation
{
	public:
	Router(ProceduralObjectSystemPtr objectSystem);
	virtual ~Router();

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::objects::operations
