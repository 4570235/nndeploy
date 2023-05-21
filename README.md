# 机器学习部署框架
+ ai算法工程化 + 推理 的 机器学习部署框架

# 开源计划 
## nndeploy
+ base模块的雏形 - 基本完成，达到可用状态
+ device模块的雏形 - 基本完成，达到可用状态
  + 架构设计 - 基本完成
  + cpu  - 基本完成
  + x86 - 基本完成
  + arm - 基本完成
  + cuda - 70%
  + opencl - always
  + metal - TODO
+ inference - 基本完成，达到可用状态
  + 架构设计 - 基本完成，达到可用状态，还差rapidjson相关
  + mnn - 基本完成
  + tensor_rt - 50%, always
  + openvino - JoDio
  + tf-lite - zwz
  + onnxruntime - zw
  + coreml - TODO
  + ncnn - tyx
  + tnn - gyh
+ thread模块 （需要学习才会有思路）
## nntask （还未开始调研，需要选择一组模型库）
+ 封装opencv
+ 基于graph和inference的扩展
+ 一组算法
  + detr，基于nndeploy完成部署 - always

## review整个代码并且完善

## windows and linux

## 目录
+ build
  + 编译后工程路径
+ cmake
  + 通用的cmake，例如cpack, install
  + config.cmake，编译模板config
  + 第三方库的cmake
    + gitmodules(头文件)
    + 指定目录链接
+ doc
  + 文档，需要调研文档工具以及写注释的方式
+ docker
  + docker脚本
+ ffi
  + 多语言支持
+ nnconvert
  + 统一的模型转换器，完成 X模型文件 -> Y模型文件 功能
  + 配合使用的推理框架版本共同使用
  + 在docker里面统一管理
+ nndeploy
  + 核心 的 c++ 部署框架
+ nnedit
  + 基于（自定义/onnx模型文件）的模型编辑器
+ nnlightweight
  + 模型轻量化工具
+ nnoptimize
  + 基于（自定义/onnx模型文件）模型图优化工具
+ nnquantize
  + 基于推理框架简历的抽象的模型量化工具，在docker里面统一管理
  + 基于（自定义/onnx模型文件）和框架（内部推理框架/训练框架pytorch）模型量化工具
+ nntask
  + 各类落地的ai算法
+ third_party
  + gitmodules
    + 头文件形式的第三方库
  + user
    + 需要自己的编译的头文件


## nndeploy/nndeploy 目录详解 
+ base
  + 类型定义
  + 各种帮助函数
+ cryption 依赖 [base]
  + ***模型加解密***
  + cl脚本 
  + 依赖特殊的加解密第三方库，门面模式 + 适配器模式 + 单例模式
+ device 依赖 [cryption] [base]
  + 异构设备统一的抽象api
    + ***设备是否可以运行检查***
    + ***设备信息查询***
    + 内存分配
    + 内存拷贝
    + 内存零拷贝
    + 同步
    + ***共享上下文***
  + 异构设备的帮助函数
    + 编译 
    + 链接
    + 执行
    + 信息查询
  + 支持异构设备的 Buffer [内部不在使用裸指针，全部用buffer代替，在这里要增加应用计数的实现]
  + 支持异构设备的 Mat [MatDesc + buffer]
  + 支持异构设备的 Tensor [TensorDesc + buffer]
  + 支持异构设备的 传统内存池
    + 可切分的一维连续内存 例如x86以及cuda一维连续内存
    + 不可切分的高维内存 例如OpenCL的cl::Image2d
+ audio 依赖 [device] [cryption] [base] 
  + 暂无想法
+ cv 依赖 [device] [cryption] [base] 
  + ***要有OpenCV的接口易用性，采用device::Mat替代cv::Mat，cv::GpuMat，cv::InputArray，cv::OutputArray***
  + ***接口与实现分离的模式（值得花很多心思去思考，要让开发工作量更小更小）*** 
    + 接口分为两层，对外的统一的api，每个设备下又有api
    + 具体算子的实现采用裸数据
