#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <limits>  // Include for numeric_limits



class SpellChecker {
private:
    std::unordered_map<std::string, bool> wordMap;

    // Function to calculate Levenshtein distance between two words
    int levenshteinDistance(const std::string& word1, const std::string& word2) const {
        const int m = word1.length();
        const int n = word2.length();

        // Create a matrix to store the distances
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Initialize the matrix
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

public:
    void loadDictionary(const std::string& filename) {
        std::ifstream file(filename);
        std::string word;

        while (file >> word) {
            std::string lowercaseWord = word;
            std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);
            wordMap[lowercaseWord] = true;
        }

        file.close();
    }

    bool checkWord(const std::string& word) const {
        std::string lowercaseWord = word;
        std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

        return wordMap.find(lowercaseWord) != wordMap.end();
    }

    // Function to get suggestions for a misspelled word
    std::vector<std::string> getSuggestions(const std::string& misspelledWord) const {
        std::vector<std::string> suggestions;
        int minDistance = std::numeric_limits<int>::max();

        for (const auto& entry : wordMap) {
            int distance = levenshteinDistance(misspelledWord, entry.first);
            if (distance < minDistance) {
                minDistance = distance;
                suggestions.clear();
                suggestions.push_back(entry.first);
            } else if (distance == minDistance) {
                suggestions.push_back(entry.first);
            }
        }

        return suggestions;
    }
};

std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char ch : word) {
        if (!std::ispunct(ch)) {
            result += ch;
        }
    }
    return result;
}

int main() {
    SpellChecker spellChecker;
    spellChecker.loadDictionary("words.txt");

    std::string userInput;
    std::cout << "Enter a word or a sentence for spell check: ";
    std::getline(std::cin, userInput);

    std::istringstream iss(userInput);
    std::string word;
    bool allCorrect = true;

    auto start = std::chrono::high_resolution_clock::now();

    while (iss >> word) {
        std::string cleanedWord = removePunctuation(word);
        if (!spellChecker.checkWord(cleanedWord)) {
            std::cout << "The word '" << cleanedWord << "' is misspelled.\n";

            // Get suggestions and output them
            std::vector<std::string> suggestions = spellChecker.getSuggestions(cleanedWord);
            if (!suggestions.empty()) {
                std::cout << "Suggestions: ";
                for (const std::string& suggestion : suggestions) {
                    std::cout << suggestion << " ";
                }
                std::cout << "\n";
            }

            allCorrect = false;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    if (allCorrect) {
        std::cout << "All words are spelled correctly.\n";
    }

    std::cout << "Time taken for spell check: " << duration.count() << " microseconds\n";

    return 0;
}
