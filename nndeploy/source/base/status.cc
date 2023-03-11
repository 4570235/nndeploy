
#include "nndeploy/include/base/status.h"

namespace nndeploy {
namespace base {

Status& Status::operator=(int32_t code) {
  code_ = code;
  return *this;
};

bool Status::operator==(int32_t code_) { return code_ == code_; };

bool Status::operator!=(int32_t code_) { return code_ != code_; };

Status::operator int32_t() { return code_; }

Status::operator bool() { return code_ == kStatusCodeOk; }

std::string Status::desc() {
  std::string str;
  switch (code_) {
    NNDEPLOY_ENUM_TO_STR(kStatusCodeOk);
    default:
      str = std::to_string(static_cast<int>(code_));
      break;
  }
  return str;
};

}  // namespace base
}  // namespace nndeploy
