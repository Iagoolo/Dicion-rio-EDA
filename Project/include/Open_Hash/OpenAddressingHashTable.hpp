#ifndef OPEN_ADDRESSING_HASH_HPP
#define OPEN_ADDRESSING_HASH_HPP

#include <vector>
#include <utility>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "../src/Dictionaty/IDictionary.hpp"
#include "../utils/lexicalStr.hpp"

/**
 * @brief Tabela hash com endereçamento aberto utilizando duplo hash.
 *
 * Esta classe implementa uma tabela hash genérica baseada em endereçamento aberto,
 * utilizando a técnica de duplo hash (double hashing) para resolução de colisões.
 *
 * @tparam Key Tipo da chave.
 * @tparam Value Tipo do valor associado à chave.
 * @tparam Hash Functor de hash a ser utilizado (padrão: std::hash<Key>).
 *
 * Funcionalidades principais:
 * - Inserção, remoção e busca de pares chave-valor.
 * - Redimensionamento automático da tabela conforme o fator de carga.
 * - Métricas de desempenho: número de comparações e colisões.
 *
 * Detalhes de implementação:
 * - Cada slot da tabela pode estar em um dos estados: VAZIO, OCUPADO ou REMOVIDO.
 * - O duplo hash utiliza duas funções de hash para calcular o índice inicial e o passo de sondagem.
 * - O método find_slot localiza o índice apropriado para inserção ou busca.
 * - O método rehash é chamado automaticamente quando o fator de carga máximo é atingido.
 *
 * Métodos públicos:
 * - add(const Key&, const Value&): Insere ou atualiza um elemento.
 * - remove(const Key&): Remove um elemento pela chave.
 * - at(const Key&): Acessa o valor associado à chave (lança exceção se não encontrado).
 * - clear(): Limpa a tabela.
 * - size(), empty(): Consultam o estado da tabela.
 * - get_comparisons(), get_collisions(): Retornam métricas de desempenho.
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class OpenAddressingHashTable : public IDictionary<Key, Value>
{
private:
    // Enum para o estado de cada slot
    enum class SlotStatus
    {
        EMPTY,
        OCCUPIED,
        DELETED
    };

    // Struct para cada slot da tabela
    struct HashSlot
    {
        std::pair<Key, Value> data;
        SlotStatus status = SlotStatus::EMPTY;
    };

    // Membros da classe
    size_t m_table_size;
    size_t m_number_of_elements;
    float m_max_load_factor;
    std::vector<HashSlot> m_table;
    Hash m_hashing;

    // Métricas de desempenho
    mutable long long comparisons = 0;
    mutable long long collisions = 0;

    // Constante para o passo de sondagem no duplo hash
    // Usamos um número primo para reduzir colisões
    // HASH_PRIME é um número primo menor que o tamanho da tabela
    // Isso ajuda a garantir uma distribuição mais uniforme dos índices
    // e a evitar padrões de colisão que podem ocorrer com números pares.
    static const size_t HASH_PRIME = 13;

    size_t hash_code(const Key &k) const;
    size_t hash_code2(const Key &k) const;
    size_t find_slot(const Key &k) const;
    void rehash(size_t new_size);

public:

    OpenAddressingHashTable(size_t tableSize = 19, float max_load_factor = 0.75f);

    
    void clear();
    bool contains(const Key &k) const override;
    bool isEmpty() const override;
    void add(const Key &k, const Value &v) override;
    void remove(const Key &k) override;
    size_t size() const override;
    const Value &get(const Key &k) const override;
    std::vector<Key> get_all_keys_sorted() const override;

    // Getters e funções de status
    long long get_comparisons() const override;// Retorna o número de comparações realizadas
    long long get_collisions() const override; // Retorna o número de colisões ocorridas
    long long get_colors() const override; // Função que retorna o número de troca de cores, essa ED não possui
    long long get_rotations() const override; // Função que retorna o número de rotações, essa ED não possui
};

template <typename Key, typename Value, typename Hash>
std::vector<Key> OpenAddressingHashTable<Key, Value, Hash>::get_all_keys_sorted() const {
    std::vector<Key> keys;
    if (this->isEmpty()) return keys;

    keys.reserve(this->size());

    for (const auto& slot : m_table) {
        if (slot.status == SlotStatus::OCCUPIED) {
            keys.push_back(slot.data.first);
        }
    }

    try {
        std::locale loc("pt_BR.UTF-8");
        const auto& collate = std::use_facet<std::collate<char>>(loc);

        std::sort(keys.begin(), keys.end(), [&collate](const lexicalStr& a, const lexicalStr& b) {
            return collate.compare(
                a.get().data(), a.get().data() + a.get().size(),
                b.get().data(), b.get().data() + b.get().size()
            ) < 0;  
        });
    } catch (const std::runtime_error& e) {
        std::cerr << "\nAVISO: Locale 'pt_BR.UTF-8' não encontrado. Ordenação padrão será usada.\n";
        std::sort(keys.begin(), keys.end());
    }   

    return keys;
}

// Função de hash para calcular o índice inicial
// A função de hash é aplicada à chave e o resultado é reduzido pelo tamanho da tabela
// Isso garante que o índice esteja sempre dentro dos limites da tabela
template <typename Key, typename Value, typename Hash>
size_t OpenAddressingHashTable<Key, Value, Hash>::hash_code(const Key &k) const
{
    return m_hashing(k) % m_table_size;
}

// Função de hash para calcular o passo de sondagem
// O passo de sondagem é calculado como o complemento do hash mod HASH_PRIME
// Isso ajuda a distribuir as sondagens de forma mais uniforme, reduzindo colisões
// O uso de um número primo como HASH_PRIME é uma prática comum em tabelas
// hash para melhorar a distribuição dos índices.
template <typename Key, typename Value, typename Hash>
size_t OpenAddressingHashTable<Key, Value, Hash>::hash_code2(const Key &k) const
{
    return HASH_PRIME - (m_hashing(k) % HASH_PRIME);
}

// Função para encontrar o slot correto
template <typename Key, typename Value, typename Hash>
size_t OpenAddressingHashTable<Key, Value, Hash>::find_slot(const Key &k) const
{
    size_t initial_index = hash_code(k);
    size_t index = initial_index;
    size_t step = hash_code2(k);

    for (size_t i = 0; i < m_table_size; ++i)
    {
        comparisons++;
        if (m_table[index].status == SlotStatus::EMPTY)
        {
            return index;
        }
        if (m_table[index].status == SlotStatus::OCCUPIED && m_table[index].data.first == k)
        {
            return index;
        }
        index = (initial_index + (i + 1) * step) % m_table_size;
    }

    return index; // Retorna o último slot verificado
}

/**
 * @brief Redimensiona (rehash) a tabela hash para um novo tamanho.
 *
 * Esta função cria uma nova tabela com o tamanho especificado e reinsere todos
 * os elementos ocupados da tabela antiga na nova. Ela reinicializa o número de
 * elementos e o contador de colisões, utilizando o método add() para reinserir
 * cada elemento, garantindo que a distribuição dos hashes seja atualizada de
 * acordo com o novo tamanho da tabela.
 *
 * @param new_size Novo tamanho desejado para a tabela hash.
 */
