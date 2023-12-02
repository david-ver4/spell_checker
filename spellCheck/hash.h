#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <limits>

class SpellChecker {
private:
    std::unordered_map<std::string, bool> wordMap;

    int levenshteinDistance(const std::string& word1, const std::string& word2) const;

public:
    void loadDictionary(const std::string& filename);

    bool checkWord(const std::string& word) const;

    std::vector<std::string> getSuggestions(const std::string& misspelledWord) const;
};

std::string removePunctuation(const std::string& word);

#endif  // SPELLCHECKER_H
