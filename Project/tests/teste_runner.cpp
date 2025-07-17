#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <random>
#include <map>
#include <iomanip>
#include <fstream>

#include "../include/AVL/avl.hpp"
#include "../include/RB-TREE/rb_tree.hpp"
#include "../include/Chained_Hash/ChainedHashTable.hpp"
#include "../include/Open_Hash/OpenAddressingHashTable.hpp"

//==================================================================
// ESTRUTURA DE TESTES DE CORREÇÃO
//==================================================================
int tests_passed = 0;
int tests_failed = 0;

#define ASSERT_EQUAL(a, b) \
    if ((a) != (b)) { \
        std::cerr << "  -> ASSERT FAILED: " << #a << " != " << #b << " (Got: " << (a) << ", Expected: " << (b) << ")" << std::endl; \
        return false; \
    }

#define ASSERT_THROWS(expression, exception_type) \
    try { \
        expression; \
        std::cerr << "  -> ASSERT FAILED: Expected exception " << #exception_type << " was not thrown." << std::endl; \
        return false; \
    } catch (const exception_type&) { \
    } catch (...) { \
        std::cerr << "  -> ASSERT FAILED: Wrong exception type thrown." << std::endl; \
        return false; \
    }

void run_test(const std::function<bool()>& test_function, const std::string& test_name) {
    std::cout << "[TEST] " << test_name << "..." << std::endl;
    if (test_function()) {
        std::cout << "  -> PASSED" << std::endl;
        tests_passed++;
    } else {
        std::cout << "  -> FAILED" << std::endl;
        tests_failed++;
    }
}

