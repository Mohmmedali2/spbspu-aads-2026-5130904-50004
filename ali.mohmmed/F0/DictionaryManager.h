#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H

#include "HashTable.h"

#include <cstddef>
#include <iosfwd>
#include <string>

class DictionaryManager {
private:
    using WordTable = HashTable<std::string, std::size_t>;
    HashTable<std::string, WordTable> dictionaries_;

public:
    DictionaryManager();

    bool createDictionary(const std::string& name);
    bool deleteDictionary(const std::string& name);
    bool exists(const std::string& name) const;

    bool insertWord(const std::string& dictionaryName, const std::string& word);
    bool removeWord(const std::string& dictionaryName, const std::string& word);
    bool searchWord(const std::string& dictionaryName, const std::string& word, std::size_t& frequency) const;
    bool loadFromFile(const std::string& dictionaryName, const std::string& fileName, std::size_t& loadedWords);

    bool clearDictionary(const std::string& dictionaryName);

    void showDictionary(const std::string& dictionaryName, std::ostream& out) const;
    void showTopWord(const std::string& dictionaryName, std::ostream& out) const;
    void showDictionaries(std::ostream& out) const;

    std::size_t count() const;
};

#endif
