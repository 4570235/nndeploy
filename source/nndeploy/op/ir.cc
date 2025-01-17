
#include "nndeploy/op/ir.h"

namespace nndeploy {
namespace op {

OpDesc::OpDesc() {}

OpDesc::OpDesc(const std::string &name, OpType op_type)
    : name_(name), op_type_(op_type) {
  op_param_ = createOpParam(op_type);
}
OpDesc::OpDesc(const std::string &name, OpType op_type,
               std::shared_ptr<base::Param> op_param)
    : name_(name), op_type_(op_type) {
  if (op_param != nullptr) {
    op_param_ = op_param->copy();
  } else {
    op_param_ = createOpParam(op_type);
  }
}

OpDesc::OpDesc(const std::string &name, OpType op_type,
               std::initializer_list<std::string> inputs,
               std::initializer_list<std::string> outputs)
    : name_(name), op_type_(op_type), inputs_(inputs), outputs_(outputs) {
  op_param_ = createOpParam(op_type);
}
OpDesc::OpDesc(const std::string &name, OpType op_type,
               std::initializer_list<std::string> inputs,
               std::initializer_list<std::string> outputs,
               std::shared_ptr<base::Param> op_param)
    : name_(name), op_type_(op_type), inputs_(inputs), outputs_(outputs) {
  if (op_param != nullptr) {
    op_param_ = op_param->copy();
  } else {
    op_param_ = createOpParam(op_type);
  }
}

OpDesc::OpDesc(const std::string &name, OpType op_type,
               std::vector<std::string> &inputs,
               std::vector<std::string> &outputs)
    : name_(name), op_type_(op_type), inputs_(inputs), outputs_(outputs) {
  op_param_ = createOpParam(op_type);
}
OpDesc::OpDesc(const std::string &name, OpType op_type,
               std::vector<std::string> &inputs,
               std::vector<std::string> &outputs,
               std::shared_ptr<base::Param> op_param)
    : name_(name), op_type_(op_type), inputs_(inputs), outputs_(outputs) {
  if (op_param != nullptr) {
    op_param_ = op_param->copy();
  } else {
    op_param_ = createOpParam(op_type);
  }
}

OpDesc::~OpDesc() {}

ValueDesc::ValueDesc() {}

ValueDesc::ValueDesc(const std::string &name) : name_(name) {}
ValueDesc::ValueDesc(const std::string &name, base::DataType data_type)
    : name_(name), data_type_(data_type) {}
ValueDesc::ValueDesc(const std::string &name, base::DataType data_type,
                     base::IntVector shape)
    : name_(name), data_type_(data_type), shape_(shape) {}

ModelDesc::ModelDesc() {}
ModelDesc::~ModelDesc() {
  for (auto iter : weights_) {
    if (iter.second != nullptr) {
      delete iter.second;
    }
  }
}

std::map<OpType, std::shared_ptr<OpParamCreator>>
    &getGlobalOpParamCreatorMap() {
  static std::once_flag once;
  static std::shared_ptr<std::map<OpType, std::shared_ptr<OpParamCreator>>>
      creators;
  std::call_once(once, []() {
    creators.reset(new std::map<OpType, std::shared_ptr<OpParamCreator>>);
  });
  return *creators;
}

std::shared_ptr<base::Param> createOpParam(OpType type) {
  std::shared_ptr<base::Param> temp;
  auto &creater_map = getGlobalOpParamCreatorMap();
  if (creater_map.count(type) > 0) {
    temp = creater_map[type]->createOpParam(type);
  }
  return temp;
}

REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeConcat, ConcatParam);

REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeConv, ConvParam);

// MaxPool 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeMaxPool, MaxPoolParam);

// Reshape 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeReshape, ReshapeParam);

// Resize 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeResize, ResizeParam);

// Slice 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeSlice, SliceParam);

// Softmax 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeSoftmax, SoftmaxParam);

// Split 算子参数类的注册函数
REGISTER_OP_PARAM_IMPLEMENTION(kOpTypeSplit, SplitParam);

// TODO: @Leonisux:
// 补充llama的算子的参数的注册

}  // namespace op
}  // namespace nndeploy