void test_all_structures() {
    std::cout << "==========================================" << std::endl;
    std::cout << "        INICIANDO TESTES DE CORRECAO" << std::endl;
    std::cout << "==========================================" << std::endl;

    // Testes AVL
    run_test([](){ AVL<int,int> avl; avl.add(1,1); return avl.size() == 1; }, "AVL add");
    run_test([](){ AVL<int,int> avl; avl.add(1,1); return avl.get(1) == 1; }, "AVL get");
    run_test([](){ AVL<int,int> avl; avl.add(1,1); avl.remove(1); return avl.size() == 0; }, "AVL Remove");
    run_test([](){ AVL<int,int> avl; avl.add(1,1); avl.add(2,2); avl.add(3,3); return avl.size() == 3; }, "AVL Multiple adds");
    run_test([](){ AVL<std::string, std::string> avl; avl.add("key1", "value1"); return avl.get("key1") == "value1"; }, "AVL String add"); 
    run_test([](){ AVL<std::string, std::string> avl; avl.add("key1", "value1"); avl.add("key2", "value2"); avl.add("key3", "value3"); return avl.size() == 3; }, "AVL String Multiple adds");
    run_test([](){ AVL<std::string, std::string> avl; avl.add("key1", "value1"); avl.add("key2", "value2"); avl.remove("key1"); ASSERT_THROWS(avl.get("key1"), std::runtime_error); return avl.get("key2") == "value2"; }, "AVL String Remove");

    // Testes Rubro-Negra
    run_test([](){ RB<int,int> rb; rb.add(1,1); return rb.size() == 1; }, "RB add");
    run_test([](){ RB<int,int> rb; rb.add(1,1); return rb.get(1) == 1; }, "RB get");
    run_test([](){ RB<int,int> rb; rb.add(1,1); rb.remove(1); return rb.size() == 0; }, "RB Remove");
    run_test([](){ RB<int,int> rb; rb.add(1,1); rb.add(2,2); rb.add(3,3); return rb.size() == 3; }, "RB Multiple adds");
    run_test([](){ RB<std::string, std::string> rb; rb.add("key1", "value1"); return rb.get("key1") == "value1"; }, "RB String add");
    run_test([](){ RB<std::string, std::string> rb; rb.add("key1", "value1"); rb.add("key2", "value2"); rb.add("key3", "value3"); return rb.size() == 3; }, "RB String Multiple adds");
    run_test([](){ RB<std::string, std::string> rb; rb.add("key1", "value1"); rb.add("key2", "value2"); rb.remove("key1"); ASSERT_THROWS(rb.get("key1"), std::runtime_error); return rb.get("key2") == "value2"; }, "RB String Remove");

    // Testes Hash Encadeada
    run_test([](){ ChainedHashTable<int,int> ht; ht.add(1,1); return ht.size() == 1; }, "Chained Hash Insert");
    run_test([](){ ChainedHashTable<int,int> ht; ht.add(1,1); return ht.get(1) == 1; }, "Chained Hash Search");
    run_test([](){ ChainedHashTable<int,int> ht; ht.add(1,1); ht.remove(1); return ht.size() == 0; }, "Chained Hash Remove");       
    run_test([](){ ChainedHashTable<int,int> ht; ht.add(1,1); ht.add(2,2); ht.add(3,3); return ht.size() == 3; }, "Chained Hash Multiple Inserts");
    run_test([](){ ChainedHashTable<std::string, int> ht; ht.add("key1", 1); return ht.get("key1") == 1; }, "Chained Hash String Insert");
    run_test([](){ ChainedHashTable<std::string, int> ht; ht.add("key1", 1); ht.add("key2", 2); ht.add("key3", 3); return ht.size() == 3; }, "Chained Hash String Multiple Inserts");
    run_test([](){ ChainedHashTable<std::string, int> ht; ht.add("key1", 1); ht.add("key2", 2); ht.remove("key1"); ASSERT_THROWS(ht.get("key1"), std::out_of_range); return ht.get("key2") == 2; }, "Chained Hash String Remove"); 
    
    // Testes Hash Endereçamento Aberto
    run_test([](){ OpenAddressingHashTable<int,int> oht(10); oht.add(1,1); return oht.size() == 1; }, "Open Addressing Hash Insert");
    run_test([](){ OpenAddressingHashTable<int,int> oht(10); oht.add(1,1); return oht.get(1) == 1; }, "Open Addressing Hash Search");
    run_test([](){ OpenAddressingHashTable<int,int> oht(10); oht.add(1,1); oht.remove(1); return oht.size() == 0; }, "Open Addressing Hash Remove");
    run_test([](){ OpenAddressingHashTable<int,int> oht(10); oht.add(1,1); oht.add(2,2); oht.add(3,3); return oht.size() == 3; }, "Open Addressing Hash Multiple Inserts");
    run_test([](){ OpenAddressingHashTable<std::string, int> oht(10); oht.add("key1", 1); return oht.get("key1") == 1; }, "Open Addressing Hash String Insert");
    run_test([](){ OpenAddressingHashTable<std::string, int> oht(10); oht.add("key1", 1); oht.add("key2", 2); oht.add("key3", 3); return oht.size() == 3; }, "Open Addressing Hash String Multiple Inserts");
    run_test([](){ OpenAddressingHashTable<std::string, int> oht(10); oht.add("key1", 1); oht.add("key2", 2); oht.remove("key1"); ASSERT_THROWS(oht.get("key1"), std::out_of_range); return oht.get("key2") == 2; }, "Open Addressing Hash String Remove");

}

//==================================================================
// ESTRUTURA DE BENCHMARK
//==================================================================
struct BenchmarkResults {
    long long insert_time_ms = 0;
    long long search_time_ms = 0;
    long long insert_comparisons = 0;
    long long search_comparisons = 0;
    long long specifc_metrics = 0;
    long long colors = 0;
};

BenchmarkResults benchmark_avl(const std::vector<std::string>& data) {
    BenchmarkResults results;
    AVL<std::string, int> avl;
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { avl.add(val, 1); }
    auto end_insert = std::chrono::high_resolution_clock::now();
    
    results.insert_time_ms = std::chrono::duration<double, std::milli>(end_insert - start_insert).count();
    results.insert_comparisons = avl.get_comparisons();
    results.specifc_metrics = avl.get_rotations();
    long long comparisons_before_search = avl.get_comparisons();
    
    auto start_search = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { avl.get(val); }
    auto end_search = std::chrono::high_resolution_clock::now();
    
    results.search_time_ms = std::chrono::duration<double, std::milli>(end_search - start_search).count();
    results.search_comparisons = avl.get_comparisons() - comparisons_before_search;
    
    return results;
}

