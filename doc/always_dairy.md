# 要做的事项
+ 建立起基本的共识
+ cmake结构
+ 第三库的管理
+ clang-tidy、clang-format 统一编码风格
+ doc的调研 注释的风格
+ base模块 c++设计与实现 分工合作
+ device 
  + cpu模块、X86M模块、arm模块、opencl模块、cuda模块
  + buffer
  + mat
+ inference
  + tf-lite
  + tensor_rt
  + openvino
  + coreml
+ graph
  + 思路，设计
+ vision
  + cv算子


# 23
+ DataType 的 type_trait
+ basic以及Desc的构造函数
  + 强转问题
+ buffer的多线程资源管理问题
+ tensor
  + 要不要增加不存在buffer，但是其他都有的接口
+ 移除include_c_cc.h


# 26
+ 多线程
+ c++语法
+ 标准库
+ "[\s\S]*" 

+ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/always/github/nndeploy/build/install   
+ export LD_LIBRARY_PATH=/home/for_all_users/third_party/user/linux/TensorRT-7.2.3.4.Ubuntu-18.04.x86_64-gnu.cuda-11.1.cudnn8.1/TensorRT-7.2.3.4/lib:$LD_LIBRARY_PATH
+ export PATH=/snap/bin/:$PATH
+ export LD_LIBRARY_PATH=/home/always/github/nndeploy/build/install/:$LD_LIBRARY_PATH
+ export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

# 2023.05.27
+ device模块的多线程问题
+ inference模块review

git config --global user.name "02200059Z"
git config --global user.email 2018222066@email.szu.edu.cn

# 2023.07.30
+ 完成六个推理框架的接入
  + tensor_rt
  + openvino
  + tf-lite
  + onnxruntime
  + mnn
  + tnn
+ 接入一组检测算法
+ 开源

sudo apt install lam-runtime       # version 7.1.4-7, or
sudo apt install mpich             # version 4.0-3
sudo apt install openmpi-bin       # version 4.1.2-2ubuntu1
sudo apt install slurm-wlm-torque  # version 21.08.5-2ubuntu1

OMP_NUM_THREADS=1 mpiexec -n 4 valgrind -s --tool=memcheck --leak-check=full --error-exitcode=1 --errors-for-leak-kinds=definite --show-leak-kinds=definite --undef-value-errors=no exe

OMP_NUM_THREADS=1 mpiexec -n 4 valgrind -s --tool=memcheck --leak-check=full --error-exitcode=1 --errors-for-leak-kinds=definite --show-leak-kinds=definite --undef-value-errors=no /home/always/github/nndeploy/build/install/demo_detect

# 2023.08.05
+ 推理的模板完成
+ openvino参考fastdeploy，完成接入

# 
base->time_profiler
device->arm
inference->tnn
model->detr、yolo

# openvino的编译
+ https://blog.csdn.net/cosmos_qiu/article/details/113502773
+ git submodule sync
+ cmake --install . --prefix /home/always/github/openvino/install