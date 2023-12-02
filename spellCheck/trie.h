#ifndef TRIE_H
#define TRIE_H
#include <iostream>

const int alphabet_size = 26;

class Trie{
public:
    struct trieNode {
        trieNode* children[alphabet_size];
        char letter;
        bool endOfWord;
    };
};




#endif