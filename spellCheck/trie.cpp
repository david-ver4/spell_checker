#include "trie.h"

Trie::trieNode::~trieNode() {
    for (trieNode* n : children) {
        delete n;
    }
}

Trie::~Trie() {
    for (trieNode* n : children) {
        delete n;
    }
}

void Trie::Insert(const std::string& word) {
    for (char c : word) {
        if (!isalpha(c)) {
            return;
        }
    }

    trieNode* node = nullptr;

    for (char c : word) {
        c = tolower(c);
        if (node == nullptr) {
            if (children[c-97] == nullptr) {
                trieNode* newNode = new trieNode;
                newNode->letter = c;
                newNode->endOfWord = false;
                children[c-97] = newNode;
            }
            node = children[c-97];
        }
        else {
            if (node->children[c-97] == nullptr) {
                trieNode* newNode = new trieNode;
                newNode->letter = c;
                newNode->endOfWord = false;
                node->children[c-97] = newNode;
            }
            node = node->children[c-97];
        }
    }
    node->endOfWord = true;
}

void Trie::InsertFromFile(const std::string& file) {
    std::ifstream filepath;
    filepath.open(file);
    while (!filepath.eof()) {
        std::string word;
        getline(filepath, word);
        Insert(word);
    }
}

bool Trie::check(const std::string& word, std::vector<std::string>& similar) {
    trieNode* curr = nullptr;
    for (char c : word) {
        if (!isalpha(c)) {
            return false;
        }
        c = tolower(c);
        if (curr == nullptr) {
            if (children[c-97] == nullptr) {
                return false;
            }
            curr = children[c-97];
        }
        else {
            if (curr->children[c-97] == nullptr) {
                return false;
            }
            curr = curr->children[c-97];
        }
    }
    if (curr->endOfWord) {
        return true;
    }

    doSimilar(similar);
    return false;
}

int Trie::levenshteinDistance(const std::string &word1, const std::string &word2) const {
    const int m = word1.length();
    const int n = word2.length();

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0) {
                dp[i][j] = j;
            } else if (j == 0) {
                dp[i][j] = i;
            } else {
                dp[i][j] = std::min({ dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (word1[i - 1] == word2[j - 1] ? 0 : 1) });
            }
        }
    }

    return dp[m][n];
}

void Trie::doSimilar(std::vector<std::string> &similar) {

}
