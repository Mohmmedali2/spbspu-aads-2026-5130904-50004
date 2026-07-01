#include "HashTable.h"

HashTable::HashTable(int size) {
    tableSize = nextPrime(size);
    currentSize = 0;
    table.resize(tableSize);
}

bool HashTable::isPrime(int number) const {
    if (number <= 1) return false;
    if (number <= 3) return true;
    if (number % 2 == 0 || number % 3 == 0) return false;

    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) return false;
    }

    return true;
}

int HashTable::nextPrime(int number) const {
    while (!isPrime(number)) number++;
    return number;
}

int HashTable::hash1(const std::string& key) const {
    unsigned long hash = 0;
    for (unsigned char c : key) {
        hash = (hash * 31 + c) % tableSize;
    }
    return static_cast<int>(hash);
}

int HashTable::hash2(const std::string& key) const {
    unsigned long hash = 0;
    for (unsigned char c : key) {
        hash = (hash * 17 + c) % (tableSize - 1);
    }
    return static_cast<int>(hash) + 1;
}

double HashTable::loadFactor() const {
    return static_cast<double>(currentSize) / tableSize;
}

void HashTable::rehash() {
    std::vector<Entry> oldTable = table;
    tableSize = nextPrime(tableSize * 2);
    table.clear();
    table.resize(tableSize);
    currentSize = 0;

    for (const Entry& entry : oldTable) {
        if (entry.state == OCCUPIED) {
            for (int i = 0; i < entry.frequency; i++) {
                insert(entry.word);
            }
        }
    }
}

bool HashTable::insert(const std::string& word) {
    if (word.empty()) return false;
    if (loadFactor() >= 0.7) rehash();

    int h1 = hash1(word);
    int h2 = hash2(word);
    int firstDeletedIndex = -1;

    for (int i = 0; i < tableSize; i++) {
        int index = (h1 + i * h2) % tableSize;

        if (table[index].state == OCCUPIED) {
            if (table[index].word == word) {
                table[index].frequency++;
                return true;
            }
        } else if (table[index].state == DELETED) {
            if (firstDeletedIndex == -1) firstDeletedIndex = index;
        } else {
            int target = (firstDeletedIndex != -1) ? firstDeletedIndex : index;
            table[target].word = word;
            table[target].frequency = 1;
            table[target].state = OCCUPIED;
            currentSize++;
            return true;
        }
    }

    if (firstDeletedIndex != -1) {
        table[firstDeletedIndex].word = word;
        table[firstDeletedIndex].frequency = 1;
        table[firstDeletedIndex].state = OCCUPIED;
        currentSize++;
        return true;
    }

    return false;
}

bool HashTable::search(const std::string& word, int& frequency) const {
    int h1 = hash1(word);
    int h2 = hash2(word);

    for (int i = 0; i < tableSize; i++) {
        int index = (h1 + i * h2) % tableSize;

        if (table[index].state == EMPTY) return false;

        if (table[index].state == OCCUPIED && table[index].word == word) {
            frequency = table[index].frequency;
            return true;
        }
    }

    return false;
}

bool HashTable::remove(const std::string& word) {
    int h1 = hash1(word);
    int h2 = hash2(word);

    for (int i = 0; i < tableSize; i++) {
        int index = (h1 + i * h2) % tableSize;

        if (table[index].state == EMPTY) return false;

        if (table[index].state == OCCUPIED && table[index].word == word) {
            table[index].word = "";
            table[index].frequency = 0;
            table[index].state = DELETED;
            currentSize--;
            return true;
        }
    }

    return false;
}

int HashTable::getFrequency(const std::string& word) const {
    int frequency = 0;
    return search(word, frequency) ? frequency : 0;
}

bool HashTable::contains(const std::string& word) const {
    int frequency = 0;
    return search(word, frequency);
}

void HashTable::clear() {
    table.clear();
    table.resize(tableSize);
    currentSize = 0;
}

void HashTable::show(std::ostream& out) const {
    if (currentSize == 0) {
        out << "Dictionary is empty.\n";
        return;
    }

    out << "Word\tFrequency\n";
    out << "------------------------\n";

    for (const Entry& entry : table) {
        if (entry.state == OCCUPIED) {
            out << entry.word << '\t' << entry.frequency << '\n';
        }
    }
}

std::pair<std::string, int> HashTable::topWord() const {
    std::string top = "";
    int maxFrequency = 0;

    for (const Entry& entry : table) {
        if (entry.state == OCCUPIED && entry.frequency > maxFrequency) {
            top = entry.word;
            maxFrequency = entry.frequency;
        }
    }

    return std::make_pair(top, maxFrequency);
}

int HashTable::size() const {
    return currentSize;
}

bool HashTable::empty() const {
    return currentSize == 0;
}

std::vector<std::pair<std::string, int>> HashTable::getAllWords() const {
    std::vector<std::pair<std::string, int>> words;

    for (const Entry& entry : table) {
        if (entry.state == OCCUPIED) {
            words.push_back(std::make_pair(entry.word, entry.frequency));
        }
    }

    return words;
}
