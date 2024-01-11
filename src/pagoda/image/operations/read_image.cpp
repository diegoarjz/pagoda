#include "read_image.h"

#include "pagoda/image/image_system.h"

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/parameter_callback.h>
#include "pagoda/objects/procedural_object_system.h"

namespace pagoda::image::operations {

const std::string ReadImage::outputImage{"out"};
const char *ReadImage::name = "ReadImage";

ReadImage::ReadImage(objects::ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {}

void ReadImage::Parameters(objects::NewParameterCallback *cb) {
  cb->StringParameter(&m_path, "path", "Path", "");
}

const std::string &ReadImage::GetOperationName() const {
  static const std::string sName{name};
  return sName;
}

void ReadImage::Interfaces(objects::InterfaceCallback *cb) {
  cb->OutputInterface(m_output, outputImage, "Out",
                      objects::Interface::Arity::One);
}

void ReadImage::DoWork() {
  auto imageSystem = m_proceduralObjectSystem->GetComponentSystem<ImageSystem>();

  auto image = Image::CreateFromFile(m_path, Image::Format::RGBA8);

  auto object = CreateOutputProceduralObject();
  auto component = imageSystem->CreateComponentAs<ImageComponent>(object);

  component->SetImage(image);

  m_output->Set(object);
}
} // namespace pagoda::image::operations
