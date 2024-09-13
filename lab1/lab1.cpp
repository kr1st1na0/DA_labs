#include <iostream>
#include <cstring>

const int NUMERAL_SYSTEM = 16;
const int KEY_MAX_SIZE = 33;
const int STRING_MAX_SIZE = 2049;

template <typename T>
class TVector {
private:
    T *data;
    size_t size, capacity;
public:
    TVector() : data{nullptr}, size(0), capacity(0) {}

    TVector(size_t initial_size) : size(initial_size), capacity(initial_size) {
        data = new T[capacity];
    }

    ~TVector() {
        delete[] data;
    }

    T *Data() const {
        return data;
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    void PushBack(const T &item) {
        if (size >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            T *newData = new T[capacity];
            if (data != nullptr) {                
                for (size_t i = 0; i < size; ++i) {
                    newData[i] = data[i];
                }
                delete[] data;
            }
            data = newData;
        }
        data[size++] = item;
    }

    T &operator[](size_t index) {
        if (index < size) {
            return data[index];
        }
        else {
            throw std::out_of_range("Index out of range");
        }
    }

    TVector<T> &operator=(const TVector &other) {
        size = other.Size();
        capacity = other.Capacity();
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }
};

class TString {
private:
    char *data;
    short len;
public:
    TString() : data(nullptr), len(0) {}
    
    TString(const char *s) {
        len = strlen(s);
        data = new char[len + 1];
        for (int i = 0 ; i < len; ++i) {
            data[i] = s[i];
        }
    }

    ~TString() {
        delete[] data;
    }

    short Length() const {
        return len;
    }

    TString &operator=(const TString &other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            data = new char[len + 1];
            for (int i = 0; i < len; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const TString &str) {
        for (int i = 0; i < str.Length(); ++i) {
            os << str.data[i];
        }
        return os;
    }
};

struct TItem {
    char key[KEY_MAX_SIZE];
    size_t value;

    TItem& operator=(const TItem& other) {
        if (this != &other) {
            for (int i = 0; i < KEY_MAX_SIZE; ++i) {
                key[i] = other.key[i];
            }
            value = other.value;
        }
        return *this;
    }
};

void RadixSort(TVector<TItem> &vec) {
    int count[NUMERAL_SYSTEM];
    size_t size = vec.Size();
    TVector<TItem> output(size);
    for (int i = KEY_MAX_SIZE - 2; i >= 0; --i) {
        for (int j = 0; j < NUMERAL_SYSTEM; ++j) {
            count[j] = 0;
        }
        for (size_t j = 0; j < size; ++j) {
            if (isdigit(vec[j].key[i])) {
                ++count[vec[j].key[i] - '0'];
            } else {
                ++count[vec[j].key[i] - 'a' + 10];
            }
        }
        for (int j = 1; j < NUMERAL_SYSTEM; ++j) {
            count[j] += count[j - 1];
        }
        for (size_t j = size; j > 0; --j) {
            if (isdigit(vec[j - 1].key[i])) {
                output[--count[vec[j - 1].key[i] - '0']] = vec[j - 1];
            } else {
                output[--count[vec[j - 1].key[i] - 'a' + 10]] = vec[j - 1];
            }
        }
        for (size_t j = 0; j < size; ++j) {
            vec[j] = output[j];
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    TVector<TItem> vec;
    TItem item;

    TVector<TString> strv;
    char str[STRING_MAX_SIZE];

    // Input vector
    size_t num = 0;
    while (std::cin >> item.key) {
        std::cin >> str;
        strv.PushBack(str);
        item.value = num;
        ++num;
        vec.PushBack(item);
    }

    // Radix sort
    RadixSort(vec);

    // Output vector
    for (int i = 0; i < vec.Size(); ++i) {
        std::cout << vec[i].key << '\t' << strv[vec[i].value] << '\n'; 
    }
}