BenchmarkResults benchmark_rb(const std::vector<std::string>& data) {
    BenchmarkResults results;
    RB<std::string, int> rb;
    
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { rb.add(val, 1); }
    auto end_insert = std::chrono::high_resolution_clock::now();
    
    results.insert_time_ms = std::chrono::duration<double, std::milli>(end_insert - start_insert).count();
    results.insert_comparisons = rb.get_comparisons();
    results.specifc_metrics = rb.get_rotations();
    results.colors = rb.get_colors();
    long long comparisons_before_search = rb.get_comparisons();
    
    auto start_search = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { rb.get(val); }
    auto end_search = std::chrono::high_resolution_clock::now();
    
    results.search_time_ms = std::chrono::duration<double, std::milli>(end_search - start_search).count();
    results.search_comparisons = rb.get_comparisons() - comparisons_before_search;
    
    return results;
}

BenchmarkResults benchmark_chained_hash(const std::vector<std::string>& data) {
    BenchmarkResults results;
    ChainedHashTable<std::string, int> ht(data.size() / 2);
    
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { ht.add(val, 1); }
    auto end_insert = std::chrono::high_resolution_clock::now();
    
    results.insert_time_ms = std::chrono::duration<double, std::milli>(end_insert - start_insert).count();
    results.insert_comparisons = ht.get_comparisons();
    results.specifc_metrics = ht.get_collisions();
    long long comparisons_before_search = ht.get_comparisons();
    
    auto start_search = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { ht.get(val); }
    auto end_search = std::chrono::high_resolution_clock::now();
    
    results.search_time_ms = std::chrono::duration<double, std::milli>(end_search - start_search).count();
    results.search_comparisons = ht.get_comparisons() - comparisons_before_search;
    
    return results;
}

BenchmarkResults benchmark_open_hash(const std::vector<std::string>& data) {
    BenchmarkResults results;
    OpenAddressingHashTable<std::string, int> oht(data.size() * 2);
    
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { oht.add(val, 1); }
    auto end_insert = std::chrono::high_resolution_clock::now();
    
    results.insert_time_ms = std::chrono::duration<double, std::milli>(end_insert - start_insert).count();
    results.insert_comparisons = oht.get_comparisons();
    results.specifc_metrics = oht.get_collisions();
    
    long long comparisons_before_search = oht.get_comparisons();
    auto start_search = std::chrono::high_resolution_clock::now();
    for (const auto& val : data) { oht.get(val); }
    auto end_search = std::chrono::high_resolution_clock::now();
    
    results.search_time_ms = std::chrono::duration<double, std::milli>(end_search - start_search).count();
    results.search_comparisons = oht.get_comparisons() - comparisons_before_search;
    
    return results;
}

// --- Função para Imprimir a Tabela de Resultados ---
void print_results_table(const std::map<std::string, BenchmarkResults>& all_results) {
    std::cout << "\n===================================================================================================================\n";
    std::cout << "                                     TABELA COMPARATIVA DE DESEMPENHO (MÉDIA DE 5 EXECUÇÕES)\n";
    std::cout << "===================================================================================================================\n";
    std::cout << std::left << std::setw(25) << "Estrutura"
              << std::setw(25) << "Tempo Insercao (ms)"
              << std::setw(25) << "Tempo Busca (ms)"
              << std::setw(25) << "Comparacoes (Insercao)"
              << std::setw(25) << "Comparacoes (Busca)"
              << std::setw(20) << "Rotacoes/Colisoes"
              << std::setw(20) << "Trocas de Cor" << std::endl;
    std::cout << std::string(145, '-') << std::endl;

    // Usando fixed e setprecision para formatar os doubles
    std::cout << std::fixed << std::setprecision(2);

    for (const auto& pair : all_results) {
        std::cout << std::left << std::setw(25) << pair.first
                  << std::setw(25) << pair.second.insert_time_ms
                  << std::setw(25) << pair.second.search_time_ms
                  << std::setw(25) << static_cast<long long>(pair.second.insert_comparisons)
                  << std::setw(25) << static_cast<long long>(pair.second.search_comparisons)
                  << std::setw(20) << static_cast<long long>(pair.second.specifc_metrics) 
                  << std::setw(20) << pair.second.colors <<std::endl;
    }
    std::cout << "===================================================================================================================\n";
}

