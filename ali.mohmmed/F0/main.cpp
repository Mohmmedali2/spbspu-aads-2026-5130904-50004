#include "DictionaryManager.h"

#include <iostream>
#include <sstream>
#include <string>

void printHelp(std::ostream& out) {
    out << "Commands:\n";
    out << "create <dictionary>\n";
    out << "delete <dictionary>\n";
    out << "load <dictionary> <file-name>\n";
    out << "insert <dictionary> <word>\n";
    out << "search <dictionary> <word>\n";
    out << "remove <dictionary> <word>\n";
    out << "show <dictionary>\n";
    out << "top <dictionary>\n";
    out << "clear <dictionary>\n";
    out << "list\n";
    out << "help\n";
    out << "exit\n";
}

int main() {
    DictionaryManager manager;
    std::string line;

    std::cout << "Frequency Dictionary\n";
    std::cout << "Type 'help' to show commands.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "exit") {
            break;
        } else if (command == "help") {
            printHelp(std::cout);
        } else if (command == "create") {
            std::string dictionary;
            ss >> dictionary;

            if (dictionary.empty()) {
                std::cout << "Usage: create <dictionary>\n";
            } else if (manager.createDictionary(dictionary)) {
                std::cout << "Dictionary created.\n";
            } else {
                std::cout << "Dictionary already exists or invalid name.\n";
            }
        } else if (command == "delete") {
            std::string dictionary;
            ss >> dictionary;

            if (dictionary.empty()) {
                std::cout << "Usage: delete <dictionary>\n";
            } else if (manager.deleteDictionary(dictionary)) {
                std::cout << "Dictionary deleted.\n";
            } else {
                std::cout << "Dictionary not found.\n";
            }
        } else if (command == "load") {
            std::string dictionary;
            std::string fileName;
            ss >> dictionary >> fileName;

            if (dictionary.empty() || fileName.empty()) {
                std::cout << "Usage: load <dictionary> <file-name>\n";
            } else {
                int loadedWords = 0;

                if (manager.loadFromFile(dictionary, fileName, loadedWords)) {
                    std::cout << "File loaded. Words added: " << loadedWords << '\n';
                } else {
                    std::cout << "Operation failed. Check dictionary name or file.\n";
                }
            }
        } else if (command == "insert") {
            std::string dictionary;
            std::string word;
            ss >> dictionary >> word;

            if (dictionary.empty() || word.empty()) {
                std::cout << "Usage: insert <dictionary> <word>\n";
            } else if (manager.insertWord(dictionary, word)) {
                std::cout << "Inserted.\n";
            } else {
                std::cout << "Operation failed.\n";
            }
        } else if (command == "search") {
            std::string dictionary;
            std::string word;
            ss >> dictionary >> word;

            if (dictionary.empty() || word.empty()) {
                std::cout << "Usage: search <dictionary> <word>\n";
            } else {
                int frequency = 0;

                if (manager.searchWord(dictionary, word, frequency)) {
                    std::cout << word << " -> " << frequency << '\n';
                } else {
                    std::cout << "Word not found.\n";
                }
            }
        } else if (command == "remove") {
            std::string dictionary;
            std::string word;
            ss >> dictionary >> word;

            if (dictionary.empty() || word.empty()) {
                std::cout << "Usage: remove <dictionary> <word>\n";
            } else if (manager.removeWord(dictionary, word)) {
                std::cout << "Removed.\n";
            } else {
                std::cout << "Word not found.\n";
            }
        } else if (command == "show") {
            std::string dictionary;
            ss >> dictionary;

            if (dictionary.empty()) {
                std::cout << "Usage: show <dictionary>\n";
            } else {
                manager.showDictionary(dictionary, std::cout);
            }
        } else if (command == "top") {
            std::string dictionary;
            ss >> dictionary;

            if (dictionary.empty()) {
                std::cout << "Usage: top <dictionary>\n";
            } else {
                manager.showTopWord(dictionary, std::cout);
            }
        } else if (command == "clear") {
            std::string dictionary;
            ss >> dictionary;

            if (dictionary.empty()) {
                std::cout << "Usage: clear <dictionary>\n";
            } else if (manager.exists(dictionary)) {
                manager.clearDictionary(dictionary);
                std::cout << "Dictionary cleared.\n";
            } else {
                std::cout << "Dictionary not found.\n";
            }
        } else if (command == "list") {
            manager.showDictionaries(std::cout);
        } else {
            std::cout << "Unknown command. Type 'help'.\n";
        }
    }

    std::cout << "Program finished.\n";
    return 0;
}
