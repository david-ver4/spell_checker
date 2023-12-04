#ifndef HASH_H
#define HASH_H

#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <limits>
#include <iostream>

unsigned int customHash(const std::string& key, int tableSize);

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;

    KeyValuePair(const K& k, const V& v);
};

template <typename K, typename V, int TableSize>
class HashMap {
private:
    std::vector<KeyValuePair<K, V>> table[TableSize];
    unsigned int  s = 0;

public:
    void insert(const K& key, const V& value);
    bool find(const K& key, V& value) const;
    void forEach(std::function<void(const KeyValuePair<K, V>&)> callback) const;
    unsigned int size() const {return s;}
};

class SpellChecker {
private:
    HashMap<std::string, bool, 100> wordMap;

    int levenshteinDistance(const std::string& word1, const std::string& word2) const;

public:
    void loadDictionary(const std::string& filename);
    bool checkWord(const std::string& word) const;
    std::vector<std::string> getSuggestions(const std::string& misspelledWord) const;
    const HashMap<std::string, bool, 100>& getWordMap() {return wordMap;}
};

std::string removePunctuation(const std::string& word);

#endif // HASH_H