// --- Função para gerar strings aleatórias ---
std::string generate_random_string(size_t length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(0, max_index - 1);
    std::string str(length, 0);
    std::generate_n(str.begin(), length, [&]() { return charset[distrib(gen)]; });
    return str;
}


int main() {
    // 1. Executa primeiro os testes de correção para garantir que tudo está funcional
    test_all_structures();
    if (tests_failed > 0) {
        std::cerr << "\nERRO: " << tests_failed << " testes de correcao falharam. Abortando benchmark." << std::endl;
        return 1;
    }
    std::cout << "\nTodos os testes de correcao passaram. Iniciando benchmark...\n" << std::endl;
    
    // 2. Preparação do Benchmark com STRINGS 
    const int NUM_ELEMENTS = 50000;
    const int STRING_LENGTH = 10;
    const int NUM_RUNS = 5;
    std::vector<std::string> benchmark_data;
    benchmark_data.reserve(NUM_ELEMENTS);
    std::cout << "Gerando " << NUM_ELEMENTS << " strings aleatorias para o benchmark..." << std::endl;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        benchmark_data.push_back(generate_random_string(STRING_LENGTH));
    }
    
    // 3. Armazenamento dos Resultados de Todas as Execuções
    std::map<std::string, std::vector<BenchmarkResults>> all_runs_results;

    for (int i = 0; i < NUM_RUNS; ++i) {
        std::cout << "\n--- Iniciando Execucao " << i + 1 << "/" << NUM_RUNS << " ---" << std::endl;
        
        std::cout << "Executando benchmark para AVL Tree..." << std::endl;
        all_runs_results["AVL Tree"].push_back(benchmark_avl(benchmark_data));

        std::cout << "Executando benchmark para Red-Black Tree..." << std::endl;
        all_runs_results["Red-Black Tree"].push_back(benchmark_rb(benchmark_data));

        std::cout << "Executando benchmark para Chained Hash Table..." << std::endl;
        all_runs_results["Chained Hash Table"].push_back(benchmark_chained_hash(benchmark_data));

        std::cout << "Executando benchmark para Open Addressing Hash..." << std::endl;
        all_runs_results["Open Addressing Hash"].push_back(benchmark_open_hash(benchmark_data));
    }

    // 4. Cálculo da Média dos Resultados
    std::map<std::string, BenchmarkResults> final_averaged_results;
    for (auto const& [name, results_vector] : all_runs_results) {
        BenchmarkResults sum_result;
        for (const auto& res : results_vector) {
            sum_result.insert_time_ms += res.insert_time_ms;
            sum_result.search_time_ms += res.search_time_ms;
            sum_result.insert_comparisons += res.insert_comparisons;
            sum_result.search_comparisons += res.search_comparisons;
            sum_result.specifc_metrics += res.specifc_metrics;
            sum_result.colors += res.colors;
        }
        
        BenchmarkResults avg_result;
        avg_result.insert_time_ms = sum_result.insert_time_ms / NUM_RUNS;
        avg_result.search_time_ms = sum_result.search_time_ms / NUM_RUNS;
        avg_result.insert_comparisons = sum_result.insert_comparisons / NUM_RUNS;
        avg_result.search_comparisons = sum_result.search_comparisons / NUM_RUNS;
        avg_result.specifc_metrics = sum_result.specifc_metrics / NUM_RUNS;
        avg_result.colors = sum_result.colors / NUM_RUNS;
        
        final_averaged_results[name] = avg_result;
    }

    // 5. Exibição dos Resultados Finais (Média)
    print_results_table(final_averaged_results);

    return 0;
}