#ifndef IDictionary_hpp
#define IDictionary_hpp

/**
 * @brief Interface genérica para um dicionário associativo.
 * 
 * Esta interface define as operações básicas para um dicionário que associa chaves a valores.
 * 
 * @tparam Key Tipo da chave.
 * @tparam Value Tipo do valor.
 */
template <typename Key, typename Value>
class IDictionary{
public:
    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~IDictionary() = default;

    /**
     * @brief Adiciona um par chave-valor ao dicionário.
     * 
     * @param key Chave a ser adicionada.
     * @param value Valor associado à chave.
     */
    virtual void add(const Key& key, const Value& value) = 0;

    /**
     * @brief Remove um elemento do dicionário pela chave.
     * 
     * @param key Chave do elemento a ser removido.
     */
    virtual void remove(const Key& key) = 0;

    /**
     * @brief Verifica se uma chave está presente no dicionário.
     * 
     * @param key Chave a ser verificada.
     * @return true se a chave está presente, false caso contrário.
     */
    virtual bool contains(const Key& key) const = 0;

    /**
     * @brief Verifica se o dicionário está vazio.
     * 
     * @return true se o dicionário está vazio, false caso contrário.
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Retorna o número de elementos no dicionário.
     * 
     * @return Quantidade de elementos.
     */
    virtual size_t size() const = 0;

    /**
     * @brief Obtém o valor associado a uma chave.
     * 
     * @param key Chave cujo valor será retornado.
     * @return Referência constante ao valor associado.
     * @throws Exceção se a chave não existir.
     */
    virtual const Value& get(const Key& key) const = 0;

    /**
     * @brief Retorna um vetor com todas as chaves ordenadas.
     * 
     * @return Vetor contendo todas as chaves em ordem.
     */
    virtual std::vector<Key> get_all_keys_sorted() const = 0;

    /**
     * @brief Retorna o número de comparações realizadas nas operações do dicionário.
     * 
     * @return Número de comparações.
     */
    virtual long long get_comparisons() const = 0;

    /**
     * @brief Retorna o número de rotações realizadas (útil para árvores balanceadas).
     * 
     * @return Número de rotações.
     */
    virtual long long get_rotations() const = 0;

    /**
     * @brief Retorna o número de operações de coloração realizadas (útil para árvores rubro-negras).
     * 
     * @return Número de operações de coloração.
     */
    virtual long long get_colors() const = 0;

    /**
     * @brief Retorna o número de colisões ocorridas (útil para tabelas hash).
     * 
     * @return Número de colisões.
     */
    virtual long long get_collisions() const = 0;
};
#endif