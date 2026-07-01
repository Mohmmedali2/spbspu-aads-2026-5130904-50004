#include "TextProcessor.h"
#include <fstream>
#include <cctype>

std::string TextProcessor::cleanWord(const std::string& word) {
    std::string result;

    for (unsigned char c : word) {
        if (std::isalnum(c)) {
            result += static_cast<char>(std::tolower(c));
        }
    }

    return result;
}

std::vector<std::string> TextProcessor::loadWords(const std::string& fileName) {
    std::vector<std::string> words;
    std::ifstream file(fileName);

    if (!file.is_open()) return words;

    std::string word;
    while (file >> word) {
        std::string cleaned = cleanWord(word);
        if (!cleaned.empty()) words.push_back(cleaned);
    }

    return words;
}
