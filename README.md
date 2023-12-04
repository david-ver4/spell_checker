# GatorSpellcheck

GatorSpellcheck is a time-efficient spell check tool developed collaboratively by Alyssa Mann, David Vera, and Diego Aguilar. The project aims to provide a quick and effective solution for verifying the existence of words in a given or default dictionary.

## Problem Statement

The primary challenge addressed by GatorSpellcheck is the need for a spell check tool that operates with high time efficiency, especially considering the vast scope of dictionaries. The program takes a text file as input for the dictionary, prioritizing time complexity over space efficiency.

## Features

The project is considered successful when the implemented data structures consistently demonstrate higher time efficiency for word search and retrieval during spell checks. Key features include:

- Trie data structure for efficient word search
- Hash Map for converting strings into indices for word comparisons
- Suggested Words to present similar words after wrongly spelled word is inputted

## Tools Used

- **Programming Languages:** C++
- **Framework:** [SFML](https://www.sfml-dev.org/documentation/2.6.0/)
  - Learn how to install SFML: [SFML Installation Tutorial](https://youtu.be/PInJvpQSRHw)

## Implementation Strategy

- **Trie:** A 26-ary tree where each child represents a possible succeeding letter from the alphabet, efficiently used to retrieve spell-checked words after user input.
- **Hash Map:** Converts strings into indices for a "bucket" containing a collection of words, facilitating quick word comparisons.

## How to Use

1. Clone the repository.
2. Compile the C++ code using an appropriate compiler.
3. Run the executable to utilize the GatorSpellcheck tool.

## References

- [GeeksforGeeks - Trie Insert and Search](https://www.geeksforgeeks.org/trie-insert-and-search/)
- [DigitalOcean - Trie Data Structure in C++](https://www.digitalocean.com/community/tutorials/trie-data-structure-in-c-plus-plus)
- [GitHub - English Words Dataset](https://github.com/dwyl/english-words)
- [DigitalOcean - Hash Table in C++](https://www.digitalocean.com/community/tutorials/hash-table-in-c-plus-plus)
- [SFML Documentation](https://www.sfml-dev.org/documentation/2.6.0/)

## Future Enhancements

The team envisions adding more features to the spellcheck, including advanced suggestions for misspelled words, to further improve user experience and functionality.

---
*Note: Ensure you have the necessary tools and dependencies installed before running the code.*
