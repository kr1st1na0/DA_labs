#include <iostream>
#include <fstream>

const size_t KEY_MAX_SIZE = 257;

class TString {
private:
    char *data;
    size_t size;
    size_t capacity;

public:
    TString();
    TString(const char *str);
    TString(const TString &other);
    ~TString();

    TString &operator=(const TString &other);
    bool operator==(const char *str) const;

    friend bool operator>(const TString &lhs, const TString &rhs);
    friend bool operator<(const TString &lhs, const TString &rhs);
    char& operator[](int index);
    const char& operator[](int index) const;

    friend std::ostream &operator<<(std::ostream &os, const TString &str) {
        os << str.data;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, TString &str) {
        str.Clear(); 
        char c;
        while (is.get(c) && (c == ' ' || c == '\n')) {}
        while (c != ' ' && c != '\n') {
            str.PushBack(c);
            is.get(c);
        }
        return is;
    }

    size_t Size() const;
    const char* GetData() const;
    void Move(char* str);
    void Clear();
    void PushBack(char c);
    void Resize(size_t new_capacity);
};

TString::TString() : size(0), data(nullptr), capacity(0) {}

TString::TString(const char *str) {
    size = 0;
    capacity = 0;
    while (str[size] != '\0' && size < KEY_MAX_SIZE - 1) {
        ++size;
    }
    capacity = size;
    data = new char[size + 1];
    for (size_t i = 0; i < size; ++i) {
        data[i] = str[i];
    }
    data[size] = '\0';
}

TString::TString(const TString &other) : data(nullptr), size(other.size), capacity(other.capacity) {
    data = new char[size + 1];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
    data[size] = '\0';
}

TString::~TString() {
    delete[] data;
}

TString &TString::operator=(const TString &other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new char[size + 1];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        data[size] = '\0';
    }
    return *this;
}

bool TString::operator==(const char *str) const {
    size_t i = 0;
    while (data[i] != '\0' && str[i] != '\0') {
        if(data[i] != str[i]) {
            return false;
        }
        ++i;
    }
    return (data[i] == '\0' && str[i] == '\0');
}

bool operator>(const TString &lhs, const TString &rhs) {
    TString l = lhs;
    TString r = rhs;
    size_t i = 0;
    while (l.data[i] != '\0' && r.data[i] != '\0') {
        if (l.data[i] > r.data[i])
            return true;
        else if (l.data[i] < r.data[i])
            return false;
        ++i;
    }
    return (l.data[i] != '\0' && r.data[i] == '\0');
}

bool operator<(const TString &lhs, const TString &rhs) {
    TString l = lhs;
    TString r = rhs;
    size_t i = 0;
    while (l.data[i] != '\0' && r.data[i] != '\0') {
        if (l.data[i] < r.data[i])
            return true;
        else if (l.data[i] > r.data[i])
            return false;
        ++i;
    }
    return (l.data[i] == '\0' && r.data[i] != '\0');
}

