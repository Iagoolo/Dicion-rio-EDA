#ifndef OUTPUT_WRITER_HPP
#define OUTPUT_WRITER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../Dictionaty/IDictionary.hpp"

/**
 * @brief Classe responsável por gerar um relatório de resultados formatado.
 * Cria um ficheiro de texto com tabelas ASCII para apresentar as métricas de desempenho
 * e a lista de frequência de palavras de forma clara e profissional.
 */
template <typename KeyType, typename ValueType>
class OutputWriter {
private:
    std::ofstream m_output_file;

    /**
     * @brief Imprime uma linha de separação para as tabelas.
     * @param widths Vetor com a largura de cada coluna.
     */
    void print_line(const std::vector<int>& widths) {
        m_output_file << "+";
        for (int w : widths) {
            m_output_file << std::string(w + 2, '-') << "+";
        }
        m_output_file << "\n";
    }

    /**
     * @brief Imprime uma linha de dados na tabela, com alinhamento.
     * @param cells Vetor de strings com o conteúdo de cada célula.
     * @param widths Vetor com a largura de cada coluna.
     */
    void print_row(const std::vector<std::string>& cells, const std::vector<int>& widths) {
        m_output_file << "|";
        for (size_t i = 0; i < cells.size(); ++i) {
            m_output_file << " " << std::left << std::setw(widths[i]) << cells[i] << " |";
        }
        m_output_file << "\n";
    }

public:
    /**
     * @brief Construtor que abre o ficheiro de saída.
     * @param output_filename O nome do ficheiro a ser criado.
     */
    OutputWriter(const std::string& output_filename) {
        m_output_file.open(output_filename);
        if (!m_output_file.is_open()) {
            std::cerr << "Erro ao criar o arquivo de saída: " << output_filename << std::endl;
        }
    }

    /**
     * @brief Destrutor que fecha o ficheiro.
     */
    ~OutputWriter() {
        if (m_output_file.is_open()) {
            m_output_file.close();
        }
    }

    /**
     * @brief Escreve o relatório completo no ficheiro.
     */
    void write_report(
        const std::string& structure_type,
        const std::string& input_filename,
        double duration_seconds,
        const IDictionary<KeyType, ValueType>& dictionary
    ) {
        if (!m_output_file.is_open()) return;

        // --- Cabeçalho do Relatório ---
        m_output_file << "======================================================\n";
        m_output_file << "      RELATÓRIO DE DESEMPENHO DO DICIONÁRIO\n";
        m_output_file << "======================================================\n\n";
        m_output_file << "Estrutura de Dados Testada: " << structure_type << "\n";
        m_output_file << "Ficheiro de Entrada: " << input_filename << "\n";
        m_output_file << "Palavras Únicas Encontradas: " << dictionary.size() << "\n\n";

        // --- Tabela de Métricas de Desempenho ---
        m_output_file << "--- Métricas de Desempenho ---\n";
        std::vector<int> metric_widths = {25, 20};
        print_line(metric_widths);
        print_row({"Métrica", "Valor"}, metric_widths);
        print_line(metric_widths);
        print_row({"Tempo de Execução (s)", std::to_string(duration_seconds)}, metric_widths);
        print_row({"Comparações Totais", std::to_string(dictionary.get_comparisons())}, metric_widths);
        if (dictionary.get_rotations() > 0)
            print_row({"Rotações", std::to_string(dictionary.get_rotations())}, metric_widths);
        if (dictionary.get_colors() > 0)
            print_row({"Trocas de Cor", std::to_string(dictionary.get_colors())}, metric_widths);
        if (dictionary.get_collisions() > 0)
            print_row({"Colisões", std::to_string(dictionary.get_collisions())}, metric_widths);
        print_line(metric_widths);

        // --- Tabela de Frequência de Palavras ---
        m_output_file << "\n--- Frequência de Palavras (Ordenado Alfabeticamente) ---\n";
        std::vector<int> freq_widths = {25, 20};
        print_line(freq_widths);
        print_row({"Palavra", "Frequência"}, freq_widths);
        print_line(freq_widths);

        auto keys = dictionary.get_all_keys_sorted();
        for (const auto& key : keys) {
            std::stringstream ss_key;
            ss_key << key; // Usa o operator<< sobrecarregado para lexicalStr
            print_row({ss_key.str(), std::to_string(dictionary.get(key))}, freq_widths);
        }
        print_line(freq_widths);

        std::cout << "Relatório de resultados salvo em: " << m_output_file_path << "\n";
    }

private:
    std::string m_output_file_path; // Caminho do ficheiro de saída
};

#endif
