
#include "nndeploy/include/architecture/architecture.h"

namespace nndeploy {
namespace architecture {

Architecture::Architecture(base::DeviceTypeCode device_type_code)
    : device_type_code_(device_type_code) {}

Architecture::~Architecture() {}

std::map<base::DeviceTypeCode, std::shared_ptr<Architecture>>&
getArchitectureMap() {
  static std::once_flag once;
  static std::shared_ptr<
      std::map<base::DeviceTypeCode, std::shared_ptr<Architecture>>>
      architecture_map;
  std::call_once(once, []() {
    architecture_map.reset(
        new std::map<base::DeviceTypeCode, std::shared_ptr<Architecture>>);
  });
  return *architecture_map;
}

Architecture* getArchitecture(base::DeviceTypeCode type) {
  return getArchitectureMap()[type].get();
}

}  // namespace architecture
}  // namespace nndeploy