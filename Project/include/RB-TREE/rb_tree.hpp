#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <iostream>
#include <utility>
#include <stdexcept>
#include "../src/Dictionaty/IDictionary.hpp"
#include "NodeRb.hpp"

/**
 * @brief Classe template para implementação de uma Árvore Rubro-Negra (Red-Black Tree).
 * 
 * @tparam Key Tipo da chave utilizada para indexação dos nós.
 * @tparam Value Tipo do valor armazenado em cada nó.
 * 
 * Esta classe implementa uma árvore rubro-negra, uma estrutura de dados balanceada
 * que garante operações de inserção, remoção e busca em tempo O(log n).
 * 
 * Principais funcionalidades:
 * - Inserção e remoção de pares chave-valor mantendo as propriedades da árvore rubro-negra.
 * - Busca eficiente por chave.
 * - Impressão da árvore para depuração.
 * - Métricas de desempenho: número de comparações, rotações e alterações de cor.
 * 
 * Métodos públicos:
 * - insert: Insere um novo elemento na árvore.
 * - remove: Remove um elemento da árvore pela chave.
 * - clear: Remove todos os elementos da árvore.
 * - search: Busca um valor associado a uma chave.
 * - size: Retorna o número de nós na árvore.
 * - print: Imprime a árvore (para depuração).
 * - get_comparisons, get_rotations, get_colors: Retornam métricas de desempenho.
 * 
 * Observação: A classe gerencia automaticamente a memória dos nós.
 */
template <typename Key, typename Value>
class RB : public IDictionary<Key, Value> {
private:
    using Nodeptr = RBNode<Key, Value>*;

    Nodeptr root;
    Nodeptr TNULL;

    mutable long long comparisons = 0;
    mutable long long rotations = 0;
    mutable long long colors = 0;
    int nodeCount = 0;

    void initializeTNULL();
    void leftRotate(Nodeptr x);
    void rightRotate(Nodeptr y);
    void insertFix(Nodeptr k);
    void transplant(Nodeptr u, Nodeptr v);
    void deleteFix(Nodeptr x);
    void destroy(Nodeptr node);
    void _insert(const Key& key, const Value& value_to_add);
    void _remove(Nodeptr node);
    Nodeptr minimum(Nodeptr node);
    Nodeptr findNode(const Key& key) const;
    void in_Order_vec(Nodeptr node, std::vector<Key>& vec) const;

public:
    RB() {
        initializeTNULL();
        root = TNULL;
        nodeCount = 0;
        comparisons = 0;
        rotations = 0;
        colors = 0;
    }

    ~RB() {
        destroy(root);
        delete TNULL;
    }

    void clear();
    void print() const; // Função de impressão para depuração
    void add(const Key& key, const Value& value_to_add) override;
    void remove(const Key& key) override;
    bool isEmpty() const override;
    bool contains(const Key& key) const override;
    size_t size() const override;
    const Value& get(const Key& key) const override;
    std::vector<Key> get_all_keys_sorted() const override;

    // Getters para métricas
    long long get_comparisons() const override;
    long long get_rotations() const override;
    long long get_colors() const override;
    long long get_collisions() const override;

private:
    void printTree(Nodeptr node, std::string prefix = "", bool isLeft = true) const;
};

// ---------- IMPLEMENTAÇÃO ----------

/**
 * @brief Realiza um percurso em ordem (in-order) na árvore rubro-negra e armazena as chaves em um vetor.
 *
 * Esta função percorre recursivamente a árvore rubro-negra a partir do nó fornecido, visitando os nós em ordem crescente de chave.
 * Para cada nó visitado, a chave (primeiro elemento do par armazenado no nó) é adicionada ao vetor passado por referência.
 *
 * @param node Ponteiro para o nó atual da árvore a ser visitado.
 * @param keys_vec Referência para o vetor onde as chaves serão armazenadas em ordem.
 */
