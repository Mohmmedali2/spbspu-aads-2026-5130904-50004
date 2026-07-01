#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "HashTable.h"
#include <ostream>
#include <string>
#include <utility>
#include <vector>

class HashTable {
private:
    enum State { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        std::string word;
        int frequency;
        State state;

        Entry() : word(""), frequency(0), state(EMPTY) {}
    };

    std::vector<Entry> table;
    int tableSize;
    int currentSize;

    int hash1(const std::string& key) const;
    int hash2(const std::string& key) const;
    double loadFactor() const;
    bool isPrime(int number) const;
    int nextPrime(int number) const;
    void rehash();

public:
    explicit HashTable(int size = 1009);

    bool insert(const std::string& word);
    bool search(const std::string& word, int& frequency) const;
    bool remove(const std::string& word);

    int getFrequency(const std::string& word) const;
    bool contains(const std::string& word) const;

    void clear();
    void show(std::ostream& out) const;
    std::pair<std::string, int> topWord() const;

    int size() const;
    bool empty() const;

    std::vector<std::pair<std::string, int>> getAllWords() const;
};

#endif
