#设置cmake最小的版本要求
cmake_minimum_required(VERSION 3.10)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)

#设置项目名
project(kv_storage)

#添加可执行文件(第一个参数是生成的可执行文件的名字，第二个参数是源文件)
add_executable(main main.cpp)

# 将可执行文件输出到 bin 目录下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)