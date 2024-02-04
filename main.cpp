#include <ctime>
#include <string>

#include "skiplist.h"

int main() {
    srand((uint32_t)time(NULL));
    {
        std::cout << "--- 默认小于号测试 ---" << std::endl;
        SkipList<int, int> list;
        list.insert(1, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        list.insert(5, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        list.display();
    }

    {
        std::cout << "--- lambda测试 ---" << std::endl;
        auto cmp = [](const std::string &a, const std::string &b) { return a.size() < b.size(); };
        SkipList<std::string, int, decltype(cmp)> list(cmp);
        list.insert("aab", 1321);
        list.insert("hello", 54342);
        list.insert("world", 544);
        list.display();
    }

    {
        std::cout << "--- 仿函数测试 ---" << std::endl;
        struct cmp {
            bool operator()(int a, int b) { return a > b; }
        };
        SkipList<int, int, cmp> list;
        for (int i = 1; i <= 10; ++i)
            list.insert(rand() % 20, rand());
        list.display();
    }

    SkipList<std::string, std::string> skipList;
    // insert 测试
    std::cout << "--- insert 测试 ---" << std::endl;
    skipList.insert("1", "one");
    skipList.insert("2", "two");
    skipList.insert("3", "three");
    skipList.insert("abc", "测试");
    skipList.insert("abc", "测试1"); // 更新"abc"
    skipList.insert("中文", "测试2");
    std::cout << "skipList size after insert(): " << skipList.size() << std::endl; // 5
    std::cout << std::endl;

    // [] 测试
    std::cout << "---  [] 测试 ---" << std::endl;
    std::cout << "skipList[\"abc\"] = " << skipList["abc"] << std::endl;
    std::cout << "skipList[\"中文\"] = " << skipList["中文"] << std::endl;
    std::cout << "skipList[\"4\"] = " << skipList["4"] << std::endl;
    skipList["4"] = "four";
    std::cout << "skipList[\"4\"] = " << skipList["4"] << std::endl;
    std::cout << "skipList size after []: " << skipList.size() << std::endl; // 6
    std::cout << std::endl;

    // erase 测试
    std::cout << "--- erase 测试 ---" << std::endl;
    std::cout << "erase \"3\" " << (skipList.erase("3") ? "成功" : "失败") << std::endl; // 成功
    std::cout << "erase \"5\" " << (skipList.erase("5") ? "成功" : "失败") << std::endl; // 失败
    std::cout << "skipList size after erase(): " << skipList.size() << std::endl;        // 5
    std::cout << std::endl;

    // count 测试
    std::cout << "--- count 测试 ---" << std::endl;
    std::cout << "count \"abc\" have " << skipList.count("abc") << std::endl;
    std::cout << "count \"adc\" have " << skipList.count("adc") << std::endl;
    std::cout << std::endl;

    // find 测试
    std::cout << "--- find 测试  ---" << std::endl;
    std::cout << "find \"abc\" have " << ((skipList.find("abc")->key == "abc") ? "成功" : "失败") << std::endl;
    std::cout << "find \"adc\" have " << ((skipList.find("abc")->key == "adc") ? "成功" : "失败") << std::endl;
    std::cout << std::endl;

    // dump_file 测试
    std::cout << "--- dump_file 测试 ---" << std::endl;
    skipList.dump_file();
    std::cout << std::endl;

    // display 测试
    std::cout << "--- display 测试 ---" << std::endl;
    skipList.display();
    std::cout << std::endl;

    // clear 测试
    std::cout << "--- clear 测试 ---" << std::endl;
    skipList.clear();
    std::cout << "skipList size after clear(): " << skipList.size() << std::endl; // 0
    std::cout << std::endl;

    // load_file 测试
    std::cout << "--- load_file 测试 ---" << std::endl;
    skipList.load_file();
    std::cout << "skipList size after load_file(): " << skipList.size() << std::endl; // 5
    skipList.display();

    std::cout << "success !" << std::endl;
    return 0;
}