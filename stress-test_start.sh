#!/bin/bash
g++ stress-test/stress_test.cpp -o ./bin/stress  --std=c++11 -pthread  

./bin/stress 100000 1
./bin/stress 200000 1
./bin/stress 500000 1
./bin/stress 1000000 1

#多线程测试
./bin/stress 2000000 1
./bin/stress 2000000 2
./bin/stress 2000000 5
./bin/stress 2000000 10