template <typename Key, typename Value, typename Hash>
void OpenAddressingHashTable<Key, Value, Hash>::rehash(size_t new_size)
{
    std::vector<HashSlot> old_table = m_table;
    m_table.assign(new_size, HashSlot());
    m_table_size = new_size;
    m_number_of_elements = 0;
    collisions = 0;

    for (const auto &slot : old_table)
    {
        if (slot.status == SlotStatus::OCCUPIED)
        {
            add(slot.data.first, slot.data.second);
        }
    }
}

/**
 * @brief Constrói uma OpenAddressingHashTable com tamanho de tabela e fator de carga máximos especificados.
 *
 * Inicializa a tabela hash com o número de buckets informado e define o fator de carga máximo,
 * que determina quando a tabela deve ser redimensionada para manter operações eficientes.
 *
 * @param tableSize Número inicial de buckets na tabela hash. Padrão: 19.
 * @param max_load_factor Fator de carga máximo permitido (razão entre elementos e buckets) antes do redimensionamento. Padrão: 0.75f.
 */
template <typename Key, typename Value, typename Hash>
OpenAddressingHashTable<Key, Value, Hash>::OpenAddressingHashTable (size_t tableSize, float max_load_factor)
{
    m_number_of_elements = 0;
    m_table_size = tableSize;
    m_max_load_factor = max_load_factor;
    m_table.resize(m_table_size);
}

/**
 * @brief Adiciona um novo par chave-valor à tabela hash com endereçamento aberto.
 *
 * Se a razão entre o número de elementos e o tamanho da tabela atingir ou exceder
 * o fator de carga máximo permitido, a tabela é redimensionada (rehash) para o dobro
 * do tamanho atual antes de inserir o novo elemento.
 *
 * A função calcula o índice inicial usando a função de hash e encontra o slot apropriado
 * para inserção. Se a chave já existir na tabela, seu valor é atualizado.
 * Caso contrário, o novo par chave-valor é inserido no slot encontrado, o status do slot
 * é atualizado para ocupado e o número de elementos é incrementado.
 * O contador de colisões é incrementado se o slot de inserção não for o índice inicial.
 *
 * @param k Chave a ser inserida ou atualizada na tabela.
 * @param v Valor associado à chave.
 */
template <typename Key, typename Value, typename Hash>
void OpenAddressingHashTable<Key, Value, Hash>::add(const Key &k, const Value &v)
{
    if (static_cast<float>(m_number_of_elements + 1) / m_table_size >= m_max_load_factor)
    {
        rehash(2 * m_table_size);
    }

    size_t initial_index = hash_code(k);
    size_t index = find_slot(k);

    if (m_table[index].status == SlotStatus::OCCUPIED)
    {
        m_table[index].data.second = v;
        return;
    }

    if (index != initial_index)
    {
        collisions++;
    }

    m_table[index].status = SlotStatus::OCCUPIED;
    m_table[index].data = std::make_pair(k, v);
    m_number_of_elements++;
}

