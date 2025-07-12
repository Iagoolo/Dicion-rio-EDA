#ifndef READTXT_HPP
#define READTXT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "Dictionaty/IDictionary.hpp" 
#include "../utils/lexicalStr.hpp"

/**
 * @brief Classe responsável por processar ficheiros de texto e popular um dicionário.
 * Esta versão final preserva os caracteres acentuados, apenas convertendo
 * as letras maiúsculas (incluindo as acentuadas) para minúsculas.
 */
template <typename KeyType>
class ReadTxt {
private:
    std::unordered_map<unsigned char, unsigned char> accent_map;

    /**
     * @brief Inicializa o mapa de conversão de acentos no construtor.
     */
    void initialize_accent_map() {
        // Mapeia o segundo byte de um caractere UTF-8 MAIÚSCULO para o seu MINÚSCULO equivalente.
        accent_map[0x80] = 0xa0; // À -> à
        accent_map[0x81] = 0xa1; // Á -> á
        accent_map[0x82] = 0xa2; // Â -> â
        accent_map[0x83] = 0xa3; // Ã -> ã
        accent_map[0x87] = 0xa7; // Ç -> ç
        accent_map[0x88] = 0xa8; // È -> è
        accent_map[0x89] = 0xa9; // É -> é
        accent_map[0x8a] = 0xaa; // Ê -> ê
        accent_map[0x8c] = 0xac; // Ì -> ì
        accent_map[0x8d] = 0xad; // Í -> í
        accent_map[0x92] = 0xb2; // Ò -> ò
        accent_map[0x93] = 0xb3; // Ó -> ó
        accent_map[0x94] = 0xb4; // Ô -> ô
        accent_map[0x95] = 0xb5; // Õ -> õ
        accent_map[0x99] = 0xb9; // Ù -> ù
        accent_map[0x9a] = 0xba; // Ú -> ú
    }

    /**
     * @brief Limpa uma palavra, preservando acentos, mas convertendo para minúsculas.
     */
    std::string clean_word(const std::string& raw_word) {
        std::string final_word;
        final_word.reserve(raw_word.length());

        for (size_t i = 0; i < raw_word.length(); ++i) {
            unsigned char c1 = raw_word[i];

            // Verifica se é um caractere UTF-8 de 2 bytes (acentos em português começam com 0xc3)
            if (c1 == 0xc3 && i + 1 < raw_word.length()) {
                unsigned char c2 = raw_word[i+1];
                auto it = accent_map.find(c2);

                // Se for um caractere acentuado MAIÚSCULO que conhecemos, converte para minúsculo
                if (it != accent_map.end()) {
                    final_word += (char)c1;
                    final_word += (char)it->second; // Adiciona a versão minúscula
                } 
                // Caso contrário, assume que é um caractere acentuado minúsculo válido e o mantém
                else {
                    final_word += (char)c1;
                    final_word += (char)c2;
                }
                i++; // Pula o segundo byte do caractere UTF-8
                continue;
            }

            // Para caracteres ASCII normais (sem acento)
            if (std::isalpha(c1)) {
                final_word += std::tolower(c1);
            } else if (std::isdigit(c1) || (c1 == '-' && i > 0 && i < raw_word.length() - 1)) {
                final_word += c1; // Mantém números e hífens no meio da palavra
            }
        }
        return final_word;
    }

public:
    ReadTxt() {
        initialize_accent_map();
    }

    /**
     * @brief Processa um ficheiro de texto, conta a frequência das palavras e preenche o dicionário.
     */
    void processFile(const std::string& filename, IDictionary<KeyType, size_t>& dictionary) {
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
                // O travessão (em-dash) em UTF-8 é a sequência de 3 bytes: 0xE2, 0x80, 0x94
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

                KeyType key(cleaned_word);

                if (dictionary.contains(key)) {
                    const int& current_freq = dictionary.get(key);
                    dictionary.add(key, current_freq + 1);
                } else {
                    dictionary.add(key, 1);
                }
            }
        }
    }
};

#endif