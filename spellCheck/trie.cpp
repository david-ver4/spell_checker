#include "trie.h"
#include <limits.h>

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
    if (word.length() == 0) {
        return;
    }
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
                size++;
                newNode->letter = c;
                newNode->endOfWord = false;
                children[c-97] = newNode;
            }
            node = children[c-97];
        }
        else {
            if (node->children[c-97] == nullptr) {
                trieNode* newNode = new trieNode;
                size++;
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
    if (!filepath.is_open()) {
        std::cout << "WTF" << std::endl;
        exit(-1);
    }
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
                doSimilarBottomUp(word, similar);
                return false;
            }
            curr = curr->children[c-97];
        }
    }
    if (curr->endOfWord) {
        return true;
    }

    doSimilarBottomUp(word, similar);
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
                dp[i][j] = std::min(std::min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + (word1[i - 1] == word2[j - 1] ? 0 : 1));
            }
        }
    }

    return dp[m][n];
}


void Trie::helper(const std::string &word, std::vector<std::string> &similar, std::vector<int> &dist, std::string curr, trieNode* root) {
    curr += root->letter;
    if (root->endOfWord) {
        int curr_dist = levenshteinDistance(word, curr);
        int max_dist = 0;
        int max_ind = 0;
        bool already_done = false;
        for (int i = 0; i < dist.size(); i++) {
            if (max_dist < dist[i]) {
                max_dist = dist[i];
                max_ind = i;
            }
            if (similar[i] == curr) {
                already_done = true;
            }
        }
        if (!already_done && curr != word && curr_dist < max_dist) {
            similar[max_ind] = curr;
            dist[max_ind] = curr_dist;
        }
    }
    for (trieNode* n : root->children) {
        if (n != nullptr) {
            helper(word, similar, dist, curr, n);
        }
    }
}

void Trie::doSimilar(const std::string& word, std::vector<std::string> &similar) {
    std::vector<int> dist(3, INT_MAX);
    similar = std::vector<std::string>(3);
    for (trieNode* n : children) {
        if (n != nullptr) {
            helper(word, similar, dist, "", n);
        }
    }
}

void Trie::doSimilarBottomUp(const std::string& word, std::vector<std::string> &similar) {
    std::vector<int> dist(3, INT_MAX);
    similar = std::vector<std::string>(3);
    trieNode* n = nullptr;
    trieNode* last = nullptr;
    std::string curr = "";
    for (char c : word) {
        curr += c;
        last = n;
        if (n == nullptr) {
            n = children[tolower(c)-97];
        }
        else {
            n = n->children[tolower(c)-97];
        }
        if (n == nullptr) {
            break;
        }
    }
    helper(word, similar, dist, curr.substr(0, curr.length()-2), last);
    if (similar[2] == "") {
        doSimilarBUHelper(word, similar, dist, "", children[tolower(word[0])-97]);
    }
}
void Trie::doSimilarBUHelper(const std::string& word, std::vector<std::string> &similar, std::vector<int> dist, std::string curr, trieNode* root) {
    if (root == nullptr || curr == word) {
        return;
    }
    doSimilarBUHelper(word, similar, dist, curr+root->letter, children[tolower(word[curr.length()+1])-97]);
    helper(word, similar, dist, curr, root);
}

