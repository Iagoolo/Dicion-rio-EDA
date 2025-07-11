#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

/**
 * @brief Estrutura de nó para uma árvore AVL.
 * 
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor associado à chave.
 * 
 * Esta estrutura representa um nó de uma árvore AVL, contendo um par chave-valor,
 * altura do nó, e ponteiros para os filhos esquerdo e direito.
 */
template <typename Key, typename Value>
struct Node{
    /**
     * @brief Alias para ponteiro de Node.
     */
    using Nodeptr = Node<Key, Value>*;

    /**
     * @brief Par contendo a chave e o valor armazenados no nó.
     */
    std::pair<Key, Value> data;

    /**
     * @brief Altura do nó na árvore AVL.
     */
    int height;

    /**
     * @brief Ponteiro para o filho esquerdo.
     */
    Nodeptr left;

    /**
     * @brief Ponteiro para o filho direito.
     */
    Nodeptr right;

    /**
     * @brief Construtor do nó AVL.
     * 
     * @param data Par chave-valor a ser armazenado.
     * @param height Altura inicial do nó.
     * @param left Ponteiro para o filho esquerdo (padrão: nullptr).
     * @param right Ponteiro para o filho direito (padrão: nullptr).
     */
    Node(std::pair< Key, Value> data, int height, Nodeptr left = nullptr, Nodeptr right = nullptr){
        this->data = data;
        this->height = height;
        this->left = left;
        this->right = right;
    }
};

#endif