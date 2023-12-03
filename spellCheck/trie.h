#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <fstream>
#include <vector>

const int alphabet_size = 26;

class Trie{
public:
    struct trieNode {
        trieNode* children[alphabet_size];
        char letter;
        bool endOfWord;
        ~trieNode();
    };

    trieNode* children[alphabet_size];
    void Insert(const std::string& word);
    void InsertFromFile(const std::string& file);
    bool check(const std::string& word, std::vector<std::string>& similar);
    void doSimilar(std::vector<std::string>& similar);
    int levenshteinDistance(const std::string& word1, const std::string& word2) const;
    ~Trie();

};




#endif