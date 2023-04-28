
#include "nndeploy/source/inference/tensor_rt/tensor_rt_config.h"

namespace nndeploy {
namespace inference {

static TypeConfigRegister<TypeConfigCreator<MnnConfigImpl>>
    g_tensor_rt_config_register(base::kInferenceTypeMnn);

base::Status MnnConfigImpl::jsonToConfig(const std::string &json,
                                         bool is_path) {
  base::Status status = DefaultConfigImpl::jsonToConfig(json, is_path);
  if (status != base::kStatusCodeOk) {
    // TODO: log
    return status;
  }

  return base::kStatusCodeOk;
}

base::Status MnnConfigImpl::set(const std::string &key, base::Value &value) {
  base::Status status = base::kStatusCodeOk;
  if (key == "library_path") {
    uint8_t *tmp = nullptr;
    if (value.get(&tmp)) {
      library_path_ = std::string(reinterpret_cast<char *>(tmp));
    } else {
      status = base::kStatusCodeErrorInvalidParam;
    }
  }
  return base::kStatusCodeOk;
}

base::Status MnnConfigImpl::get(const std::string &key, base::Value &value) {
  base::Status status = base::kStatusCodeOk;
  return base::kStatusCodeOk;
}

}  // namespace inference
}  // namespace nndeploy
