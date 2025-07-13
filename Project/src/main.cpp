#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <fstream> 
#include "../include/Dictionaty/IDictionary.hpp"
#include "../include/ReadTxt/readTxt.hpp"
#include "../include/utils/lexicalStr.hpp"
#include "../include/AVL/avl.hpp"
#include "../include/RB-TREE/rb_tree.hpp"
#include "../include/Chained_Hash/ChainedHashTable.hpp"
#include "../include/Open_Hash/OpenAddressingHashTable.hpp"
#include "../include/utils/outputWriter.hpp" // Inclui a nova classe

/**
 * @brief Função template que executa o ciclo completo de teste para uma estrutura de dados.
 * Cria a estrutura, processa o ficheiro, mede o tempo e gera o relatório de saída.
 * @tparam KeyType O tipo da chave a ser usada (std::string ou lexicalStr).
 * @param structure_type O nome da estrutura para o relatório.
 * @param filename O caminho do ficheiro de texto a ser processado.
 */
template <typename KeyType>
void run_and_generate_report(const std::string& structure_type, const std::string& filename) {
    // Cria um ponteiro para a interface com o tipo de chave correto
    std::unique_ptr<IDictionary<KeyType, size_t>> dictionary;

    // Cria a instância correta da estrutura de dados
    if (structure_type == "avl") {
        dictionary = std::make_unique<AVL<KeyType, size_t>>();
    } else if (structure_type == "rb") {
        dictionary = std::make_unique<RB<KeyType, size_t>>();
    } else if (structure_type == "chained_hash") {
        dictionary = std::make_unique<ChainedHashTable<KeyType, size_t>>();
    } else if (structure_type == "open_hash") {
        dictionary = std::make_unique<OpenAddressingHashTable<KeyType, size_t>>();
    } else {
        std::cerr << "Tipo de estrutura desconhecido: " << structure_type << std::endl;
        return;
    }

    // Processa o ficheiro e mede o tempo
    ReadTxt<KeyType> processor;
    auto start = std::chrono::high_resolution_clock::now();
    processor.processFile(filename, *dictionary);
    auto end = std::chrono::high_resolution_clock::now();
    double duration_seconds = std::chrono::duration<double>(end - start).count();

    // --- MUDANÇA PRINCIPAL: USO DO OUTPUTWRITER ---
    
    // 1. Define um nome de ficheiro de saída único para este teste.
    std::string output_filename = "output/resultado_" + structure_type + ".txt";

    // 2. Cria um objeto OutputWriter para esse ficheiro.
    OutputWriter<KeyType, size_t> writer(output_filename);

    // 3. Chama o método para gerar o relatório completo.
    writer.write_report(structure_type, filename, duration_seconds, *dictionary);
}

int main(int argc, char* argv[]) {
    std::cout << "Bem-vindo ao Dicionário EDA!" << std::endl;

    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tipo_estrutura> <caminho_arquivo>" << std::endl;
        std::cerr << "Tipos de estrutura disponiveis: avl, rb, chained_hash, open_hash" << std::endl;
        return 1;
    }

    std::string structure_type = argv[1];
    std::string filename = argv[2];

    std::cout << "Processando '" << filename << "' com a estrutura '" << structure_type << "'..." << std::endl;

    // Lógica para decidir qual tipo de chave usar (lexicalStr ou std::string)
    if (structure_type == "avl" || structure_type == "rb") {
        run_and_generate_report<lexicalStr>(structure_type, filename);
    } else if (structure_type == "chained_hash" || structure_type == "open_hash") {
        run_and_generate_report<std::string>(structure_type, filename);
    } else {
        std::cerr << "Erro: Tipo de estrutura '" << structure_type << "' desconhecido." << std::endl;
        return 1;
    }

    std::cout << "Processamento concluído." << std::endl;

    return 0;
}