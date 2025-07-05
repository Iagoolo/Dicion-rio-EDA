#ifndef NODERB_HPP
#define NODERB_HPP

#include <utility>

enum Color { RED, BLACK };

/**
 * @brief Estrutura de nó para uma árvore rubro-negra (Red-Black Tree).
 * 
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor associado à chave.
 * 
 * Esta estrutura representa um nó de uma árvore rubro-negra, contendo ponteiros para o pai,
 * filho esquerdo e filho direito, além de informações sobre a cor do nó, altura e os dados
 * armazenados (par chave-valor).
 * 
 * Membros:
 * - data: Par contendo a chave e o valor armazenados no nó.
 * - color: Cor do nó (vermelho ou preto).
 * - parent: Ponteiro para o nó pai.
 * - left: Ponteiro para o filho esquerdo.
 * - right: Ponteiro para o filho direito.
 * - height: Altura do nó na árvore.
 * 
 * Construtores:
 * - RBNode(const std::pair<Key, Value>& data): Inicializa o nó com um par chave-valor.
 * - RBNode(const Key& key, const Value& value): Inicializa o nó com chave e valor separados.
 * - RBNode(): Inicializa o nó com chave e valor padrão.
 */
template <typename Key, typename Value>
struct RBNode {
    using Nodeptr = RBNode<Key, Value>*;

    std::pair<Key, Value> data;
    Color color;
    Nodeptr parent;
    Nodeptr left;
    Nodeptr right;
    int height;

    RBNode(const std::pair<Key, Value>& data)
        : data(data), color(RED), parent(nullptr), left(nullptr), right(nullptr), height(1) {}

    RBNode(const Key& key, const Value& value)
        : data(std::make_pair(key, value)), color(RED), parent(nullptr), left(nullptr), right(nullptr), height(1) {}

    RBNode()
        : data(std::make_pair(Key(), Value())), color(RED), parent(nullptr), left(nullptr), right(nullptr), height(1) {}
};

#endif