+ ir 依赖 [base] ***TODO***
  + 自定义机器学习中间表示
    + 简洁
    + 尽量参考onnx，然后做到最大范围支持图优化
    + 满足convert、interpert、convert、interpret、quantize、edit、forward的需求
    + Model
      + graph
        + node
          + 满足inferShape
        + inputs
        + outputs
        + initalizer
        + ValueInfo
  + ***自定义模型文件格式抉择***
    + text+bin > json+bin
      + 得自己写反序列化以及序列化
      + 不需要依赖其他库，对移动端友好
      + 感觉可以更好的支持plugin，通过编写一个文件实现算子插件，会大大提高工程化的同事自定义算子的意愿
        + 单独目录编译
        + 通过各种变形，让用户写最少的代码
        + 因为不用考虑自定义用户，内部的模块可以做的更好
    + FlatBuffer
      + 无需自己写反序列化以及序列化
      + 对于用户自定义算子
        + 转换缺失时，需要去写一个FlatBuffer协议
      + 头文件依赖,对移动端相对友好
    + protobuf
      + 无需自己写反序列化以及序列化
      + 对于用户自定义算子
        + 转换缺失时，需要去写一个protobuf协议
      + 有库依赖，对移动端不友好
+ interpret 依赖 [ir] [device] [cryption] [base]
  + 支持模型加解密
  + 解释自定义模型文件
  + ***onnx模型文件*** 
    + python接口更容易被用户使用
    + 多端部署同一套模型，降低工程化同事同一个算法维护多个模型的负担
+ op 依赖 [ir] [device] [cryption] [base]
  + 外部可以直接使用该算子
    + 人像分割模型导出的模型文件没带softmax，op可以直接被调用的化就会方便很多
  + 要有OpenCV的接口易用性，device::Tensor要有类似device::Mat的功能
  + 接口与实现分离的模式（值得花很多心思去思考，要让开发工作量更小更小） 
    + 接口分为两层，对外的统一的api，每个设备下又有api
  + ***具体算子的实现委托给如下机器学习算子框架（这个是最重要的原因，觉得可以去尝试开发一个推理框架，而且性能会比较好）***
    + oneDnn (openvino采用该算子框架，在x86平台下该算子框架最快)
    + xnnpack (tf-lite采用该算子框架，在android移动端该算子框架最快) （人像分割模型：tf-lite[xnnpack] > mnn > tnn > tf-lte）
    + qnnpack 
+ forward 依赖 [op] [interpret] [ir] [device] [cryption] [base]
  + 关联设备
    + ***可以共享调用层上下文***
  + 内存管理 
  + 自动data_format选择
  + ***模型并行：***
    + 自动子图拆分
      + CPU(X86和ARM)-GPU自动异构执行
    + ***模型拆分给多异构设备运行***
    + ***流水线***
  + GPU算子不支持回退到CPU下执行
  + 多种数据交互方式
  + 可以手动搭建forward，借鉴tensor_rt以及openvino
+ optimize 依赖 [forward] [op] [interpret] [ir] [device] [cryption] [base]
  + 支持convert模块的多个优化pass，嵌入在convert中 or 在convert后加一个步骤
  + 支持quantize模块的多个优化pass，嵌入在quantize中 or 在quantize后加一个步骤
  + 支持interpret模块的多个优化pass，嵌入在interpret中 or 在interpret后加一个步骤
  + 支持forward模块的多个优化pass，嵌入在forward中 or 在forward初始化后加一个步骤
+ inference 依赖 [forward] [op] [interpret] [ir] [device] [cryption] [base]
  + ***关联其他推理框架***
    + 关联缺省的推理框架，通过组合的方式封装如下api
      + Forward 的 api
      + Interpret 的 api
    + 关联trt、coreml、openvino、tf-lite
  + ***方便导出python的推理接口，接口要做到像onnxruntime一样易用***
  + ***数据并行：应用层实现多batch下CPU(X86\ARM)-GPU下多Forward推理***
+ aicompile 
  + ai编译器的抽象，暂无想法
+ graph 依赖 [aicompiler] [inference] [cv] [audio] [device] [cryption] [base]
  + 图管理模块，非常核心且重要的模块，具体算法基于该模块建立，
    + 通过编译宏依赖如下平行四个模块 
      + audio
      + cv
      + aicompiler
      + inference
    + 具体子子模块
      + config, 用于传参
      + edge, 数据传输，边
      + edge_manager, 数据管理模块
      + node, 执行节点
      + node_manager, 节点管理模块
      + graph, 执行图


