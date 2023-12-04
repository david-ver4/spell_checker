#include "hash.h"

// custom hash function for strings
unsigned int customHash(const std::string& key, int tableSize) {
    unsigned int hash = 0;
    // iterate through each character in the key
    for (char ch : key) {
        hash = (hash * 31) + static_cast<unsigned int>(ch);
    }
    // ensure the hash fits within the table size and return
    return hash % tableSize;
}

// constructor for key-value pair
template <typename K, typename V>
KeyValuePair<K, V>::KeyValuePair(const K& k, const V& v) : key(k), value(v) {}

// insert key-value pair into the hash map
template <typename K, typename V, int TableSize>
void HashMap<K, V, TableSize>::insert(const K& key, const V& value) {
    unsigned int index = customHash(key, TableSize);
    s -= table[index].size();
    table[index].emplace_back(key, value);
    s += table[index].size();
}

// find value associated with the given key in the hash map
template <typename K, typename V, int TableSize>
bool HashMap<K, V, TableSize>::find(const K& key, V& value) const {
    unsigned int index = customHash(key, TableSize);
    for (const auto& pair : table[index]) {
        // if key is found, update value and return true
        if (pair.key == key) {
            value = pair.value;
            return true;
        }
    }
    return false;
}

// perform a callback function on each key-value pair in the hash map
template <typename K, typename V, int TableSize>
void HashMap<K, V, TableSize>::forEach(std::function<void(const KeyValuePair<K, V>&)> callback) const {
    // iterate through each bucket in the hash map
    for (const auto& bucket : table) {
        // iterate through each key-value pair in the bucket and apply the callback
        for (const auto& pair : bucket) {
            callback(pair);
        }
    }
}

// load dictionary from a file into the spell checker's word map
void SpellChecker::loadDictionary(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;

    while (file >> word) {
        std::string lowercaseWord = word;
        std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);
        wordMap.insert(lowercaseWord, true);
    }

    file.close();
}

// check if a given word is present in the spell checker's word map
bool SpellChecker::checkWord(const std::string& word) const {
    std::string lowercaseWord = word;
    std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

    bool isWordPresent = false;
    wordMap.find(lowercaseWord, isWordPresent);

    return isWordPresent;
}

// get suggestions for a misspelled word from the spell checker's word map
std::vector<std::string> SpellChecker::getSuggestions(const std::string& misspelledWord) const {
    std::vector<std::string> suggestions;
    int minDistance = std::numeric_limits<int>::max();

    wordMap.forEach([&](const auto& entry) {
        int distance = levenshteinDistance(misspelledWord, entry.key);
        // calculate the levenshtein distance between the misspelled word and each entry in the map
        if (distance < minDistance) {
            // update suggestions based on the calculated distance
            minDistance = distance;
            suggestions.clear();
            suggestions.push_back(entry.key);
        } else if (distance == minDistance) {
            suggestions.push_back(entry.key);
        }
    });

    return suggestions;
}

// calculate the levenshtein distance between two strings
int SpellChecker::levenshteinDistance(const std::string &word1, const std::string &word2) const {
    const int m = word1.length();
    const int n = word2.length();
    // initialize a 2D vector for dynamic programming
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    // fill in the table
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

// remove punctuation from a given word
std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (!std::ispunct(ch)) {
            result += ch;
        }
    }
    return result;
}

