#include "DictionaryManager.h"
#include "TextProcessor.h"

#include <ostream>

DictionaryManager::DictionaryManager() :
    dictionaries_()
{}

bool DictionaryManager::createDictionary(const std::string& name) {
    if (name.empty() || dictionaries_.contains(name)) return false;
    dictionaries_.insert(name, WordTable());
    return true;
}

bool DictionaryManager::deleteDictionary(const std::string& name) {
    return dictionaries_.erase(name);
}

bool DictionaryManager::exists(const std::string& name) const {
    return dictionaries_.contains(name);
}

bool DictionaryManager::insertWord(const std::string& dictionaryName, const std::string& word) {
    WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) return false;

    std::string cleaned = TextProcessor::cleanWord(word);
    if (cleaned.empty()) return false;

    std::size_t* frequency = dictionary->find(cleaned);
    if (frequency == 0) {
        dictionary->insert(cleaned, 1);
    } else {
        ++(*frequency);
    }

    return true;
}

bool DictionaryManager::removeWord(const std::string& dictionaryName, const std::string& word) {
    WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) return false;

    std::string cleaned = TextProcessor::cleanWord(word);
    if (cleaned.empty()) return false;

    return dictionary->erase(cleaned);
}

bool DictionaryManager::searchWord(const std::string& dictionaryName, const std::string& word, std::size_t& frequency) const {
    const WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) return false;

    std::string cleaned = TextProcessor::cleanWord(word);
    const std::size_t* found = dictionary->find(cleaned);
    if (found == 0) return false;

    frequency = *found;
    return true;
}

bool DictionaryManager::loadFromFile(const std::string& dictionaryName, const std::string& fileName, std::size_t& loadedWords) {
    WordTable* dictionary = dictionaries_.find(dictionaryName);
    loadedWords = 0;

    if (dictionary == 0) return false;

    return TextProcessor::processFile(fileName, [dictionary, &loadedWords](const std::string& word) {
        std::size_t* frequency = dictionary->find(word);
        if (frequency == 0) {
            dictionary->insert(word, 1);
        } else {
            ++(*frequency);
        }
        ++loadedWords;
    });
}

bool DictionaryManager::clearDictionary(const std::string& dictionaryName) {
    WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) return false;

    dictionary->clear();
    return true;
}

void DictionaryManager::showDictionary(const std::string& dictionaryName, std::ostream& out) const {
    const WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) {
        out << "Dictionary not found.\n";
        return;
    }

    if (dictionary->empty()) {
        out << "Dictionary is empty.\n";
        return;
    }

    out << "Dictionary: " << dictionaryName << '\n';
    out << "Word\tFrequency\n";
    out << "------------------------\n";

    dictionary->forEach([&out](const std::string& word, const std::size_t& frequency) {
        out << word << '\t' << frequency << '\n';
    });
}

void DictionaryManager::showTopWord(const std::string& dictionaryName, std::ostream& out) const {
    const WordTable* dictionary = dictionaries_.find(dictionaryName);
    if (dictionary == 0) {
        out << "Dictionary not found.\n";
        return;
    }

    std::string topWord;
    std::size_t topFrequency = 0;

    dictionary->forEach([&topWord, &topFrequency](const std::string& word, const std::size_t& frequency) {
        if (frequency > topFrequency) {
            topWord = word;
            topFrequency = frequency;
        }
    });

    if (topFrequency == 0) {
        out << "Dictionary is empty.\n";
    } else {
        out << topWord << " -> " << topFrequency << '\n';
    }
}

void DictionaryManager::showDictionaries(std::ostream& out) const {
    if (dictionaries_.empty()) {
        out << "No dictionaries.\n";
        return;
    }

    out << "Dictionaries:\n";
    dictionaries_.forEach([&out](const std::string& name, const WordTable& dictionary) {
        out << name << " (" << dictionary.size() << " unique words)\n";
    });
}

std::size_t DictionaryManager::count() const {
    return dictionaries_.size();
}