template <typename Key, typename Value>
void RB<Key, Value>::in_Order_vec(Nodeptr node, std::vector<Key>& keys_vec) const{
    if (node == TNULL) return;
    
    in_Order_vec(node->left, keys_vec);
    keys_vec.push_back(node->data.first);
    in_Order_vec(node->right, keys_vec);
}

/**
 * @brief Retorna um vetor contendo todas as chaves da árvore rubro-negra em ordem crescente.
 *
 * Este método percorre a árvore rubro-negra em ordem (in-order) e armazena todas as chaves
 * em um vetor, garantindo que o resultado esteja ordenado de acordo com o operador de comparação
 * das chaves. Caso a árvore esteja vazia, retorna um vetor vazio.
 *
 * @return std::vector<Key> Vetor com todas as chaves da árvore em ordem crescente.
 */
template <typename Key, typename Value>
std::vector<Key> RB<Key, Value>::get_all_keys_sorted() const {
    std::vector<Key> keys_vec;
    if (this->isEmpty()) {
        return {}; 
    }
    
    keys_vec.reserve(this->size());
    in_Order_vec(root, keys_vec);

    return keys_vec;
}

/**
 * @brief Inicializa o nó sentinela TNULL da Árvore Rubro-Negra.
 *
 * Esta função aloca e configura o nó TNULL, que atua como nó sentinela (folha) da Árvore Rubro-Negra.
 * O nó TNULL recebe a cor PRETA e seus ponteiros left, right e parent são definidos como nullptr.
 * Esse nó é utilizado para representar a ausência de um filho na árvore, simplificando as operações
 * e garantindo que todas as folhas sejam pretas, conforme exigido pelas propriedades da Árvore Rubro-Negra.
 */
template <typename Key, typename Value>
void RB<Key, Value>::initializeTNULL() {
    TNULL = new RBNode<Key, Value>();
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    TNULL->parent = nullptr;
}

/**
 * @brief Desaloca recursivamente todos os nós da subárvore a partir do nó fornecido.
 *
 * Esta função realiza uma travessia pós-ordem para deletar com segurança todos os nós da subárvore,
 * garantindo que os filhos esquerdo e direito sejam deletados antes do próprio nó pai.
 * É normalmente utilizada para liberar a memória ao destruir toda a árvore rubro-negra.
 *
 * @param node Ponteiro para a raiz da subárvore a ser destruída. Se node for TNULL, a função retorna imediatamente.
 */
