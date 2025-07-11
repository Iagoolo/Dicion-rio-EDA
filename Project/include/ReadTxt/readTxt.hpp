#ifndef READ_TXT_HPP
#define READ_TXT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "../src/Dictionaty/IDictionary.hpp" 

class ReadTxt {
private:
    std::unordered_map<unsigned char, char> accent_map;

    void initialize_accent_map() {
        
        accent_map[0xa0] = 'a'; accent_map[0xa1] = 'a'; accent_map[0xa2] = 'a'; accent_map[0xa3] = 'a';
        accent_map[0xa7] = 'c';
        accent_map[0xa8] = 'e'; accent_map[0xa9] = 'e'; accent_map[0xaa] = 'e';
        accent_map[0xac] = 'i'; accent_map[0xad] = 'i';
        accent_map[0xb2] = 'o'; accent_map[0xb3] = 'o'; accent_map[0xb4] = 'o'; accent_map[0xb5] = 'o';
        accent_map[0xb9] = 'u'; accent_map[0xba] = 'u'; accent_map[0xbb] = 'u';
        accent_map[0x80] = 'a'; accent_map[0x81] = 'a'; accent_map[0x82] = 'a'; accent_map[0x83] = 'a';
        accent_map[0x87] = 'c';
        accent_map[0x88] = 'e'; accent_map[0x89] = 'e'; accent_map[0x8a] = 'e';
        accent_map[0x8c] = 'i'; accent_map[0x8d] = 'i';
        accent_map[0x92] = 'o'; accent_map[0x93] = 'o'; accent_map[0x94] = 'o'; accent_map[0x95] = 'o';
        accent_map[0x99] = 'u'; accent_map[0x9a] = 'u'; accent_map[0x9b] = 'u';
    }

    std::string clean_word(const std::string& raw_word) {
        std::string final_word;
        final_word.reserve(raw_word.length());
        for (size_t i = 0; i < raw_word.length(); ++i) {
            unsigned char c1 = raw_word[i];
            if (c1 == 0xc3 && i + 1 < raw_word.length()) {
                unsigned char c2 = raw_word[i+1];
                auto it = accent_map.find(c2);
                if (it != accent_map.end()) {
                    final_word += it->second;
                    i++;
                    continue;
                }
            }
            if (std::isalnum(c1)) {
                final_word += std::tolower(c1);
            } else if (c1 == '-' && i > 0 && i < raw_word.length() - 1) {
                final_word += '-';
            }
        }
        return final_word;
    }

public:
    ReadTxt() {
        initialize_accent_map();
    }

    void processFile(const std::string& filename, IDictionary<std::string, int>& dictionary) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir o ficheiro " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            
            std::string processed_line;
            processed_line.reserve(line.length());
            for (size_t i = 0; i < line.length(); ++i) {
                
                if (static_cast<unsigned char>(line[i]) == 0xE2 && i + 2 < line.length() &&
                    static_cast<unsigned char>(line[i+1]) == 0x80 &&
                    static_cast<unsigned char>(line[i+2]) == 0x94) {
                    processed_line += ' ';
                    i += 2; 
                } else {
                    processed_line += line[i];
                }
            }

            std::stringstream ss(processed_line);
            std::string word;
            while (ss >> word) {
                std::string cleaned_word = clean_word(word);
                if (cleaned_word.empty()) {
                    continue;
                }
                if (dictionary.contains(cleaned_word)) {
                    const int& current_freq = dictionary.get(cleaned_word);
                    dictionary.add(cleaned_word, current_freq + 1);
                } else {
                    dictionary.add(cleaned_word, 1);
                }
            }
        }
    }
};

#endif