/**
 * @brief Remove um elemento da tabela hash com endereçamento aberto.
 *
 * Esta função procura pela chave fornecida na tabela hash utilizando a função find_slot.
 * Se a chave for encontrada (ou seja, se o slot correspondente estiver com status OCCUPIED),
 * o status do slot é alterado para DELETED, marcando-o como removido logicamente.
 * Em seguida, o contador de elementos armazenados na tabela (m_number_of_elements) é decrementado.
 *
 * Passos detalhados:
 * 1. Busca o índice do slot correspondente à chave 'k' usando find_slot.
 * 2. Verifica se o slot encontrado está ocupado (OCCUPIED).
 * 3. Se estiver ocupado, altera o status do slot para DELETED.
 * 4. Decrementa o número total de elementos na tabela.
 *
 * @param k Chave do elemento a ser removido da tabela hash.
 */
template <typename Key, typename Value, typename Hash>
void OpenAddressingHashTable<Key, Value, Hash>::remove(const Key &k)
{
    size_t index = find_slot(k);
    if (m_table[index].status == SlotStatus::OCCUPIED)
    {
        m_table[index].status = SlotStatus::DELETED;
        m_number_of_elements--;
    }
}

/**
 * @brief Retorna uma referência constante para o valor associado a uma chave fornecida.
 *
 * Esta função procura a chave especificada na tabela hash utilizando o método find_slot.
 *
 * Passo a passo:
 * 1. Chama a função find_slot passando a chave 'k' para obter o índice onde a chave deveria estar.
 * 2. Verifica se o slot encontrado está ocupado (status == OCCUPIED) e se a chave armazenada no slot é igual à chave buscada.
 * 3. Se qualquer uma dessas condições falhar, lança uma exceção std::out_of_range indicando que a chave não foi encontrada.
 * 4. Caso contrário, retorna uma referência constante para o valor associado à chave encontrada.
 *
 * @param k Chave a ser buscada na tabela hash.
 * @return Referência constante para o valor associado à chave.
 * @throws std::out_of_range Se a chave não for encontrada na tabela.
 */
template <typename Key, typename Value, typename Hash>
const Value& OpenAddressingHashTable<Key, Value, Hash>::get(const Key &k) const
{
    size_t index = find_slot(k);

    if (m_table[index].status != SlotStatus::OCCUPIED || m_table[index].data.first != k)
    {
        throw std::out_of_range("Chave não encontrada");
    }
    return m_table[index].data.second;
}

/**
 * @brief Remove todos os elementos da tabela hash, redefinindo-a para o estado vazio.
 *
 * Esta função zera o número de elementos na tabela e marca todos os slots como EMPTY,
 * limpando efetivamente a tabela hash. Após a chamada desta função, a tabela não conterá
 * nenhum elemento e todos os slots estarão disponíveis para novas inserções.
 */
template <typename Key, typename Value, typename Hash>
void OpenAddressingHashTable<Key, Value, Hash>::clear()
{
    m_number_of_elements = 0;
    for (auto &slot : m_table)
    {
        slot.status = SlotStatus::EMPTY;
    }
    comparisons = 0;
    collisions = 0;
}

/**
 * @brief Verifica se uma chave está presente na tabela hash.
 *
 * Esta função procura pela chave especificada na tabela hash utilizando o método de endereçamento aberto.
 * Retorna verdadeiro se a chave estiver presente e ocupando uma posição válida na tabela, caso contrário retorna falso.
 * Incrementa o contador de comparações ao realizar a busca.
 *
 * @param k A chave a ser buscada na tabela hash.
 * @return true se a chave estiver presente, false caso contrário.
 */
template <typename Key, typename Value, typename Hash>
bool OpenAddressingHashTable<Key, Value, Hash>::contains(const Key &k) const
{
    size_t index = find_slot(k);
    comparisons++;
    return m_table[index].status == SlotStatus::OCCUPIED && m_table[index].data.first == k;
}

// Getters e funções de status
template <typename Key, typename Value, typename Hash>
size_t OpenAddressingHashTable<Key, Value, Hash>::size() const { return m_number_of_elements; }      // Retorna o número de elementos na tabela

template <typename Key, typename Value, typename Hash>
bool OpenAddressingHashTable<Key, Value, Hash>::isEmpty() const { return m_number_of_elements == 0; }  // Verifica se a tabela está vazia

template <typename Key, typename Value, typename Hash>
long long OpenAddressingHashTable<Key, Value, Hash>::get_comparisons() const { return comparisons; } // Retorna o número de comparações realizadas

template <typename Key, typename Value, typename Hash>
long long OpenAddressingHashTable<Key, Value, Hash>::get_collisions() const { return collisions; }   // Retorna o número de colisões ocorridas

template <typename Key, typename Value, typename Hash>
long long OpenAddressingHashTable<Key, Value, Hash>::get_colors() const { return 0; } // Função que retorna o número de troca de cores, essa ED não possui

template <typename Key, typename Value, typename Hash>
long long OpenAddressingHashTable<Key, Value, Hash>::get_rotations() const { return 0; } // Função que retorna o número de rotações, essa ED não possui

#endif