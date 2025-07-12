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
#include "../include/utils/outputWriter.hpp"

template <typename KeyType>
void run_and_print_results(const std::string& structure_type, const std::string& filename) {
    std::unique_ptr<IDictionary<KeyType, size_t>> dictionary;

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

    auto start = std::chrono::high_resolution_clock::now();

    ReadTxt<KeyType> processor;
    processor.processFile(filename, *dictionary);

    auto end = std::chrono::high_resolution_clock::now();
    double duration_seconds = std::chrono::duration<double>(end - start).count();

    std::string output_filename = "output/saida_" + structure_type + ".txt";

    save_results_to_file(*dictionary, output_filename, duration_seconds);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <tipo_estrutura> <caminho_arquivo>" << std::endl;
        return 1;
    }

    std::string structure_type = argv[1];
    std::string filename = argv[2];

    if (structure_type == "avl" || structure_type == "rb") {
        run_and_print_results<lexicalStr>(structure_type, filename);
    } else if (structure_type == "chained_hash" || structure_type == "open_hash") {
        run_and_print_results<std::string>(structure_type, filename);
    } else {
        std::cerr << "Erro: Tipo de estrutura '" << structure_type << "' desconhecido." << std::endl;
        return 1;
    }

    return 0;
}
