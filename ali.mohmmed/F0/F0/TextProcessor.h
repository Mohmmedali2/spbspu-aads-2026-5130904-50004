#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include <vector>

class TextProcessor {
public:
    static std::vector<std::string> loadWords(const std::string& fileName);
    static std::string cleanWord(const std::string& word);
};

#endif
