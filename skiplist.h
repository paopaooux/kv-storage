#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;

template <typename T>
struct Less {
    bool operator()(const T &a, const T &b) const { return a < b; }
};

template <typename K, typename V, typename Comp = Less<K>>
class SkipList {
private:
    struct Node {
        Node **forward;
        int level;
        const K key;
        V value;
        Node() : key{0}, value{0}, level{0}, forward(nullptr) {}
        Node(const K &k, const V &v, const int &l, Node *nxt = nullptr) : key(k), value(v), level(l) {
            this->forward = new Node *[level + 1];
            for (int i = 0; i <= level; ++i) {
                this->forward[i] = nxt;
            }
        }
        ~Node() { delete[] forward; }
    };

public:
    SkipList();
    SkipList(Comp);
    int get_random_level();
    void insert(const K &, const V &);
    bool erase(const K &);
    bool count(const K &);
    void clear();
    int size();
    void display();
    void dump_file();
    void load_file();

    ~SkipList();

    struct Iter {
        Node *p;
        Iter() : p(nullptr){};
        Iter(Node *rhs) : p(rhs) {}
        Node *operator->() const { return (p); }
        Node &operator*() const { return *p; }
        bool operator==(const Iter &rhs) { return rhs.p == p; }
        bool operator!=(const Iter &rhs) { return !(rhs.p == p); }
        void operator++() { p = p->forward[0]; }
        void operator++(int) { p = p->forward[0]; }
    };
    Iter begin();
    Iter end();
    Iter find(const K &);
    V &operator[](const K &);

private:
    void init();
    Node *find(const K &, Node **);
    static constexpr int max_level = 32;
    int skip_list_level;
    int element_count;
    Node *head, *tail;
    Comp less;
    std::ifstream file_reader;
    std::ofstream file_writer;
};

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::init() {
    mtx.lock();
    skip_list_level = 0;
    element_count = 0;
    head->forward = new Node *[max_level + 1];
    for (int i = 0; i <= max_level; ++i) {
        head->forward[i] = tail;
    }
    mtx.unlock();
}

template <typename K, typename V, typename Comp>
SkipList<K, V, Comp>::SkipList() : head(new Node()), tail(new Node()), less{Comp()} {
    init();
}

template <typename K, typename V, typename Comp>
SkipList<K, V, Comp>::SkipList(Comp _less) : head(new Node()), tail(new Node()), less{_less} {
    init();
}

template <typename K, typename V, typename Comp>
int SkipList<K, V, Comp>::get_random_level() {
    int lv = 1;
    while (rand() & 1) {
        ++lv;
    }
    return lv < max_level ? lv : max_level;
}

template <typename K, typename V, typename Comp>
typename SkipList<K, V, Comp>::Node *SkipList<K, V, Comp>::find(const K &k, Node **update) {
    Node *p = head;
    for (int i = max_level; i >= 0; --i) {
        while (p->forward[i] != tail && less(p->forward[i]->key, k)) {
            p = p->forward[i];
        }
        update[i] = p;
    }
    p = p->forward[0];
    return p;
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::insert(const K &k, const V &v) {
    Node *update[max_level + 1];
    mtx.lock();
    Node *p = find(k, update);
    if (p->key == k) {
        p->value = v;
        mtx.unlock();
        return;
    }
    int lv = get_random_level();
    if (lv > skip_list_level) {
        lv = ++skip_list_level;
        update[lv] = head;
    }
    Node *newNode = new Node(k, v, lv);
    for (int i = lv; i >= 0; --i) {
        p = update[i];
        newNode->forward[i] = p->forward[i];
        p->forward[i] = newNode;
    }
    ++element_count;
    mtx.unlock();
}

template <typename K, typename V, typename Comp>
bool SkipList<K, V, Comp>::erase(const K &k) {
    Node *update[max_level + 1];
    mtx.lock();
    Node *p = find(k, update);
    if (p->key != k) {
        mtx.unlock();
        return false;
    }
    mtx.unlock();
    return true;
    for (int i = 0; i <= p->level; ++i) {
        update[i]->forward[i] = p->forward[i];
    }
    delete p;
    while (skip_list_level > 0 && head->forward[skip_list_level] == tail) {
        --skip_list_level;
    }
    --element_count;
    mtx.unlock();
    return true;
}

template <typename K, typename V, typename Comp>
bool SkipList<K, V, Comp>::count(const K &k) {
    Node *update[max_level + 1];
    Node *p = find(k, update);
    if (p == tail) {
        return false;
    }
    return p->key == k;
}

template <typename K, typename V, typename Comp>
V &SkipList<K, V, Comp>::operator[](const K &k) {
    Node *update[max_level + 1];
    Iter p = find(k, update);
    // std::cout << p->key << std::endl;
    if (p == end()) {
        insert(k, V{});
        p = find(k, update);
    }
    return p->value;
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::clear() {
    std::cout << "clear ..." << std::endl;
    mtx.lock();
    Node *node = head->forward[0];
    while (node != tail) {
        Node *temp = node;
        node = node->forward[0];
        delete temp;
    }
    mtx.unlock();
    init();
}

template <typename K, typename V, typename Comp>
int SkipList<K, V, Comp>::size() {
    return this->element_count;
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::display() {
    for (auto it = begin(); it != end(); it++) {
        std::cout << it->key << " " << it->value << "\n";
    }
    std::cout << std::endl;
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::dump_file() {
    std::cout << "dump_file..." << std::endl;
    file_writer.open(STORE_FILE, std::ofstream::out | std::ofstream::trunc);

    for (auto node = begin(); node != end(); node++) {
        file_writer << node->key << " " << node->value << "\n";
        std::cout << node->key << " " << node->value << "\n";
    }
    file_writer.flush();
    file_writer.close();
    return;
}

template <typename K, typename V, typename Comp>
void SkipList<K, V, Comp>::load_file() {
    file_reader.open(STORE_FILE, std::istream::in);
    if (!file_reader.is_open()) {
        std::cout << "文件打开失败!" << std::endl;
        return;
    }
    K k;
    V v;
    while (file_reader >> k >> v) {
        insert(k, v);
    }
    file_reader.close();
}

template <typename K, typename V, typename Comp>
SkipList<K, V, Comp>::~SkipList() {
    if (file_writer.is_open()) {
        file_writer.close();
    }
    if (file_reader.is_open()) {
        file_reader.close();
    }
    delete head;
    delete tail;
}

template <typename K, typename V, typename Comp>
typename SkipList<K, V, Comp>::Iter SkipList<K, V, Comp>::find(const K &k) {
    Node *update[max_level + 1];
    Node *p = find(k, update);
    if (p == tail) {
        return tail;
    }
    if (p->key != k) {
        return tail;
    }
    return Iter(p);
}

template <typename K, typename V, typename Comp>
typename SkipList<K, V, Comp>::Iter SkipList<K, V, Comp>::begin() {
    return Iter(head->forward[0]);
}

template <typename K, typename V, typename Comp>
typename SkipList<K, V, Comp>::Iter SkipList<K, V, Comp>::end() {
    return Iter(tail);
}
