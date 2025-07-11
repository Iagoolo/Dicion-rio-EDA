// src/main.cpp (Aplicação Final Revisada)

#include <iostream>
#include <string>
#include <vector>
#include <memory> // Para std::unique_ptr
#include <chrono>
#include <iomanip>
#include <stdexcept> // Para std::out_of_range

// Inclua a interface, o processador e todas as implementações de ED
#include "Dictionaty/IDictionary.hpp"
#include "ReadTxt/readTxt.hpp"
#include "AVL/avl.hpp"
#include "RB-TREE/rb_tree.hpp"
#include "Chained_Hash/ChainedHashTable.hpp"
#include "Open_Hash/OpenAddressingHashTable.hpp"

// Função para imprimir os resultados de forma organizada
void print_results(const std::string& structure_type, double time_ms, const IDictionary<std::string, int>& dict) {
    std::cout << "\n==================================================" << std::endl;
    std::cout << "      RESULTADOS PARA: " << structure_type << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Tempo total de construção do dicionário: " << time_ms << " ms" << std::endl;
    std::cout << "Número total de palavras únicas: " << dict.size() << std::endl;
    std::cout << "---------------- Métricas --------------------" << std::endl;
    std::cout << "  - Comparações Totais: " << dict.get_comparisons() << std::endl;
    
    // Imprime métricas específicas apenas se forem diferentes de zero
    if (dict.get_rotations() > 0) {
        std::cout << "  - Rotações: " << dict.get_rotations() << std::endl;
    }
    if (dict.get_colors() > 0) {
        std::cout << "  - Trocas de Cor: " << dict.get_colors() << std::endl;
    }
    if (dict.get_collisions() > 0) {
        std::cout << "  - Colisões: " << dict.get_collisions() << std::endl;
    }
    std::cout << "==================================================" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Bem-vindo ao Dicionário EDA!" << std::endl;

    // 1. Validação dos Argumentos de Linha de Comando
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tipo_estrutura> <caminho_arquivo>" << std::endl;
        std::cerr << "Tipos de estrutura disponiveis: avl, rb, chained_hash, open_hash" << std::endl;
        return 1;
    }

    std::string structure_type = argv[1];
    std::string filename = argv[2];

    std::cout << "Estrutura selecionada: " << structure_type << std::endl;
    std::cout << "Arquivo a ser processado: " << filename << std::endl;
    // 2. Criação Polimórfica da Estrutura de Dados
    std::unique_ptr<IDictionary<std::string, int>> dictionary;

    if (structure_type == "avl") {
        std::cout << "Usando AVL Tree..." << std::endl;
        dictionary = std::make_unique<AVL<std::string, int>>();
    } else if (structure_type == "rb") {
        dictionary = std::make_unique<RB<std::string, int>>();
    } else if (structure_type == "chained_hash") {
        dictionary = std::make_unique<ChainedHashTable<std::string, int>>();
    } else if (structure_type == "open_hash") {
        dictionary = std::make_unique<OpenAddressingHashTable<std::string, int>>();
    } else {
        std::cerr << "Erro: Tipo de estrutura '" << structure_type << "' desconhecido." << std::endl;
        return 1;
    }

    // 3. Processamento e Medição de Desempenho
    ReadTxt processor;
    
    std::cout << "Processando o ficheiro '" << filename << "' com a estrutura '" << structure_type << "'..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    processor.processFile(filename, *dictionary);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    // 4. Apresentação dos Resultados
    print_results(structure_type, elapsed.count(), *dictionary);

    // 5. Impressão Opcional da Frequência (AGORA ATIVADO E MAIS SEGURO)
    if (dictionary) {
        std::cout << "\n--- Frequencia de todas as palavras (ordenado) ---" << std::endl;
        try {
            // Assumindo que a sua interface e classes implementam get_all_keys_sorted()
            std::vector<std::string> keys = dictionary->get_all_keys_sorted();
            // Imprime todas as chaves (palavras) separadas por vírgula
            std::cout << "Palavras únicas: ";
            for (size_t i = 0; i < keys.size(); ++i) {
                std::cout << keys[i];
                if (i != keys.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
            for(const auto& key : keys) {
                std::cout << key << ": " << dictionary->get(key) << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Ocorreu um erro ao imprimir as frequencias: " << e.what() << std::endl;
        }
    }

    return 0;
}