## 想法来源
+ 一套算法实现跑多个推理框架（trt、openvion、coreml、tflite、mnn、tnn、onnxruntime、nndeploy、aitemplate）
+ 通过搭积木的方式完成ai算法的落地，组成有向无环图
  + 单模型算法
    + 前处理node -> 推理node -> 后处理node
  + 多模型的的算法
    + 前处理node -> 推理node -> 后处理node -> 前处理node -> 推理node -> 后处理node -> ... -> ... 前处理node -> 推理node -> 后处理node
  + 图像处理的node
    + 组合cv算子
    + 组合audio算子
    + 组合op算子
  + 推理node
    + 静态shape
    + 动态shape
    + 单输入
    + 单输出
    + 多输入
    + 多输出
    + 单batch
    + 多batch
    + 模型内异构并行
    + 多推理实例
    + 模型并行
    + 流水线
+ 具体算法模块插件化实现
+ 内存管理
  + 传统内存池
  + 基于图的tensor复用的内存池
    + 切割方式
    + 分块方式
    + 独立方式 例如 cl::image2d 的必须等额复用
    + 最小内存
      + 运行时分配 （算法工程化）
      + 基于图算法优化 - （TODO）
    + 最快速度
      + 提前分配
      + 最符合缓存
  + ***满足多个模型组成的pipeline算法的内存复用需求***
    + 基于图的tensor复用的内存池是非嵌入式的
    + 切割方式
    + 模型的output单独分配内存
+ 设备管理
  + 共享应用层上下文，从而实现gpu的数据零拷贝给模型推理
  + 设备信息查询
  + 设备检查, opencl可能会crash
  + 多设备管理 - 支持多线程，多进程
+ 异构执行
  + 单batch下CPU(X86和ARM)-GPU自动异构执行
  + ***单batch下模型并行***
  + ***多batch下多推理，数据并行的分布式推理***
+ 动态尺寸
  + 多种形状 min_shape opt_shape max_shape current_shape，提高工程易用性
  + reshape
  + 检查动态形状是否能完成推理
+ 硬件公司接入推理框架
  + device and op（高性能算子）
  + device and 推理框架 （hvx 和 snpe）
+ 模型量化
  + tnn量化后性能差，人像分割模型量化后性能负优化
    + 量化算子有限，quantize节点多
  + openvino量化后性能极佳，视频去水印模型量化加速2.5倍，量化算子众多
    + 很多子图模块全是量化算子
      + quantize节点少
      + simd: int8以及int16 比 fp16和fp32更快
      + 量化算子扎扎实实少了4倍，对访存极其友好
  + PPQ 深度且受控的量化工具
+ 模型转换 - 建议算法工程师拿到预训练模型后首先做一下模型转换，模型转换时提出一些性能更好的模型修改建议 
  + 设计更规整的模型性，降低模型转换过程中算子缺失的概率 
  + 设计更易图优化的模型结构
+ 模型优化
  + 离线的模型优化器
+ 模型编辑
  + 将细碎的算子 -> 自定义大的算子 -> 推理框架对应算子插件
+ 模型轻量化
  + 针对移动端机器学习算法，一个基础较大的模型，衍生出一批小模型，模型分级落地
+ 模型解释
  + 模型加解密
  + 静态尺寸 static_shape
+ 数据对齐，与python代码对齐
  
+ 基于nndeploy的python部署代码 可以直接生成 so，在各类端侧运行


## 希望可以做到
+ AI算法工程化工程师
  + 开发效率 
  + 性能
    + 时间性能
    + 空间性能
+ 算法工程师
  + 基于python接口框架开发
    + 一键切换推理框架
    + 与训练框架结果对齐
+ 算法落地的公司
  + 一批直接可供使用的算法
  + 算法落地
    + 开发效率
    + 性能
+ 芯片公司
  + 帮助其在生产环境落地
    + 推理框架对接
    + 算子对接
+ 针对大模型的分布式推理(TODO)
+ 统一的机器学习部署框架
  + python -> so(nndeploy and aitemplate)