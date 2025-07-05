#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "AVL/avl.hpp"
#include "RB-TREE/rb_tree.hpp"
#include "Chained_Hash/ChainedHashTable.hpp"
#include "Open_Hash/OpenAddressingHashTable.hpp"


// ===== Função Auxiliar para Imprimir um Vetor de Dados de Teste =====
void imprimirDados(const std::vector<std::pair<std::string, int>>& dados) {
    std::cout << "Dados de teste:" << std::endl;
    for (const auto& par : dados) {
        std::cout << "  - Chave: \"" << par.first << "\", Valor: " << par.second << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}


int main() {
    // Dados de teste padronizados para todas as estruturas
    std::vector<std::pair<std::string, int>> dados_de_teste = {
        {"estrutura", 10}, {"de", 20}, {"dados", 30},
        {"arvore", 40},    {"tabela", 50}, {"hash", 60},
        {"projeto", 70},   {"final", 80}
    };

    imprimirDados(dados_de_teste);

    // ==========================================
    // ===== TESTE: ÁRVORE AVL ==================
    // ==========================================
    std::cout << "\n\n// ===== INICIANDO TESTE: ARVORE AVL ===== //" << std::endl;
    AVL<std::string, int> avlTree;

    std::cout << "\n>>> Fase de Insercao e Atualizacao <<<" << std::endl;
    for (const auto& par : dados_de_teste) {
        avlTree.insert(par.first, par.second);
    }
    std::cout << "Atualizando 'dados' para o valor 99..." << std::endl;
    avlTree.insert("dados", 99);
    
    std::cout << "\n>>> Fase de Busca e Acesso <<<" << std::endl;
    try {
        std::cout << "Valor de 'dados': " << avlTree.search("dados") << " (deve ser 99)" << std::endl;
        std::cout << "Tentando acessar 'inexistente'... ";
        avlTree.search("inexistente");
    } catch (const std::runtime_error& e) {
        std::cout << "Capturou excecao esperada: " << e.what() << std::endl;
    }
    
    std::cout << "\n>>> Fase de Remocao <<<" << std::endl;
    std::cout << "Removendo 'arvore'..." << std::endl;
    avlTree.remove("arvore");
    
    std::cout << "\n--- Metricas Finais para AVL ---" << std::endl;
    std::cout << "Comparacoes: " << avlTree.getComparisons() << std::endl;
    std::cout << "Rotacoes (metrica adicional): " << avlTree.getRotations() << std::endl;
    

    // ==========================================
    // ===== TESTE: ÁRVORE RUBRO-NEGRA ==========
    // ==========================================
    std::cout << "\n\n// ===== INICIANDO TESTE: ARVORE RUBRO-NEGRA ===== //" << std::endl;
    RB<std::string, int> rbTree;
    
    std::cout << "\n>>> Fase de Insercao e Atualizacao <<<" << std::endl;
    for (const auto& par : dados_de_teste) {
        rbTree.insert(par.first, par.second);
    }
    std::cout << "Atualizando 'dados' para o valor 99..." << std::endl;
    rbTree.insert("dados", 99);

    std::cout << "\n>>> Fase de Busca e Acesso <<<" << std::endl;
    try {
        std::cout << "Valor de 'dados': " << rbTree.search("dados") << " (deve ser 99)" << std::endl;
        std::cout << "Tentando acessar 'inexistente'... ";
        rbTree.search("inexistente");
    } catch (const std::runtime_error& e) {
        std::cout << "Capturou excecao esperada: " << e.what() << std::endl;
    }

    std::cout << "\n>>> Fase de Remocao <<<" << std::endl;
    std::cout << "Removendo 'arvore'..." << std::endl;
    rbTree.remove("arvore");

    std::cout << "\n--- Metricas Finais para Rubro-Negra ---" << std::endl;
    std::cout << "Comparacoes: " << rbTree.get_comparisons() << std::endl;
    std::cout << "Rotacoes: " << rbTree.get_rotations() << std::endl;
    std::cout << "Trocas de Cor (metrica adicional): " << rbTree.get_colors() << std::endl;


    // ================================================
    // ===== TESTE: TABELA HASH (ENCADEAMENTO) ========
    // ================================================
    std::cout << "\n\n// ===== INICIANDO TESTE: TABELA HASH ENCADEADA ===== //" << std::endl;
    ChainedHashTable<std::string, int> chainedHashTable(5);

    std::cout << "\n>>> Fase de Insercao e Atualizacao <<<" << std::endl;
    for (const auto& par : dados_de_teste) {
        chainedHashTable.add(par.first, par.second);
    }
    std::cout << "Atualizando 'dados' para o valor 99..." << std::endl;
    chainedHashTable.add("dados", 99);

    std::cout << "\n>>> Fase de Busca e Acesso <<<" << std::endl;
    try {
        std::cout << "Valor de 'dados': " << chainedHashTable.at("dados") << " (deve ser 99)" << std::endl;
        std::cout << "Tentando acessar 'inexistente'... ";
        chainedHashTable.at("inexistente");
    } catch (const std::out_of_range& e) {
        std::cout << "Capturou excecao esperada: " << e.what() << std::endl;
    }

    std::cout << "\n>>> Fase de Remocao <<<" << std::endl;
    std::cout << "Removendo 'arvore'..." << std::endl;
    chainedHashTable.remove("arvore");

    std::cout << "\n--- Metricas Finais para Tabela Hash (Encadeada) ---" << std::endl;
    std::cout << "Comparacoes: " << chainedHashTable.get_comparisons() << std::endl;
    std::cout << "Colisoes (metrica adicional): " << chainedHashTable.get_collisions() << std::endl;


    // ================================================
    // ===== TESTE: TABELA HASH (END. ABERTO) =========
    // ================================================
    std::cout << "\n\n// ===== INICIANDO TESTE: TABELA HASH END. ABERTO ===== //" << std::endl;
    OpenAddressingHashTable<std::string, int> openHashTable(11); // Tamanho primo > 8 para evitar rehash imediato

    std::cout << "\n>>> Fase de Insercao e Atualizacao <<<" << std::endl;
    for (const auto& par : dados_de_teste) {
        openHashTable.add(par.first, par.second);
    }
    std::cout << "Atualizando 'dados' para o valor 99..." << std::endl;
    openHashTable.add("dados", 99);

    std::cout << "\n>>> Fase de Busca e Acesso <<<" << std::endl;
    try {
        std::cout << "Valor de 'dados': " << openHashTable.at("dados") << " (deve ser 99)" << std::endl;
        std::cout << "Tentando acessar 'inexistente'... ";
        openHashTable.at("inexistente");
    } catch (const std::out_of_range& e) {
        std::cout << "Capturou excecao esperada: " << e.what() << std::endl;
    }

    std::cout << "\n>>> Fase de Remocao <<<" << std::endl;
    std::cout << "Removendo 'arvore'..." << std::endl;
    openHashTable.remove("arvore");

    std::cout << "\n--- Metricas Finais para Tabela Hash (End. Aberto) ---" << std::endl;
    std::cout << "Comparacoes: " << openHashTable.get_comparisons() << std::endl;
    std::cout << "Colisoes (metrica adicional): " << openHashTable.get_collisions() << std::endl;


    std::cout << "\n\n// ===== TESTES CONCLUIDOS ===== //" << std::endl;

    return 0;
}