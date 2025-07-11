#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <numeric>
#include <random>
#include <string>



using namespace std;
using namespace std::chrono;

class BTreeNode {
public:
    vector<int> keys;
    vector<BTreeNode*> children;
    bool is_leaf;
    int order;

    BTreeNode(int _order, bool _is_leaf) : order(_order), is_leaf(_is_leaf) {
        keys.reserve(order - 1);
        children.reserve(order);
    }

    int findKey(int k) {
        auto it = lower_bound(keys.begin(), keys.end(), k);
        return it - keys.begin();
    }

    void insertNonFull(int k) {
        int i = keys.size() - 1;

        if (is_leaf) {
            keys.push_back(0);
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
        }
        else {
            while (i >= 0 && keys[i] > k)
                i--;

            if (children[i + 1]->keys.size() == order - 1) {
                splitChild(i + 1, children[i + 1]);
                if (keys[i + 1] < k)
                    i++;
            }
            children[i + 1]->insertNonFull(k);
        }
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(y->order, y->is_leaf);
        int split_pos = (order - 1) / 2;

        z->keys.insert(z->keys.end(), y->keys.begin() + split_pos + 1, y->keys.end());

        if (!y->is_leaf) {
            z->children.insert(z->children.end(), y->children.begin() + split_pos + 1, y->children.end());
            y->children.erase(y->children.begin() + split_pos + 1, y->children.end());
        }

        int median = y->keys[split_pos];
        y->keys.resize(split_pos);

        children.insert(children.begin() + i + 1, z);
        keys.insert(keys.begin() + i, median);
    }

    bool search(int k) {
        int i = findKey(k);

        if (i < keys.size() && keys[i] == k)
            return true;

        if (is_leaf)
            return false;

        return children[i]->search(k);
    }

    ~BTreeNode() {
        for (auto child : children)
            delete child;
    }
};

class BTree {
private:
    BTreeNode* root;
    int order;

public:
    BTree(int _order) : order(_order), root(nullptr) {
        if (order < 3) {
            throw invalid_argument("Order must be at least 3");
        }
    }

    bool search(int k) {
        return (root == nullptr) ? false : root->search(k);
    }

    void insert(int k) {
        if (root == nullptr) {
            root = new BTreeNode(order, true);
            root->keys.push_back(k);
        }
        else {
            if (root->keys.size() == order - 1) {
                BTreeNode* s = new BTreeNode(order, false);
                s->children.push_back(root);
                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0] < k)
                    i++;
                s->children[i]->insertNonFull(k);

                root = s;
            }
            else {
                root->insertNonFull(k);
            }
        }
    }

    ~BTree() {
        if (root)
            delete root;
    }
};




vector<int> generate_sequential(int n) {
    vector<int> data(n);
    iota(data.begin(), data.end(), 0);
    return data;
}

vector<int> generate_random(int n) {
    vector<int> data(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, n * 10);
    generate(data.begin(), data.end(), [&]() { return dist(gen); });
    return data;
}

void run_safe_recursive_test(const string& filename) {
    ofstream out_file(filename);
    if (!out_file) {
        cerr << "Failed to open output file" << endl;
        return;
    }

    out_file << "test_type,order,array_size,operation,time_ns\n";

    // Безопасные параметры для рекурсивной версии
    const vector<int> orders = { 3, 5, 7 };  // Порядки дерева
    const vector<int> sizes = { 500, 1000, 2000, 5000 };  // Размеры данных
    const int search_samples = 200;  // Число проверок поиска

    // Тестируем только sequential и random - reversed может вызывать переполнение
    for (int order : orders) {
        for (const string& test_type : { "sequential", "random" }) {
            for (int size : sizes) {
                // Ограничение для order=3 (самый глубокий)
                if (order == 3 && size > 3000) continue;

                cout << "Testing: order=" << order
                    << ", type=" << test_type
                    << ", size=" << size << endl;

                // Генерация данных
                vector<int> data = (test_type == "sequential")
                    ? generate_sequential(size)
                    : generate_random(size);

                BTree tree(order);

                // Тест вставки с прогресс-баром
                auto start = high_resolution_clock::now();
                for (int i = 0; i < size; i++) {
                    tree.insert(data[i]);
                    if (i % 500 == 0) cout << "." << flush;
                }
                auto end = high_resolution_clock::now();
                out_file << test_type << "," << order << "," << size << ",insert,"
                    << duration_cast<nanoseconds>(end - start).count() / size << "\n";

                // Тест поиска существующих элементов
                start = high_resolution_clock::now();
                for (int i = 0; i < search_samples; i++) {
                    tree.search(data[i % size]);
                }
                end = high_resolution_clock::now();
                out_file << test_type << "," << order << "," << size << ",search_exist,"
                    << duration_cast<nanoseconds>(end - start).count() / search_samples << "\n";

                // Тест поиска отсутствующих элементов
                start = high_resolution_clock::now();
                for (int i = 0; i < search_samples; i++) {
                    tree.search(-(i + 1));
                }
                end = high_resolution_clock::now();
                out_file << test_type << "," << order << "," << size << ",search_miss,"
                    << duration_cast<nanoseconds>(end - start).count() / search_samples << "\n";

                cout << endl;
            }
        }
    }
    out_file.close();
    cout << "Testing completed. Results saved to " << filename << endl;
}

int main() {
    try {
        run_safe_recursive_test("C:\\Users\\ИОН\\Downloads\\ЛШПИ_2025\\LSHPI_Tree_project\\res2.txt");
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