template <typename Key, typename Value>
void RB<Key, Value>::destroy(Nodeptr node) {
    if (node == TNULL) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

/**
 * @brief Procura um nó na árvore rubro-negra baseado em uma chave fornecida.
 *
 * Esta função percorre a árvore rubro-negra a partir da raiz, comparando a chave fornecida
 * com as chaves dos nós existentes. Se a chave for menor que a do nó atual, a busca segue para a subárvore à esquerda;
 * se for maior, segue para a subárvore à direita. O número de comparações realizadas é contabilizado na variável 'comparisons'.
 * 
 * @param key A chave a ser buscada na árvore.
 * @return Nodeptr Um ponteiro para o nó encontrado com a chave correspondente, ou TNULL caso a chave não exista na árvore.
 */
template <typename Key, typename Value>
typename RB<Key, Value>::Nodeptr RB<Key, Value>::findNode(const Key& key) const {
    Nodeptr current = root;
    while (current != TNULL) {
        if (key < current->data.first) {
            comparisons++;
            current = current->left;
        } else if (key > current->data.first) {
            comparisons += 2;
            current = current->right;
        } else {
            comparisons += 2;
            return current;
        }
    }
    return TNULL;
}

/**
 * @brief Realiza uma rotação para a esquerda em torno do nó x em uma árvore rubro-negra.
 *
 * Esta função ajusta os ponteiros dos nós envolvidos para realizar a rotação à esquerda,
 * mantendo as propriedades da árvore rubro-negra. A rotação para a esquerda move o filho
 * direito de x para a posição de x, e x torna-se o filho esquerdo desse nó.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param x Ponteiro para o nó em torno do qual a rotação será realizada.
 */
template <typename Key, typename Value>
void RB<Key, Value>::leftRotate(Nodeptr x) {
    rotations++;

    Nodeptr y = x->right;
    x->right = y->left;

    if (y->left != TNULL) y->left->parent = x;

    y->parent = x->parent;

    if (y->parent == TNULL) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

/**
 * @brief Realiza uma rotação para a direita no nó fornecido da Árvore Rubro-Negra.
 *
 * Esta função rotaciona a subárvore enraizada no nó 'y' para a direita. O filho esquerdo de 'y'
 * torna-se a nova raiz da subárvore, e 'y' passa a ser o filho direito de seu antigo filho esquerdo.
 * A função também atualiza os ponteiros de pai e mantém as propriedades da Árvore Rubro-Negra.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param y Ponteiro para o nó em torno do qual a rotação à direita será realizada.
 */
template <typename Key, typename Value>
void RB<Key, Value>::rightRotate(Nodeptr y) {
    rotations++;

    Nodeptr x = y->left;
    y->left = x->right;

    if (x->right != TNULL) x->right->parent = y;
    
    
    x->parent = y->parent;
    if (y->parent == TNULL) root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    
    x->right = y;
    y->parent = x;
}

/**
 * @brief Corrige as violações das propriedades da árvore rubro-negra após uma inserção.
 *
 * Esta função é chamada após a inserção de um novo nó na árvore rubro-negra para garantir
 * que todas as propriedades da árvore sejam mantidas. O procedimento percorre a árvore de baixo
 * para cima, realizando rotações e recolorindo nós conforme necessário para restaurar as propriedades
 * rubro-negras. O algoritmo lida com diferentes casos dependendo da posição do nó inserido e das cores
 * dos nós adjacentes.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param k Ponteiro para o nó recém-inserido que pode ter causado violação das propriedades.
 *
 * @note A função utiliza as funções auxiliares `leftRotate` e `rightRotate` para realizar rotações
 */
template <typename Key, typename Value>
void RB<Key, Value>::insertFix(Nodeptr k) {
    Nodeptr u;

    while (k != root && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;

            if (u->color == RED) {
                u->color = BLACK; colors++;
                k->parent->color = BLACK; colors++;
                k->parent->parent->color = RED; colors++;

                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }

                k->parent->color = BLACK; colors++;
                k->parent->parent->color = RED; colors++;
                leftRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;

            if (u->color == RED) {
                u->color = BLACK; colors++;
                k->parent->color = BLACK; colors++;
                k->parent->parent->color = RED; colors++;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
            
                k->parent->color = BLACK; colors++;
                k->parent->parent->color = RED; colors++;
                rightRotate(k->parent->parent);
            }
        }
    }
    if (root->color != BLACK) {
        root->color = BLACK;
        colors++;
    }
}

/**
 * @brief Substitui um subárvore por outra dentro da árvore rubro-negra.
 *
 * Esta função substitui o nó apontado por 'u' pelo nó apontado por 'v' na árvore.
 * É utilizada principalmente em operações de remoção, onde um nó precisa ser removido
 * e substituído por outro, mantendo a estrutura da árvore.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor armazenado nos nós da árvore.
 * @param u Ponteiro para o nó que será substituído.
 * @param v Ponteiro para o nó que irá substituir 'u'.
 *
 * @note Após a chamada, o pai de 'u' passa a apontar para 'v' e o pai de 'v' é atualizado para ser o pai de 'u'.
 *       Se 'u' for a raiz, 'v' se torna a nova raiz.
 */
template <typename Key, typename Value>
void RB<Key, Value>::transplant(Nodeptr u, Nodeptr v) {

    if (u->parent == TNULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/**
 * @brief Corrige as violações das propriedades da árvore rubro-negra após uma remoção.
 *
 * Esta função é chamada após a remoção de um nó preto da árvore rubro-negra, pois tal remoção pode violar
 * as propriedades da árvore. O procedimento percorre a árvore de baixo para cima, realizando rotações e
 * recolorindo nós conforme necessário para restaurar as propriedades rubro-negras. O algoritmo lida com
 * diferentes casos dependendo da posição do nó e das cores dos nós irmãos e seus filhos.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param x Ponteiro para o nó que pode ter causado violação das propriedades após a remoção.
 *
 * @note A função utiliza as funções auxiliares `leftRotate` e `rightRotate` para realizar rotações,
 * e manipula o contador `colors` para registrar o número de alterações de cor realizadas.
 */
template <typename Key, typename Value>
void RB<Key, Value>::deleteFix(Nodeptr x) {
    Nodeptr s;

    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;

            if (s->color == RED) {
                s->color = BLACK; colors++;
                x->parent->color = RED; colors++;

                leftRotate(x->parent);
                
                s = x->parent->right;
            }
            
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED; colors++;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK; colors++;
                    s->color = RED; colors++;
                    rightRotate(s);
                    s = x->parent->right;
                }
                
                s->color = x->parent->color;
                x->parent->color = BLACK; colors++;
                s->right->color = BLACK; colors++;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            
            if (s->color == RED) {
                s->color = BLACK; colors++;
                x->parent->color = RED; colors++;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED; colors++;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK; colors++;
                    s->color = RED; colors++;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK; colors++;
                s->left->color = BLACK; colors++;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    if (x->color != BLACK) {
        x->color = BLACK;
        colors++;
    }
}

/**
 * @brief Encontra o nó com a chave mínima na subárvore enraizada no nó fornecido.
 *
 * Esta função percorre os filhos à esquerda do nó fornecido até alcançar
 * o nó mais à esquerda, que contém a menor chave da subárvore. A busca
 * termina quando um nó cujo filho esquerdo é TNULL é encontrado.
 *
 * @param node Ponteiro para a raiz da subárvore na qual buscar o mínimo.
 * @return Nodeptr Ponteiro para o nó com a chave mínima na subárvore.
 */
template <typename Key, typename Value>
typename RB<Key, Value>::Nodeptr RB<Key, Value>::minimum(Nodeptr node) {

    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

/**
 * @brief Insere um novo nó na árvore rubro-negra com a chave e valor fornecidos.
 *
 * Este método realiza a inserção de um par chave-valor na árvore rubro-negra.
 * Se a chave já existir, o valor associado é atualizado. Caso contrário, um novo nó é criado,
 * inserido na posição correta e as propriedades da árvore rubro-negra são restauradas.
 *
 * @tparam Key Tipo da chave.
 * @tparam Value Tipo do valor.
 * @param key Chave a ser inserida ou atualizada.
 * @param value_to_add Valor a ser associado à chave.
 *
 * @note O método atualiza os contadores de comparações, número de nós e cores conforme necessário.
 * @note Após a inserção, pode chamar a função de ajuste (insertFix) para manter as propriedades da árvore rubro-negra.
 */
template <typename Key, typename Value>
void RB<Key, Value>::_insert(const Key& key, const Value& value_to_add) {
    Nodeptr y = TNULL;
    Nodeptr x = root;

    while (x != TNULL) {
        y = x;
        if (key < x->data.first) {
            comparisons++;
            x = x->left;
        } else if (key > x->data.first) {
            comparisons += 2;
            x = x->right;
        } else {
            comparisons += 2;
            x->data.second = value_to_add;
            return;
        }
    }

    Nodeptr node = new RBNode<Key, Value>(std::make_pair(key, value_to_add));
    node->parent = y;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;
    
    nodeCount++;

    if (y == TNULL) {
        root = node;
    } else if (key < y->data.first) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == TNULL) {
        node->color = BLACK;
        colors++;
        return;
    }

    insertFix(node);
}

/**
 * @brief Remove um nó da árvore rubro-negra.
 *
 * Esta função realiza a remoção de um nó especificado da árvore rubro-negra,
 * mantendo as propriedades da árvore após a remoção. O nó a ser removido é passado
 * como parâmetro. O procedimento lida com três casos principais:
 *   1. O nó possui no máximo um filho não-nulo.
 *   2. O nó possui dois filhos, sendo necessário encontrar o sucessor.
 *   3. Ajusta as ligações e cores para manter as propriedades da árvore rubro-negra.
 *
 * Após a remoção, caso seja necessário, a função chama deleteFix para restaurar
 * as propriedades da árvore rubro-negra.
 *
 * @param node_to_delete Ponteiro para o nó que deve ser removido da árvore.
 */
template <typename Key, typename Value>
void RB<Key, Value>::_remove(Nodeptr node_to_delete) {
    Nodeptr z = node_to_delete;
    Nodeptr x, y;

    y = z;
    Color y_original_color = y->color;
    
    if (z->left == TNULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != TNULL) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;
    nodeCount--;

    if (y_original_color == BLACK) {
        deleteFix(x);
    }
}

/**
 * @brief Insere um novo par chave-valor na árvore rubro-negra.
 *
 * Esta função é a interface pública para inserção de elementos na árvore.
 * Ela recebe uma chave e um valor a serem inseridos e delega a operação
 * para a função privada _insert, que realiza a lógica interna de inserção
 * mantendo as propriedades da árvore rubro-negra.
 *
 * @param key Chave a ser inserida na árvore.
 * @param value_to_add Valor associado à chave a ser inserido.
 */
template <typename Key, typename Value>
void RB<Key, Value>::add(const Key& key, const Value& value_to_add){
    _insert(key, value_to_add);
}

/**
 * @brief Remove um nó da árvore rubro-negra com a chave especificada.
 *
 * Esta função procura um nó na árvore cuja chave corresponde ao parâmetro fornecido.
 * Se o nó for encontrado, a função privada _remove é chamada para realizar a remoção
 * adequada do nó, mantendo as propriedades da árvore rubro-negra. Caso o nó não seja
 * encontrado (ou seja, igual a TNULL), nenhuma ação é realizada.
 *
 * @param key Chave do nó a ser removido da árvore.
 */
template <typename Key, typename Value>
void RB<Key, Value>::remove(const Key& key) {
    Nodeptr node = findNode(key);
    if (node == TNULL) {
        return;
    }
    _remove(node);
}


/**
 * @brief Verifica se uma chave está presente na árvore rubro-negra.
 *
 * Esta função verifica se a chave fornecida existe na árvore rubro-negra.
 * Retorna true se a chave estiver presente, caso contrário retorna false.
 *
 * @param key A chave a ser buscada na árvore.
 * @return true se a chave estiver presente, false caso contrário.
 */
template <typename Key, typename Value>
bool RB<Key, Value>::contains(const Key& key) const {
    return findNode(key) != TNULL;
}

/**
 * @brief Remove todos os nós da Árvore Rubro-Negra, esvaziando seu conteúdo.
 *
 * Esta função desaloca todos os nós da árvore chamando a função auxiliar destroy() a partir da raiz.
 * Após a exclusão de todos os nós, o ponteiro root é redefinido para TNULL e os contadores de nós,
 * comparações, rotações e alterações de cor são zerados.
 * 
 * Após a chamada desta função, a árvore estará vazia e todas as estatísticas associadas serão reiniciadas.
 */
template <typename Key, typename Value>
void RB<Key, Value>::clear() {
    destroy(root);
    root = TNULL;
    nodeCount = 0;
    comparisons = 0;
    rotations = 0;
    colors = 0;
}

/**
 * @brief Imprime a estrutura da Árvore Rubro-Negra de forma visual e formatada.
 *
 * Esta função percorre recursivamente a árvore e imprime cada nó, mostrando sua chave, valor
 * e cor (Vermelho ou Preto), utilizando caracteres ASCII para representar a estrutura da árvore.
 * A saída é indentada para refletir a profundidade e a posição (filho à esquerda ou à direita) de cada nó.
 *
 * @tparam Key   Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor armazenado nos nós da árvore.
 * @param node   Ponteiro para o nó atual a ser impresso.
 * @param prefix String usada para formatar a indentação e os ramos da árvore.
 * @param isLeft Booleano indicando se o nó atual é filho à esquerda.
 *
 * @note Esta função é destinada à depuração e visualização.
 *       Não modifica a árvore.
 */
template <typename Key, typename Value>
void RB<Key, Value>::printTree(Nodeptr node, std::string prefix, bool isLeft) const {
    if (node == TNULL) return;

    std::cout << prefix << (isLeft ? "├──" : "└──") << node->data.first 
              << ":" << node->data.second << " (" << (node->color == RED ? "R" : "B") << ")" << std::endl;
              
    printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
}

/**
 * @brief Imprime o conteúdo da Árvore Rubro-Negra.
 *
 * Esta função inicia a impressão de toda a Árvore Rubro-Negra,
 * chamando a função auxiliar printTree a partir do nó raiz.
 * O formato e o destino da saída dependem da implementação de printTree.
 * Esta função não modifica a árvore.
 */
template <typename Key, typename Value>
void RB<Key, Value>::print() const {
    printTree(root);
}

/**
 * @brief Retorna uma referência ao valor associado a uma chave na árvore rubro-negra.
 *
 * Procura o nó correspondente à chave fornecida. Se a chave for encontrada,
 * retorna uma referência ao valor associado. Caso contrário, lança uma exceção
 * std::runtime_error indicando que a chave não foi encontrada na árvore.
 *
 * @param key A chave a ser buscada na árvore.
 * @return Referência ao valor associado à chave.
 * @throws std::runtime_error Se a chave não for encontrada na árvore.
 */
template <typename Key, typename Value>
const Value& RB<Key, Value>::get(const Key& key) const {
    Nodeptr node = findNode(key);
    if (node == TNULL) {
        throw std::runtime_error("Chave não encontrada na árvore.");
    }
    return node->data.second;
}

/**
 * @brief Verifica se a árvore rubro-negra está vazia.
 *
 * Esta função retorna verdadeiro se a árvore não contém nenhum nó,
 * ou seja, se o número de nós (nodeCount) é igual a zero.
 *
 * @return true se a árvore estiver vazia, false caso contrário.
 */
template <typename Key, typename Value>
bool RB<Key, Value>::isEmpty() const {
    return nodeCount == 0;
}

/**
 * @brief Retorna a quantidade de nós presentes na árvore rubro-negra.
 *
 * @return O número de nós atualmente armazenados na árvore.
 */
template <typename Key, typename Value>
size_t RB<Key, Value>::size() const {
    return nodeCount;
}

/**
 * @brief Retorna o número de comparações de chaves realizadas pela árvore rubro-negra.
 *
 * Esta função fornece acesso ao contador interno que registra quantas comparações de chaves
 * foram realizadas durante as operações na árvore rubro-negra.
 *
 * @return O total de comparações de chaves como um inteiro longo (long long).
 */
template <typename Key, typename Value>
long long RB<Key, Value>::get_comparisons() const {
    return comparisons;
}

/**
 * @brief Retorna o número de rotações realizadas pela árvore rubro-negra.
 *
 * Este método fornece a quantidade total de rotações (simples ou duplas) que foram
 * executadas durante as operações de inserção ou remoção na árvore rubro-negra.
 *
 * @return Número de rotações realizadas.
 */
template <typename Key, typename Value>
long long RB<Key, Value>::get_rotations() const {
    return rotations;
}

/**
 * @brief Retorna o número de cores utilizadas na árvore rubro-negra.
 *
 * Esta função retorna o valor da variável 'colors', que representa a quantidade de cores
 * presentes ou utilizadas na estrutura da árvore rubro-negra. Normalmente, em árvores rubro-negras,
 * as cores são usadas para balanceamento, sendo geralmente duas (vermelho e preto).
 *
 * @return long long O número de cores utilizadas na árvore.
 */
template <typename Key, typename Value>
long long RB<Key, Value>::get_colors() const {
    return colors;
}

template <typename Key, typename Value>
long long RB<Key, Value>::get_collisions() const {
    return 0; // Retorna 0, pois não há colisões em uma árvore rubro-negra
}

#endif