char& TString::operator[](int index) {
    if (index >= 0 && index < size) {
        return data[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}
const char& TString::operator[](int index) const {
    if (index >= 0 && index < size) {
        return data[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

size_t TString::Size() const {
    return size;
}

const char *TString::GetData() const {
    return data;
}

void TString::Move(char* str) {
    delete[] this->data;
    this->data = str;
    while (str[size] != '\0' && size < KEY_MAX_SIZE - 1) {
        ++size;
    }
    this->capacity = this->size + 1;
    str = nullptr;
}

void TString::Clear() {
    size = 0;
    if (data) {
        data[0] = '\0';
    }
}

void TString::PushBack(char c) {
    if (size + 1 > capacity) {
        Resize(size + 1);
    }
    data[size++] = c;
    data[size] = '\0';
}

void TString::Resize(size_t new_capacity) {
    capacity = new_capacity;
    char* new_data = new char[capacity + 1];
    for (size_t i = 0; i < size; ++i) {
        new_data[i] = data[i];
    }
    new_data[size] = '\0';
    delete[] data;
    data = new_data;
}

template <typename KeyType, typename ValueType>
class TAVLTree {
private:
    struct TNode {
        KeyType key;
        ValueType value;
        TNode *left, *right;
        unsigned char height;

        TNode(const KeyType &k, const ValueType &val) {
            key = k;
            value = val;
            left = right = nullptr;
            height = 1;
        }
    };
    TNode *root;

    unsigned char Max(unsigned char a, unsigned char b) {
        return a > b ? a : b;
    }

    unsigned char GetHeight(TNode *tree) {
        return tree == nullptr ? 0 : tree->height;
    }

    void FixHeight(TNode *tree) {
        tree->height = Max(GetHeight(tree->left), GetHeight(tree->right)) + 1;
    }

    int BFactor(TNode *tree) {
        return GetHeight(tree->right) - GetHeight(tree->left);
    }

    TNode *Min(TNode *tree) {
        while (tree->left != nullptr) {
            tree = tree->left;
        }
        return tree;
    }

    TNode *RemoveMin(TNode *tree) {
        if (tree->left == nullptr) {
            return tree->right;
        }
        tree->left = RemoveMin(tree->left);
        return Balance(tree);
    }

    TNode *RightRotate(TNode *tree) {
        TNode *tmp = tree->left;
        tree->left = tmp->right;
        tmp->right = tree;
        FixHeight(tree);
        FixHeight(tmp);
        return tmp;
    }

    TNode *LeftRotate(TNode *tree) {
        TNode *tmp = tree->right;
        tree->right = tmp->left;
        tmp->left = tree;
        FixHeight(tree);
        FixHeight(tmp);
        return tmp;
    }

    TNode *Balance(TNode *tree) {
        FixHeight(tree);
        if (BFactor(tree) < -1) {
            if (BFactor(tree->left) > 0) {
                tree->left = LeftRotate(tree->left);
            }
            return RightRotate(tree);
        } else if (BFactor(tree) > 1) {
            if (BFactor(tree->right) < 0) {
                tree->right = RightRotate(tree->right);
            }
            return LeftRotate(tree);
        }
        return tree;
    }

    TNode *FindTree(TNode *tree, const KeyType &k) {
        while (tree != nullptr) {
            if (k > tree->key) {
                tree = tree->right;
            } else if (k < tree->key) {
                tree = tree->left;
            } else {
                return tree;
            }
        }
        return tree;
    }

    TNode *InsertNode(TNode *tree, TNode *ins) {
        if (tree == nullptr) {
            return ins;
        } if (ins->key < tree->key) {
            TNode *tmp = InsertNode(tree->left, ins);
            if (tmp == nullptr) {
                return nullptr;
            }
            tree->left = tmp;
        } else if (ins->key > tree->key) {
            TNode *tmp = InsertNode(tree->right, ins);
            if (tmp == nullptr) {
                return nullptr;
            }
            tree->right = tmp;
        } else {
            return nullptr;
        }
        return Balance(tree);
    }

    TNode *RemoveNode(TNode *tree, const KeyType &k) {
        if (tree == nullptr) {
            return nullptr;
        }
        if (k < tree->key) {
            tree->left = RemoveNode(tree->left, k);;
        } else if (k > tree->key) {
            tree->right = RemoveNode(tree->right, k);
        } else {
            if (tree->right == nullptr) {
                TNode *tmp = tree->left;
                delete tree;
                return tmp;
            } else if (tree->left == nullptr) {
                TNode *tmp = tree->right;
                delete tree;
                return tmp;
            } else {
                TNode *m = Min(tree->right);
                TNode* tmp = new TNode(m->key, m->value);
                tmp->left = tree->left;
                tmp->right = RemoveMin(tree->right);
                delete tree;
                return Balance(tmp);
            }
        }
        return Balance(tree);
    }

    void DeleteTree(TNode *tree) {
        if (tree == nullptr) {
            return;
        }
        DeleteTree(tree->left);
        DeleteTree(tree->right);
        delete tree;
    }

    void Serialize(TNode *tree, std::ofstream &ofs) {
        if (tree == nullptr) {
            return;
        }

        size_t size = tree->key.Size();
        ofs.write(reinterpret_cast<char *>(&size), sizeof(size_t));
        ofs.write(tree->key.GetData(), size);
        ofs.write((char*) &tree->value, sizeof(ValueType));

        bool left = (tree->left != nullptr);
        bool right = (tree->right != nullptr);
        ofs.write(reinterpret_cast<char *>(&left), sizeof(bool));
        ofs.write(reinterpret_cast<char *>(&right), sizeof(bool));
        
        if (left) Serialize(tree->left, ofs);
        if (right) Serialize(tree->right, ofs);
    }

    TNode *Deserialize(std::ifstream &ifs) {
        size_t size = 0;
        ifs.read(reinterpret_cast<char *>(&size), sizeof(size_t));

        char *key = new char[size + 1];
        key[size] = '\0';
        ifs.read(key, size);

        ValueType val = 0;
        ifs.read(reinterpret_cast<char *>(&val), sizeof(ValueType));

        TNode *newNode = new TNode(key, val);
        newNode->key.Move(key);
        newNode->value = val;

        bool left = false;
        bool right = false;
        ifs.read((char*) &left, sizeof(bool));
        ifs.read((char*) &right, sizeof(bool));

        if (left) newNode->left = Deserialize(ifs);
        if (right) newNode->right = Deserialize(ifs);
        return newNode;
    }

public:
    void Load(std::ifstream &is) {
        DeleteTree(root);
        root = Deserialize(is);
    }

    void Save(std::ofstream &ofs) {
        Serialize(root, ofs);
    }

    ValueType *Find(const KeyType &k) {
        TNode *tmp = FindTree(root, k);
        return tmp == nullptr ? nullptr : &(tmp->value);
    }

    int Insert(const KeyType &key, const ValueType &val) {
        if (FindTree(root, key) != nullptr) {
            return 0;
        }
        root = InsertNode(root, new TNode(key, val));
        return 1;
    }

    int Remove(const KeyType &key) {
        if (FindTree(root, key) == nullptr) {
            return 0;
        }
        root = RemoveNode(root, key);
        return 1;
    }

    int Empty() {
        return root == nullptr;
    }

    TAVLTree() {
        root = nullptr;
    }

    ~TAVLTree() {
        DeleteTree(root);
    }
};

char ToLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

void ToLower(TString &str) {
    for (size_t i = 0; i < str.Size(); ++i) {
        str[i] = ToLowerChar(str[i]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    TAVLTree<TString, unsigned long long> tree;
    TString command;
    TString key;
    TString fileName;
    unsigned long long value;

    while (true) {
        std::cin >> std::ws;
        if (std::cin.eof()) {
            break;
        }
        std::cin >> command;
        if (command == "+") {
            std::cin >> key >> value;
            ToLower(key);
            if (tree.Insert(key, value)) {
                std::cout << "OK" << '\n';
            } else {
                std::cout << "Exist" << '\n';
            }
        } else if (command == "-") {
            std::cin >> key;
            ToLower(key);
            if (tree.Remove(key)) {
                std::cout << "OK" << '\n';
            } else {
                std::cout << "NoSuchWord" << '\n';
            }
        } else if (command == "!") {
            std::cin >> key;
            if (key == "Save") {
                std::cin >> fileName;
                std::ofstream ofs(fileName.GetData(), std::ios::binary | std::ios::out);
                tree.Save(ofs);
                std::cout << "OK" << '\n';
            } else if (key == "Load") {
                std::cin >> fileName;
                std::ifstream ifs(fileName.GetData(), std::ios::binary | std::ios::in);
                tree.Load(ifs);
                std::cout << "OK" << '\n';
            }
        } else {
            ToLower(command);
            unsigned long long *tmpValue;
            if ((tmpValue = tree.Find(command)) != nullptr) {
                std::cout << "OK: " << *tmpValue << '\n';
            } else {
                std::cout << "NoSuchWord" << '\n';
            }
        }
    }

    return 0;
}