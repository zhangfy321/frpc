cmake_minimum_required(VERSION 3.15)
project(
    frpc 
    VERSION 0.1
    DESCRIPTION "FRPC Framework"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11) 

## 启用单元测试
enable_testing()

# add_executable(frpc main.cpp)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "DEBUG") # Debug / Release
endif()

set(CXX_FLAGS
 -g # 生成调试信息
 # -DVALGRIND # 内存泄漏检查工具
#  -DCHECK_PTHREAD_RETURN_VALUE # todo 作用待研究
 # -D_FILE_OFFSET_BITS=64 # 定义一个环境变量_FILE_OFFSET_BITS且值为64 
 -Wall  # 显示所有警告信息
 -Wextra # 打印一些额外的警告信息
 -Werror # 将所有的警告当成错误进行处理
 -Wconversion # 当从较宽类型到较窄类型的隐式转换时,会产生警告
 -Wno-unused-parameter
 -Wno-unused-variable
 -Wno-unused-result
 -Wno-unused-but-set-variable
 -Wold-style-cast # 不允许使用C方式的强转
 -Woverloaded-virtual # 不允许虚函数重载
 -Wpointer-arith # 不允许void* 指针算术运算
 -Wshadow
 -Wwrite-strings # 不允许将字符串常量转为char*
 -march=native # todo 一些人说加上这个会导致性能损失 待测试
 # -O0 # 不进行优化处理
 # -O1 # 进行优化
 # -O2 # 进一步优化
 # -O3 # 更进一步优化 包括inline函数
#  -march选项就是就是指定目标架构的名字，gcc就会生成针对目标架构优化的目标代码，如-march=prescott会生成针对i5或i7的目标码，
#  从而充分发挥cpu的性能。自gcc4.2，引入了-march=native，从而允许编译器自动探测目标架构并生成针对目标架构优化的目标代码，
#  这比手工设置要安全的多
# -MMD # todo 作用待研究 自动生成规则，显示.c文件依赖的除标准库外的头文件，输出到.d文件
 -std=c++11
#  -rdynamic # todo 作用待研究 
 -pthread   # 编译多线程程序 需要链接pthread库
 )

if(CMAKE_BUILD_BITS EQUAL 32) # 32位机器上加32架构flag
  list(APPEND CXX_FLAGS "-m32")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang") # Clang编译器关闭一些flag
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}") #todo

set(CMAKE_CXX_FLAGS_DEBUG "-O0")  # DEBUG版本关闭优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") # 发行版本开启优化

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) # 指定二进制输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) # 指定库输出目录


find_package(Boost REQUIRED) # 需要官方已经定义好FindXXX.cmake
# find_package(Protobuf)

# find_library(JEMALLOC) # todo 引入jemalloc

# 检查是否找到对应库或包
if(PROTOBUF_FOUND)
  message(STATUS "found protobuf")
endif()

# 打印编译选项
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})