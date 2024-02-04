#include "../skiplist.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <string>

#define STORE_FILE_STRESS_TSET "stress-test/test_result"

int64_t NUM_THREADS, TEST_COUNT;

SkipList<int, std::string> skipList;

void *insertElement(void *threadid) {
    int64_t tid = (int64_t)threadid;
    // std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i, ++count) {
        skipList.insert(rand() % TEST_COUNT, "a");
    }
    pthread_exit(NULL);
}

void *findElement(void *threadid) {
    int64_t tid = (int64_t)threadid;
    // std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i, ++count) {
        skipList.find(rand() % TEST_COUNT);
    }
    pthread_exit(NULL);
}

void *eraseElement(void *threadid) {
    int64_t tid = (int64_t)threadid;
    // std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i, ++count) {
        skipList.erase(rand() % TEST_COUNT);
    }
    pthread_exit(NULL);
}

void *fixElement(void *threadid) {
    int64_t tid = (int64_t)threadid;
    // std::cout << tid << std::endl;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i, ++count) {
        skipList.insert(rand() % TEST_COUNT, "b");
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    srand((uint64_t)time(NULL));

    TEST_COUNT = std::stoll(std::string(argv[1])), NUM_THREADS = std::stoll(std::string(argv[2]));
    std::ofstream file_writer;
    file_writer.open(STORE_FILE_STRESS_TSET, std::ofstream::out | std::ofstream::app);
    file_writer << "TEST_COUNT : " << TEST_COUNT << " NUM_THREADS: " << NUM_THREADS << std::endl;
    file_writer.close();

    // ~ insert 测试
    {
        pthread_t threads[NUM_THREADS];

        auto start = std::chrono::high_resolution_clock::now();

        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            // std::cout << "main() : creating thread, " << i << std::endl;
            int ret = pthread_create(&threads[i], NULL, insertElement, (void *)i);

            if (ret) {
                std::cout << "Error:unable to create thread," << ret << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_create() error");
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file_writer.open(STORE_FILE_STRESS_TSET, std::ofstream::out | std::ofstream::app);
        file_writer << "insert elapsed: " << elapsed.count() << std::endl;
        file_writer.close();
    }
    std::cout << "skipList size after insert: " << skipList.size() << std::endl;
    // skipList.display();
    // ~ update 测试
    {
        pthread_t threads[NUM_THREADS];

        auto start = std::chrono::high_resolution_clock::now();

        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            // std::cout << "main() : creating thread, " << i << std::endl;
            int ret = pthread_create(&threads[i], NULL, fixElement, (void *)i);

            if (ret) {
                std::cout << "Error:unable to create thread," << ret << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_create() error");
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file_writer.open(STORE_FILE_STRESS_TSET, std::ofstream::out | std::ofstream::app);
        file_writer << "update elapsed: " << elapsed.count() << std::endl;
        file_writer.close();
    }
    std::cout << "skipList size after update: " << skipList.size() << std::endl;
    // ~ find 测试
    {
        pthread_t threads[NUM_THREADS];

        auto start = std::chrono::high_resolution_clock::now();

        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            // std::cout << "main() : creating thread, " << i << std::endl;
            int ret = pthread_create(&threads[i], NULL, findElement, (void *)i);

            if (ret) {
                std::cout << "Error:unable to create thread," << ret << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_create() error");
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file_writer.open(STORE_FILE_STRESS_TSET, std::ofstream::out | std::ofstream::app);
        file_writer << "find elapsed: " << elapsed.count() << std::endl;
        file_writer.close();
    }
    std::cout << "skipList size after find: " << skipList.size() << std::endl;
    // ~ erase 测试
    {
        pthread_t threads[NUM_THREADS];

        auto start = std::chrono::high_resolution_clock::now();

        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            // std::cout << "main() : creating thread, " << i << std::endl;
            int ret = pthread_create(&threads[i], NULL, eraseElement, (void *)i);

            if (ret) {
                std::cout << "Error:unable to create thread," << ret << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (int64_t i = 0; i < NUM_THREADS; ++i) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_create() error");
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        file_writer.open(STORE_FILE_STRESS_TSET, std::ofstream::out | std::ofstream::app);
        file_writer << "erase elapsed: " << elapsed.count() << std::endl;
        file_writer << std::endl;
        file_writer.close();
    }
    std::cout << "skipList size after erase: " << skipList.size() << std::endl;
    std::cout << "success !" << std::endl;
    std::cout << std::endl;
    return 0;
}