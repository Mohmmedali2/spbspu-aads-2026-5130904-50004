#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H

#include "HashTable.h"
#include <ostream>
#include <string>
#include <vector>

class DictionaryManager {
private:
    struct Dictionary {
        std::string name;
        HashTable table;

        Dictionary(const std::string& dictionaryName, int tableSize = 1009)
            : name(dictionaryName), table(tableSize) {}
    };

    std::vector<Dictionary> dictionaries;
    int findDictionaryIndex(const std::string& name) const;

public:
    DictionaryManager();

    bool createDictionary(const std::string& name);
    bool deleteDictionary(const std::string& name);
    bool exists(const std::string& name) const;

    bool insertWord(const std::string& dictionaryName, const std::string& word);
    bool removeWord(const std::string& dictionaryName, const std::string& word);
    bool searchWord(const std::string& dictionaryName, const std::string& word, int& frequency) const;
    bool loadFromFile(const std::string& dictionaryName, const std::string& fileName, int& loadedWords);

    void showDictionary(const std::string& dictionaryName, std::ostream& out) const;
    void showTopWord(const std::string& dictionaryName, std::ostream& out) const;
    void clearDictionary(const std::string& dictionaryName);
    void showDictionaries(std::ostream& out) const;

    int count() const;
};

#endif
