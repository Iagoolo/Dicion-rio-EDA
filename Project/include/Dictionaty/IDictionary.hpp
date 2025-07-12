#ifndef IDictionary_hpp
#define IDictionary_hpp

template <typename Key, typename Value>
class IDictionary{
public:
    virtual ~IDictionary() = default;

    // Funções de modificação
    virtual void add(const Key& key, const Value& value) = 0;
    virtual void remove(const Key& key) = 0;

    // Funções de consulta
    virtual bool contains(const Key& key) const = 0;
    virtual bool isEmpty() const = 0;
    virtual size_t size() const = 0;
    virtual const Value& get(const Key& key) const = 0;

    // Função de ordenação
    virtual std::vector<Key> get_all_keys_sorted() const = 0;

    // Funções de métricas
    virtual long long get_comparisons() const = 0;
    virtual long long get_rotations() const = 0;
    virtual long long get_colors() const = 0;
    virtual long long get_collisions() const = 0;
};
#endif