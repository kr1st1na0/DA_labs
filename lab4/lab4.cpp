#include <iostream>
#include <vector>

const unsigned short MAX_WORD_SIZE = 17;
const unsigned short ALPHABET_SIZE = 26;

struct TWord {
    TWord();

    friend bool operator==(const TWord &lhs, const TWord &rhs);
    friend bool operator!=(const TWord &lhs, const TWord &rhs);

    char word[MAX_WORD_SIZE];
    unsigned int stringID;
    unsigned int wordID;
    unsigned int size;
    unsigned int hash;
};

TWord::TWord() : wordID(0), stringID(0), hash(0), size(0) {}

bool operator==(const TWord &lhs, const TWord &rhs) {
    if (lhs.hash != rhs.hash) {
        return false;
    }
    for (int i = 0; i < lhs.size; ++i) {
        if (lhs.word[i] != rhs.word[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const TWord &lhs, const TWord &rhs) {
    return !(lhs == rhs);
}

inline void Clear(TWord &word) {
    word.hash = 0;
    word.size = 0;
}

std::vector<int> ZFunction(const std::vector<TWord> &str) {
    int n = (int) str.size();
    std::vector<int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= r) {
            z[i] = std::min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

std::vector<int> SPFunction(const std::vector<TWord> &str) {
    std::vector<int> z = ZFunction(str);
    int n = (int) z.size();
    std::vector<int> sp(n);
    for (int i = n - 1; i > 0; --i) {
        sp[i + z[i] - 1] = z[i];
    }
    return sp;
}

void KMP(const std::vector<TWord> &pattern, const std::vector<TWord> &text, const std::vector<int> &sp, int &start) {
    size_t m = pattern.size();
    size_t n = text.size();
    if (m > n) {
        return;
    }
    unsigned int i = start;
    while (i < n - m + 1) {
        unsigned int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            std::cout << text[i].stringID << ", " << text[i].wordID << "\n";
        } else if (j > 0 && j > sp[j - 1] + 1) {
            i = i + j - sp[j - 1] - 1;
        }
        ++i;
    }
    start = i - m;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector<TWord> pattern;
    std::vector<TWord> text;
    int start = 0;
    unsigned short ind = 0;
    TWord current;
    std::vector<int> sp;
    std::string buffer;

    getline(std::cin, buffer);
    for (auto &c: buffer) {
        if (c == ' ' || c == '\t') {
            if (ind > 0) {
                pattern.emplace_back(current);
            }
            Clear(current);
            ind = 0;
        } else {
            current.word[ind] = toupper(c);
            current.hash = current.hash * ALPHABET_SIZE + current.word[ind] - 'A';
            ++ind;
        }
    }
    if (ind > 0) {
        pattern.emplace_back(current);
        Clear(current);
        ind = 0;
    }

    sp = SPFunction(pattern);
    text.reserve(2 * pattern.size());
    current.wordID = 1;
    current.stringID = 1;
    while (getline(std::cin, buffer)) {
        for (auto &c: buffer) {
            if (c == '\t' || c == ' ') {
                if (ind > 0) {
                    text.emplace_back(current);
                    if (text.size() > 2 * pattern.size()) {
                        KMP(pattern, text, sp, start);
                        text.erase(text.begin(), text.begin() + (int) pattern.size());
                        text.reserve(2 * text.size());
                    }
                    ind = 0;
                    ++current.wordID;
                    Clear(current);
                }
            } else {
                current.word[ind] = toupper(c);
                current.hash = current.hash * ALPHABET_SIZE + current.word[ind] - 'A';
                ++ind;
            }
        }
        if (ind > 0) {
            text.emplace_back(current);
            if (text.size() > 2 * pattern.size()) {
                KMP(pattern, text, sp, start);
                text.erase(text.begin(), text.begin() + (int) pattern.size());
                text.reserve(2 * text.size());
            }
        }
        current.wordID = 1;
        ++current.stringID;
        Clear(current);
        ind = 0;
    }
    if (ind > 0) {
        text.emplace_back(current);
    }
    KMP(pattern, text, sp, start);

    return 0;
}