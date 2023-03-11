
#ifndef _NNDEPLOY_INCLUDE_BASE_STATUS_H_
#define _NNDEPLOY_INCLUDE_BASE_STATUS_H_

#include "nndeploy/include/base/include_c_cpp.h"
#include "nndeploy/include/base/log.h"
#include "nndeploy/include/base/macro.h"

namespace nndeploy {
namespace base {

enum StatusCode : int32_t {
  kStatusCodeOk = 0x0000,

  kStatusCodeErrorUnknown,

  kStatusCodeErrorOutOfMemory,
  kStatusCodeErrorNotSupport,
  kStatusCodeErrorNotImplement,
  kStatusCodeErrorInvalidValue,
  kStatusCodeErrorInvalidParam,

  // device
  kStatusCodeErrorDeviceCpu,
  kStatusCodeErrorDeviceArm,
  kStatusCodeErrorDeviceX86,
  kStatusCodeErrorDeviceCuda,
  kStatusCodeErrorDeviceOpenCL,
  kStatusCodeErrorDeviceOpenGL,
  kStatusCodeErrorDeviceMetal,

  // inference
  kStatusCodeErrorInferenceTnn,
  kStatusCodeErrorInferenceMnn,
};

inline std::ostream& operator<<(std::ostream& out, const StatusCode& v) {
  switch (v) {
    NNDEPLOY_ENUM_TO_STR(kStatusCodeOk);
    default:
      out << static_cast<int>(v);
      break;
  }
  return out;
};

class Status {
 public:
  ~Status();
  Status(int32_t code = kStatusCodeOk);

  Status& operator=(int32_t code);
  bool operator==(int32_t code_);
  bool operator!=(int32_t code_);
  operator int32_t();
  operator bool();

  std::string desc();

 private:
  int32_t code_ = kStatusCodeOk;
};

#define ASSERT(x)                     \
  {                                   \
    int res = (x);                    \
    if (!res) {                       \
      LOGE("Error: assert failed\n"); \
      assert(res);                    \
    }                                 \
  }

#ifndef _DEBUG
#undef NNDEPLOY_LOGDT
#undef NNDEPLOY_LOGD
#define NNDEPLOY_LOGDT(fmt, tag, ...)
#define NNDEPLOY_LOGD(fmt, ...)
#undef NNDEPLOY_ASSERT
#define NNDEPLOY_ASSERT(x)
#endif  // _DEBUG

#define NNDEPLOY_RETURN_VALUE_ON_NEQ(status, expected, value) \
  do {                                                        \
    if (status != (expected)) {                               \
      return (value);                                         \
    }                                                         \
  } while (0)

#define NNDEPLOY_RETURN_VALUE_ON_EQ(status, expected, value) \
  do {                                                       \
    if (status == (expected)) {                              \
      return (value);                                        \
    }                                                        \
  } while (0)

#define NNDEPLOY_RETURN_ON_NEQ(status, expected) \
  do {                                           \
    if (status != (expected)) {                  \
      return status;                             \
    }                                            \
  } while (0)

#define NNDEPLOY_RETURN_ON_EQ(status, expected) \
  do {                                          \
    if (status == (expected)) {                 \
      return status;                            \
    }                                           \
  } while (0)

#define NNDEPLOY_CHECK_PARAM_NULL(param)           \
  do {                                             \
    if (!param) {                                  \
      return Status(kStatusCodeErrorInvalidParam); \
    }                                              \
  } while (0)

}  // namespace base
}  // namespace nndeploy

#endif  // _NNDEPLOY_INCLUDE_BASE_STATUS_H_
