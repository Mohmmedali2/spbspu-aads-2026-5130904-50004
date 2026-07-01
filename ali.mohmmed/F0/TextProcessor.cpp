#include "TextProcessor.h"

#include <cctype>

std::string TextProcessor::cleanWord(const std::string& word) {
    std::string result;

    for (unsigned char symbol : word) {
        if (std::isalnum(symbol)) {
            result += static_cast<char>(std::tolower(symbol));
        }
    }

    return result;
}
