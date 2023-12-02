cmake_minimum_required(VERSION 3.24)
project(spellCheck)

set(CMAKE_CXX_STANDARD 17)

add_executable(spellCheck hash_map.cpp hash.h trie.cpp trie.h)
