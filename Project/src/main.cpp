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

/**
 * @brief Executa o processamento de um arquivo de entrada utilizando uma estrutura de dados especificada,
 *        mede o tempo de execução e gera um relatório de saída.
 *
 * Esta função instancia dinamicamente uma estrutura de dicionário (AVL, Rubro-Negra, Hash Encadeado ou Hash Aberto)
 * de acordo com o parâmetro 'structure_type'. Em seguida, processa o arquivo de entrada informado por 'filename',
 * armazenando os dados na estrutura escolhida. O tempo de processamento é medido e, ao final, um relatório é gerado
 * e salvo no arquivo especificado por 'output_filename'.
 *
 * @tparam KeyType Tipo da chave utilizada no dicionário.
 * @param structure_type Tipo da estrutura de dados a ser utilizada ("avl", "rb", "chained_hash" ou "open_hash").
 * @param filename Caminho para o arquivo de entrada a ser processado.
 * @param output_filename Caminho para o arquivo onde o relatório será salvo.
 */
template <typename KeyType>
void run_and_generate_report(const std::string& structure_type, const std::string& filename, const std::string& output_filename) {
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

    ReadTxt<KeyType> processor;

    auto start = std::chrono::high_resolution_clock::now();
    processor.processFile(filename, *dictionary);
    auto end = std::chrono::high_resolution_clock::now();
    double duration_seconds = std::chrono::duration<double>(end - start).count();

    OutputWriter<KeyType, size_t> writer(output_filename);
    writer.write_report(structure_type, filename, duration_seconds, *dictionary);
}

int main(int argc, char* argv[]) {
    std::cout << "Bem-vindo ao Dicionário EDA!" << std::endl;

    if ((argc != 3 && argc != 5) || (argc == 3 && argv[1] == std::string("--out"))) {
        std::cerr << "Uso:\n"
                  << "  " << argv[0] << " <tipo_estrutura> <caminho_arquivo> [--out <arquivo_saida>]\n"
                  << "  " << argv[0] << " --all <caminho_arquivo>\n"
                  << "Tipos disponíveis: avl, rb, chained_hash, open_hash\n";
        return 1;
    }

    std::string structure_type = argv[1];
    std::string filename = argv[2];

    if (structure_type == "--all") {
        std::vector<std::string> structures = {"avl", "rb", "chained_hash", "open_hash"};
        for (const auto& s : structures) {
            std::string output_filename = "output/resultado_" + s + ".txt";
            std::cout << "\n--> Processando com estrutura: " << s << std::endl;
            if (s == "avl" || s == "rb") {
                run_and_generate_report<lexicalStr>(s, filename, output_filename);
            } else {
                run_and_generate_report<std::string>(s, filename, output_filename);
            }
        }
    } else {
        std::string output_filename = "output/resultado_" + structure_type + ".txt";
        if (argc == 5) {
            std::string opt = argv[3];
            if (opt != "--out") {
                std::cerr << "Erro: argumento opcional inválido. Use '--out <arquivo_saida>'" << std::endl;
                return 1;
            }
            output_filename = argv[4];
        }

        std::cout << "Processando '" << filename << "' com a estrutura '" << structure_type << "'..." << std::endl;

        if (structure_type == "avl" || structure_type == "rb") {
            run_and_generate_report<lexicalStr>(structure_type, filename, output_filename);
        } else if (structure_type == "chained_hash" || structure_type == "open_hash") {
            run_and_generate_report<std::string>(structure_type, filename, output_filename);
        } else {
            std::cerr << "Erro: Tipo de estrutura '" << structure_type << "' desconhecido." << std::endl;
            return 1;
        }
    }

    std::cout << "Processamento concluído.\n";
    return 0;
}