#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <fstream>
#include <string>

class TextProcessor {
public:
    static std::string cleanWord(const std::string& word);

    template <class Function>
    static bool processFile(const std::string& fileName, Function function) {
        std::ifstream file(fileName.c_str());
        if (!file.is_open()) return false;

        std::string word;
        bool hasWords = false;

        while (file >> word) {
            std::string cleaned = cleanWord(word);
            if (!cleaned.empty()) {
                function(cleaned);
                hasWords = true;
            }
        }

        return hasWords;
    }
};

#endif
