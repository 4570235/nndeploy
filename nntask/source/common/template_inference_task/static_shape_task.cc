#include "nntask/source/common/template_inference_task/static_shape_task.h"

namespace nntask {
namespace common {

StaticShapeTask::StaticShapeTask(bool allcoate_tensor_flag,
                                 nndeploy::base::InferenceType type,
                                 nndeploy::base::DeviceType device_type,
                                 const std::string &name)
    : Task(type, device_type, name) {
  allcoate_tensor_flag_ = allcoate_tensor_flag;
}

StaticShapeTask::~StaticShapeTask() {}

nndeploy::base::Status StaticShapeTask::init() {
  nndeploy::base::Status status = nndeploy::base::kStatusCodeOk;
  status = Task::init();
  allocateInputOutputTensor();
  return status;
}

nndeploy::base::Status StaticShapeTask::deinit() {
  nndeploy::base::Status status = nndeploy::base::kStatusCodeOk;
  status = Task::deinit();
  deallocateInputOutputTensor();
  return status;
}

nndeploy::base::Status StaticShapeTask::setInput(Packet &input) {
  Task::setInput(input);
  for (auto tensor : input_tensors_) {
    inference_->setInputTensor(tensor->getName(), tensor);
  }
  post_process_->setInput(*inference_output_packet_);
  return nndeploy::base::kStatusCodeOk;
}

nndeploy::base::Status StaticShapeTask::setOutput(Packet &output) {
  Task::setOutput(output);
  pre_process_->setOutput(*inference_input_packet_);
  for (auto tensor : output_tensors_) {
    inference_->setOutputTensor(tensor->getName(), tensor);
  }
  return nndeploy::base::kStatusCodeOk;
}

nndeploy::base::Status StaticShapeTask::run() {
  nndeploy::base::Status status = nndeploy::base::kStatusCodeOk;
  status = Task::run();
  return status;
}

nndeploy::base::Status StaticShapeTask::allocateInputOutputTensor() {
  nndeploy::inference::InferenceParam *inference_param =
      dynamic_cast<nndeploy::inference::InferenceParam *>(
          inference_->getParam());
  nndeploy::device::Device *device =
      nndeploy::device::getDevice(inference_param->device_type_);
  bool is_share_command_queue = inference_->isShareCommanQueue();

  if (is_share_command_queue && allcoate_tensor_flag_ == false) {
    input_tensors_ = inference_->getAllInputTensorVector();
    output_tensors_ = inference_->getAllOutputTensorVector();
  } else {
    nndeploy::device::Device *device =
        nndeploy::device::getDevice(device_type_);

    std::vector<std::string> input_names = inference_->getAllInputTensorName();
    for (auto name : input_names) {
      nndeploy::device::TensorDesc desc =
          inference_->getInputTensorAlignDesc(name);
      nndeploy::device::Tensor *tensor = new nndeploy::device::Tensor(
          device, desc, name, nndeploy::base::IntVector());
      input_tensors_.emplace_back(tensor);
    }

    std::vector<std::string> output_names =
        inference_->getAllOutputTensorName();
    for (auto name : output_names) {
      nndeploy::device::TensorDesc desc =
          inference_->getOutputTensorAlignDesc(name);
      nndeploy::device::Tensor *tensor = new nndeploy::device::Tensor(
          device, desc, name, nndeploy::base::IntVector());
      output_tensors_.emplace_back(tensor);
    }
  }

  inference_input_packet_ = new Packet(input_tensors_);
  inference_output_packet_ = new Packet(output_tensors_);
  return nndeploy::base::kStatusCodeOk;
}

nndeploy::base::Status StaticShapeTask::deallocateInputOutputTensor() {
  nndeploy::base::Status status = nndeploy::base::kStatusCodeErrorUnknown;
  bool is_share_command_queue = inference_->isShareCommanQueue();
  if (is_share_command_queue && allcoate_tensor_flag_ == false) {
    status = nndeploy::base::kStatusCodeOk;
  } else {
    for (auto iter : input_tensors_) {
      delete iter;
    }
    input_tensors_.clear();
    for (auto iter : output_tensors_) {
      delete iter;
    }
    output_tensors_.clear();
    status = nndeploy::base::kStatusCodeOk;
  }

  delete inference_input_packet_;
  delete inference_output_packet_;

  return status;
}

}  // namespace common
}  // namespace nntask
