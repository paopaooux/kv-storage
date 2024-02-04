## 基于跳表的实现的轻量级KV存储引擎

> 在非关系型数据库redis，以及levedb，rockdb其核心存储引擎的数据结构就是跳表。

采用**跳表**作为底层数据结构，使用C++11实现。支持数据的插入、删除、查询、修改、可持久化以及数据库大小显示。

使用C++模板编程，使用类似于STL中的map容器。支持自定义类型与自定义比较函数（可以传入lambda与仿函数），迭代器遍历。

在随机写读情况下，以 **100 w** 数据结果计算每秒可处理读请求数（QPS）插入:**65.26 w**，修改:**48.45 w**，查询:**54.87 w**，删除:**55.85 w**。

## 项目中文件

+ `main.cpp` 包含`skiplist.h`使用跳表进行数据操作以及简单测试
+ `skiplist.h` 跳表核心实现
+ `README.md` 项目介绍         
+ `bin` 生成可执行文件目录 
+ `CmakeLists.txt` 编译脚本
+ `store` 数据落盘的文件存放在这个文件夹 
+ `stress-test` 压力测试代码`stress_test.cpp`以及测试数据存放在这个文件夹 
+ `stress_test_start.sh` 压力测试脚本
+ `fromat_count.sh` 代码格式化以及代码行数统计脚本
+ `LICENSE` 使用协议

## 提供接口 

+ insert (插入数据以及更新数据)
+ erase (删除数据)
+ find (查询数据，返回迭代器)
+ count (查询数据，返回数据个数)
+ display (展示已存数据)
+ dump_File (数据落盘)
+ load_File (加载数据)
+ size (返回数据规模)
+ clear (清空数据)
+ [] (查询以及插入数据)

## 存储引擎数据表现

跳表树高：**32** 

### 增删改查QPS测试

测试方法：设定固定的操作数，采用随机数据，统计各个操作运行时间。

#### 单线程测试

| 具体操作\数据规模 | 10 w        | 20w        | 50 w       | 100 w     |
| ----------------- | ----------- | ---------- | ---------- | --------- |
| 插入              | 0.0518790 s | 0.138943 s | 0.535340 s | 1.53227 s |
| 修改              | 0.0636764 s | 0.204427 s | 0.775214 s | 2.06390 s |
| 查询              | 0.0572079 s | 0.191262 s | 0.706922 s | 1.82243 s |
| 删除              | 0.0589757 s | 0.193862 s | 0.718549 s | 1.79036 s |

以 **100 w** 数据结果计算 QPS:

🔹 **插入** ：**65.26 w** 🔸 **修改** ：**48. 45 w** 🔹 **查询** ：**54.87 w** 🔸 **删除** ：**55.85 w**

#### 多线程测试

数据规模为 :**200w**
| 具体操作\线程数 | 1         | 2         | 5         | 10        |
| --------------- | --------- | --------- | --------- | --------- |
| 插入            | 3.39630 s | 7.30715 s | 6.92384 s | 4.38100 s |
| 修改            | 4.14382 s | 8.17073 s | 8.25041 s | 5.28443 s |
| 查询            | 4.00080 s | 2.06557 s | 1.07572 s | 1.03808 s |
| 删除            | 4.00035 s | 7.94836 s | 7.79253 s | 5.60646 s |


## 项目运行方式

```shell
mkdir build  && cd build && cmake .. && make         // complie demo main.cpp
./bin/main      // run 
```



+ 可以运行如下脚本测试KV存储引擎的性能

```shell
bash stress_test_start.sh 
```
