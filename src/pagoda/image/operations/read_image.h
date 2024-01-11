#pragma once

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::image::operations
{
class ReadImage : public objects::ProceduralOperation
{
	public:
	static const std::string outputImage;
	static const char* name;

	ReadImage(objects::ProceduralObjectSystemPtr objectSystem);
	~ReadImage() override = default;

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_output;

  std::string m_path;
};
}  // namespace pagoda::geometry::operations
