#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <fstream>
#include <vector>

const int alphabet_size = 26;

class Trie{
public:
    struct trieNode {
        trieNode() {
            for (int i = 0; i < alphabet_size; i++) {
                children[i] = nullptr;
            }
        }
        trieNode* children[alphabet_size];
        char letter;
        bool endOfWord;
        ~trieNode();
    };

    Trie() {
        for (int i = 0; i < alphabet_size; i++) {
            children[i] = nullptr;
        }
    }
    trieNode* children[alphabet_size];
    void Insert(const std::string& word);
    void InsertFromFile(const std::string& file);
    bool check(const std::string& word, std::vector<std::string>& similar);
    void helper(const std::string& word, std::vector<std::string>& similar, std::vector<int>& dist, std::string curr, trieNode* root);
    void doSimilar(const std::string& word, std::vector<std::string>& similar);
    int levenshteinDistance(const std::string& word1, const std::string& word2) const;
    ~Trie();

};




#endif