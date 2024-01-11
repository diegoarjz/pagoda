#include "pagoda/image/image_system.h"

#include "operations/read_image.h"

#include "pagoda/objects/operation_factory.h"
#include "pagoda/objects/procedural_object_system.h"

#include "pagoda/pagoda.h"

namespace pagoda::image {
std::string ImageComponent::GetComponentSystemName() {
  return ImageSystem::GetComponentSystemName();
}

const std::string ImageSystem::GetComponentSystemName() {
  return "ImageSystem";
}

ImageSystem::ImageSystem()
    : ProceduralComponentSystem(GetComponentSystemTypeName()) {
  //
}

void ImageSystem::DoClone(std::shared_ptr<ImageComponent> from,
                          std::shared_ptr<ImageComponent> to) {
  //
}

void ImageSystem::Registration(Pagoda *pagoda) {
  LOG_TRACE(Core, "Registering " << GetComponentSystemName());

  auto operationFactory = pagoda->GetOperationFactory();
  auto objectSystem = pagoda->GetProceduralObjectSystem();

  objectSystem->RegisterProceduralComponentSystem(
      std::make_shared<ImageSystem>());

  operationFactory->Register("ReadImage", [objectSystem]() {
    return std::make_shared<operations::ReadImage>(objectSystem);
  });
}
} // namespace pagoda::image
