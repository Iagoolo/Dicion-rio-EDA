#ifndef OUTPUTWRITER_HPP
#define OUTPUTWRITER_HPP

#include <fstream>
#include <iomanip>
#include <iostream>
#include "../Dictionaty/IDictionary.hpp"

template <typename KeyType>
void save_results_to_file(
    const IDictionary<KeyType, size_t>& dictionary,
    const std::string& output_filename,
    double duration_seconds
) {
    std::ofstream output(output_filename);
    if (!output.is_open()) {
        std::cerr << "Erro ao criar o arquivo de saída: " << output_filename << std::endl;
        return;
    }

    output << "--- Frequencia de todas as palavras (ordenado) ---\n";
    auto keys = dictionary.get_all_keys_sorted();
    for (const auto& key : keys) {
        output << key << ": " << dictionary.get(key) << "\n";
    }

    output << "\n--- Estatísticas ---\n";
    output << "Tempo de execução: " << std::fixed << std::setprecision(6)
           << duration_seconds << " segundos\n";
    output << "Comparações: " << dictionary.get_comparisons() << "\n";
    output << "Colisões: " << dictionary.get_collisions() << "\n";
    output << "Rotações: " << dictionary.get_rotations() << "\n";
    output << "Alterações de cor: " << dictionary.get_colors() << "\n";

    output.close();

    std::cout << "Resultados salvos em: " << output_filename << "\n";
}

#endif
