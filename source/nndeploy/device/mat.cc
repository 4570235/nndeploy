
#include "nndeploy/device/mat.h"

namespace nndeploy {
namespace device {

Mat::Mat() {}
Mat::~Mat() { destory(); }

Mat::Mat(const std::string &name) : name_(name){};

Mat::Mat(const MatDesc &desc, const std::string &name) {
  create(desc, name);
}

Mat::Mat(Device *device, const MatDesc &desc,
         const std::string &name, const base::IntVector &config) {
  create(device, desc, name, config);
}

Mat::Mat(Device *device, const MatDesc &desc, void *data_ptr,
         const std::string &name,
         const base::IntVector &config) {
  create(device, desc, data_ptr, name, config);
}
Mat::Mat(Device *device, const MatDesc &desc, int data_id,
         const std::string &name,
         const base::IntVector &config) {
  create(device, desc, data_id, name, config);
}

Mat::Mat(const MatDesc &desc, Buffer *buffer, const std::string &name) {
  create(desc, buffer, name);
}

//
Mat::Mat(const Mat &mat) {
  if (this == &mat) {
    return;
  }

  desc_ = mat.desc_;
  buffer_ = mat.buffer_;
  if (buffer_ != nullptr && is_external_buffer_ == false) {
    buffer_->addRef();
  }
}
Mat::Mat(Mat &&mat) {
  if (this == &mat) {
    return;
  }
  desc_ = mat.desc_;
  buffer_ = mat.buffer_;
  is_external_buffer_ = mat.is_external_buffer_;
  mat.buffer_ = nullptr;
}

//
Mat &Mat::operator=(const Mat &mat) {
  if (this == &mat) {
    return *this;
  }

  desc_ = mat.desc_;
  buffer_ = mat.buffer_;
  if (buffer_ != nullptr && is_external_buffer_ == false) {
    buffer_->addRef();
  }
  return *this;
}
Mat &Mat::operator==(Mat &&mat) {
  if (this == &mat) {
    return *this;
  }
  desc_ = mat.desc_;
  buffer_ = mat.buffer_;
  is_external_buffer_ = mat.is_external_buffer_;
  mat.buffer_ = nullptr;
  return *this;
}

// create
// 必须确保为空
void Mat::create(const MatDesc &desc, const std::string &name) {
  create(nullptr, desc, nullptr, nullptr, -1, name, base::IntVector());
}

void Mat::create(Device *device, const MatDesc &desc, const std::string &name,
                 const base::IntVector &config) {
  create(device, desc, nullptr, nullptr, -1, name, config);
}

void Mat::create(Device *device, const MatDesc &desc, void *data_ptr,
                 const std::string &name, const base::IntVector &config) {
  create(device, desc, nullptr, data_ptr, -1, name, config);
}
void Mat::create(Device *device, const MatDesc &desc, int data_id,
                 const std::string &name, const base::IntVector &config) {
  create(device, desc, nullptr, nullptr, data_id, name, config);
}

void Mat::create(const MatDesc &desc, Buffer *buffer, const std::string &name) {
  create(nullptr, desc, buffer, nullptr, -1, name, base::IntVector());
}

void Mat::create(Device *device, const MatDesc &desc, Buffer *buffer,
                 void *data_ptr, int data_id, const std::string &name,
                 const base::IntVector &config) {
  name_ = name;
  desc_ = desc;
  if (buffer != nullptr) {
    is_external_buffer_ = true;
    buffer_ = buffer;
    return;
  }
  if (device != nullptr) {
    is_external_buffer_ = false;
    if (data_ptr != nullptr) {
      BufferDesc buffer_desc = device->toBufferDesc(desc, config);
      buffer_ =
          device->create(buffer_desc, data_ptr, kBufferSourceTypeExternal);
      return;
    } else if (data_id != -1) {
      BufferDesc buffer_desc = device->toBufferDesc(desc, config);
      buffer_ = device->create(buffer_desc, data_id, kBufferSourceTypeExternal);
      return;
    } else {
      BufferDesc buffer_desc = device->toBufferDesc(desc, config);
      buffer_ = device->allocate(buffer_desc);
      return;
    }
  }
  return;
}

void Mat::destory() {
  desc_.data_type_ = base::dataTypeOf<float>();
  desc_.shape_.clear();
  desc_.stride_.clear();

  deallocateBuffer();

  is_external_buffer_ = false;
}

void Mat::allocBuffer(Device *device, const base::IntVector &config) {
  BufferDesc dst_buffer_desc = device->toBufferDesc(desc_, config);
  if (buffer_ != nullptr && device == buffer_->getDevice()) {
    BufferDesc src_buffer_desc = buffer_->getDesc();
    if (device->compareBufferDesc(dst_buffer_desc, src_buffer_desc) <= 0) {
      return;
    }
  }
  deallocateBuffer();
  is_external_buffer_ = false;
  buffer_ = device->allocate(dst_buffer_desc);
}
void Mat::deallocateBuffer() {
  if (buffer_ != nullptr && is_external_buffer_ == false) {
    if (buffer_->subRef() == 1) {
      Device *device = buffer_->getDevice();
      device->deallocate(buffer_);
    }
  }
  buffer_ = nullptr;
}

bool Mat::justModify(const MatDesc &desc) {
  if (buffer_ == nullptr) {
    desc_ = desc;
    return true;
  } else {
    // TODO, 做到可以安全修改
    desc_ = desc;
    return true;
  }
}

bool Mat::justModify(Buffer *buffer) {
  // TODO, 做到可以安全修改
  deallocateBuffer();
  is_external_buffer_ = true;
  buffer_ = buffer;
  return true;
}

// get
bool Mat::empty() { return desc_.shape_.empty(); }
bool Mat::isContinue() {
  if (desc_.stride_.size() == 0) {
    return true;
  } else {
    int size = desc_.stride_.size();
    size_t acc = 1;
    for (int i = size - 1; i >= 0; --i) {
      acc *= desc_.shape_[i];
      if (desc_.stride_[i] != acc) {
        return false;
      }
    }
  }
  return true;
}
bool Mat::isExternalBuffer() { return is_external_buffer_; }

MatDesc Mat::getDesc() { return desc_; }
base::DataType Mat::getDataType() { return desc_.data_type_; }
base::IntVector Mat::getShape() { return desc_.shape_; }
int Mat::getShapeIndex(int index) { return desc_.shape_[index]; }
int Mat::getHeight() { return desc_.shape_[0]; }
int Mat::getWidth() { return desc_.shape_[1]; }
int Mat::getChannel() { return desc_.shape_[2]; }
base::SizeVector Mat::getStride() { return desc_.stride_; }
size_t Mat::getStrideIndex(int index) { return desc_.stride_[index]; }

Buffer *Mat::getBuffer() { return buffer_; }
base::DeviceType Mat::getDeviceType() { return buffer_->getDeviceType(); }
Device *Mat::getDevice() { return buffer_->getDevice(); }
BufferPool *Mat::getBufferPool() { return buffer_->getBufferPool(); }
bool Mat::isBufferPool() { return buffer_->isBufferPool(); }
BufferDesc Mat::getBufferDesc() { return buffer_->getDesc(); }
size_t Mat::getSize() { return buffer_->getSize(); }
base::SizeVector Mat::getSizeVector() { return buffer_->getSizeVector(); }
base::IntVector Mat::getConfig() { return buffer_->getConfig(); }
void *Mat::getPtr() { return buffer_->getPtr(); }
int Mat::getId() { return buffer_->getId(); }
BufferSourceType Mat::getBufferSourceType() {
  return buffer_->getBufferSourceType();
}

}  // namespace device
}  // namespace nndeploy
