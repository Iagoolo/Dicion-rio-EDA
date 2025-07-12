#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <stack>
#include <algorithm>
#include <vector>
#include "Node.hpp"
#include "../Dictionaty/IDictionary.hpp"

/**
 * @brief Classe que implementa uma Árvore AVL (Adelson-Velsky e Landis).
 * 
 * Esta classe representa uma árvore binária de busca auto-balanceada, onde as operações de inserção,
 * remoção e busca são realizadas em tempo logarítmico no pior caso. O balanceamento é garantido por rotações
 * automáticas após operações que possam desbalancear a árvore.
 * 
 * @tparam Chave Tipo da chave utilizada para ordenação dos nós na árvore.
 * @tparam Valor Tipo do valor associado a cada chave armazenada na árvore.
 * 
 * Principais atributos privados:
 * - Nodeptr root: Ponteiro para o nó raiz da árvore AVL.
 * - mutable long long comparisons: Contador de comparações realizadas durante as operações.
 * - long long rotations: Contador de rotações executadas para manter o balanceamento.
 * - int nodeCount: Contador do número total de nós presentes na árvore.
 * 
 * A classe oferece métodos para inserção, remoção, busca, impressão e obtenção de métricas
 * relacionadas ao desempenho das operações (como número de comparações e rotações).
 */
template <typename Key, typename Value>
class AVL : public IDictionary<Key, Value> {
private:
    using Nodeptr = Node<Key, Value>*;

    Nodeptr root;
    
    int nodeCount = 0; // Contador de nós
    mutable long long comparisons = 0; // Contador de comparações
    long long rotations = 0; // Contador de rotações

    // Funções auxiliares
    Nodeptr minValueNode(Nodeptr node);
    Nodeptr _remove(Nodeptr node, const Key& key);
    Nodeptr _insert(Nodeptr node, const Key& key, const Value& value_to_add);
    Nodeptr leftRotate(Nodeptr node);
    Nodeptr rightRotate(Nodeptr node);
    Nodeptr findNode(Nodeptr node, const Key& key) const;
    int height(Nodeptr node);
    int getBalance(Nodeptr node);
    void destroy(Nodeptr node);

    // Funções de impressão
    void printTree(Nodeptr node, std::string prefix = "", bool isLeft = true) const;
    std::string in_Order() const;
    std::string pre_Ordem() const;
    std::string pos_Ordem() const;

    // Função auxiliar para ordenação
    void in_Order_vec(Nodeptr node, std::vector<Key>& keys) const;
    
public:
    AVL() : root(nullptr), nodeCount(0), comparisons(0), rotations(0) {}
    ~AVL() {
        destroy(root);
    }
    
    void clear();
    void print() const;
    void add(const Key& key, const Value& value_to_add) override;
    void remove(const Key& key) override;
    bool isEmpty() const override;
    bool contains(const Key& key) const override;
    size_t size() const override;
    const Value& get(const Key& key) const override;

    std::vector<Key> get_all_keys_sorted() const override;

    // Funções para obter métricas
    long long get_comparisons() const override;
    long long get_rotations() const override;
    long long get_colors() const override;
    long long get_collisions() const override;
};

//------------- Implementação --------------

template <typename Key, typename Value>
void AVL<Key, Value>::in_Order_vec(Nodeptr node, std::vector<Key>& keys_vec) const{
    if (!node) return;
    
    in_Order_vec(node->left, keys_vec);
    keys_vec.push_back(node->data.first);
    in_Order_vec(node->right, keys_vec);
}

template <typename Key, typename Value>
std::vector<Key> AVL<Key, Value>::get_all_keys_sorted() const {
    std::vector<Key> keys_vec;
    if (this->isEmpty()) {
        return {}; 
    }
    
    keys_vec.reserve(this->size());
    in_Order_vec(root, keys_vec);

    return keys_vec;
}

/**
 * @brief Libera recursivamente toda a memória alocada pelos nós da árvore AVL a partir do nó fornecido.
 *
 * Esta função percorre a subárvore enraizada em 'node' utilizando pós-ordem (primeiro filhos, depois o nó),
 * garantindo que todos os nós filhos sejam deletados antes do próprio nó. É utilizada internamente para
 * destruir a árvore ao chamar o destrutor da classe ou o método clear(), evitando vazamentos de memória.
 *
 * Exemplo de uso:
 * - destroy(root); // Remove todos os nós da árvore a partir da raiz.
 *
 * @param node Ponteiro para o nó raiz da subárvore a ser destruída. Se for nullptr, nada é feito.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::destroy(Nodeptr node){
    if (node){
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

/**
 * @brief Procura recursivamente por um nó com uma chave específica na árvore AVL.
 *
 * Esta função busca um nó cujo valor da chave seja igual ao parâmetro fornecido.
 * Se o nó for encontrado, retorna um ponteiro para ele; caso contrário, retorna nullptr.
 * Durante a busca, o contador de comparações é incrementado para cada comparação realizada.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param node Ponteiro para o nó atual a ser examinado.
 * @param key Chave a ser buscada na árvore.
 * @return Nodeptr Ponteiro para o nó encontrado ou nullptr se não existir.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::findNode(Nodeptr node, const Key& key) const {
    if (!node) {
        return node; // Retorna o nullptr se não encontrado
    }

    comparisons++; // Incrementa o contador de comparações
    if (key < node->data.first){
        return findNode(node->left, key);
    }
    
    comparisons++;
    if (key > node->data.first){
        return findNode(node->right, key);
    }

    return node; // Retorna o nó se a chave for encontrada
}

/**
 * @brief Retorna a altura de um nó na árvore AVL.
 *
 * Esta função recebe um ponteiro para um nó e retorna sua altura.
 * Caso o nó seja nulo, retorna 0, indicando que a altura de um nó inexistente é zero.
 *
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor armazenado no nó.
 * @param node Ponteiro para o nó cuja altura será retornada.
 * @return int Altura do nó, ou 0 se o nó for nulo.
 */
template <typename Key, typename Value>
int AVL<Key, Value>::height(Nodeptr node) {
    return node ? node->height : 0;
}

/**
 * @brief Calcula o fator de balanceamento de um nó na árvore AVL.
 *
 * Esta função retorna a diferença entre as alturas do filho direito e do filho esquerdo
 * do nó fornecido. O fator de balanceamento é utilizado para determinar se a árvore
 * precisa ser reestruturada (rotacionada) para manter suas propriedades de balanceamento.
 *
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor armazenado no nó.
 * @param node Ponteiro para o nó cujo balanceamento será calculado.
 * @return int Fator de balanceamento do nó (altura do filho direito - altura do filho esquerdo).
 *             Retorna 0 se o nó for nulo.
 */
template <typename Key, typename Value>
int AVL<Key, Value>::getBalance(Nodeptr node) {
    return node ? height(node->right) - height(node->left) : 0;
}

/**
 * @brief Realiza uma rotação para a esquerda em um nó da árvore AVL.
 *
 * Esta função executa a rotação à esquerda no nó especificado, ajustando os ponteiros
 * e atualizando as alturas dos nós afetados para manter o balanceamento da árvore AVL.
 * A rotação à esquerda é utilizada quando o filho direito do nó está desbalanceado,
 * promovendo o filho direito para a posição do nó atual.
 *
 * @param node Ponteiro para o nó onde a rotação à esquerda será realizada.
 * @return Nodeptr Novo nó raiz da subárvore após a rotação.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::leftRotate(Nodeptr node) {
    rotations++; // Incrementa o contador de rotações

    Nodeptr u = node->right;
    node->right = u->left;
    u->left = node;

    node->height = 1 + std::max(height(node->left), height(node->right));
    u->height = 1 + std::max(height(u->left), height(u->right));

    return u;
}

/**
 * @brief Realiza uma rotação para a direita em torno do nó fornecido.
 *
 * Esta função executa uma rotação simples à direita em uma subárvore AVL,
 * cujo nó raiz é passado como parâmetro. A rotação para a direita é utilizada
 * para balancear a árvore quando ocorre um desbalanceamento à esquerda.
 * O contador de rotações é incrementado para fins de estatística.
 *
 * @param node Ponteiro para o nó em torno do qual a rotação será realizada.
 * @return Nodeptr Novo nó raiz da subárvore após a rotação.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::rightRotate(Nodeptr node) {
    rotations++; // Incrementa o contador de rotações

    Nodeptr u = node->left;
    node->left = u->right;
    u->right = node;

    node->height = 1 + std::max(height(node->left), height(node->right));
    u->height = 1 + std::max(height(u->left), height(u->right));

    return u;
}

/**
 * @brief Retorna o nó com o menor valor na subárvore a partir do nó fornecido.
 *
 * Esta função percorre a subárvore à esquerda do nó fornecido até encontrar o nó mais à esquerda,
 * que contém o menor valor (chave) naquela subárvore. É útil, por exemplo, para encontrar o sucessor
 * em operações de remoção em árvores AVL.
 *
 * @param node Ponteiro para o nó a partir do qual a busca será realizada.
 * @return Nodeptr Ponteiro para o nó com o menor valor encontrado, ou nullptr se o nó fornecido for nulo.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::minValueNode(Nodeptr node) {
    Nodeptr current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

/**
 * @brief Insere um novo nó na árvore AVL ou atualiza o valor de uma chave existente.
 *
 * Esta função realiza a inserção recursiva de um nó na árvore AVL, mantendo o balanceamento da árvore após a inserção.
 * Se a chave já existir, apenas atualiza o valor associado a ela. O balanceamento é garantido por rotações apropriadas
 * (simples ou duplas) após a inserção. Também incrementa contadores de nós e comparações para fins estatísticos.
 *
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor associado à chave.
 * @param node Ponteiro para o nó atual da árvore (subárvore).
 * @param key Chave a ser inserida ou atualizada.
 * @param value_to_add Valor a ser associado à chave.
 * @return Nodeptr Ponteiro para o nó raiz da subárvore após a inserção e possíveis rotações.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::_insert(Nodeptr node, const Key& key, const Value& value_to_add) {
    if (!node){
        nodeCount++; // Incrementa o contador de nós
        return new Node<Key, Value>(std::make_pair(key, value_to_add), 1);
    }

    if (key < node->data.first){
        comparisons++; // Incrementa o contador de comparações
        node->left = _insert(node->left, key, value_to_add);
    }
    else if (key > node->data.first){
        comparisons+= 2; // Incrementa o contador de comparações
        node->right = _insert(node->right, key, value_to_add);
    }
    else {
        comparisons+= 2; // Incrementa o contador de comparações
        node->data.second = value_to_add; // Atualiza o valor se a chave já existir
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int bal = getBalance(node);

    // Casos de rotação
    if (bal < -1 && getBalance(node->left) <= 0) // Ou apenas < 0 se preferir
    return rightRotate(node);
    // Caso Esquerda-Direita
    if (bal < -1 && getBalance(node->left) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Caso Direita-Direita
    if (bal > 1 && getBalance(node->right) >= 0) // Ou apenas > 0
        return leftRotate(node);
    // Caso Direita-Esquerda
    if (bal > 1 && getBalance(node->right) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/**
 * @brief Remove um nó com a chave especificada da árvore AVL.
 *
 * Esta função realiza a remoção de um nó cuja chave é igual ao parâmetro 'key'.
 * O algoritmo busca recursivamente o nó a ser removido, ajusta os ponteiros dos filhos,
 * atualiza a altura dos nós afetados e garante o balanceamento da árvore após a remoção,
 * realizando rotações se necessário.
 *
 * Também atualiza os contadores de comparações e de nós, utilizados para análise de desempenho.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param node Ponteiro para o nó atual da árvore/subárvore.
 * @param key Chave do nó a ser removido.
 * @return Nodeptr Ponteiro para o nó raiz da subárvore após a remoção e rebalanceamento.
 */
template <typename Key, typename Value>
typename AVL<Key, Value>::Nodeptr AVL<Key, Value>::_remove(Nodeptr node, const Key& key) {
    if (!node) {
        return node;
    }

    if (key < node->data.first){
        comparisons++; // Incrementa o contador de comparações
        node->left = _remove(node->left, key);
    }
    else if (key > node->data.first){
        comparisons+= 2; // Incrementa o contador de comparações
        node->right = _remove(node->right, key);}
    else {
        comparisons+= 2; // Incrementa o contador de comparações
        if (!node->left || !node->right) {
            Nodeptr temp = node->left ? node->left : node->right;
            nodeCount--; // Decrementa o contador de nós
            delete node;
            return temp;
        } else {
            Nodeptr temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = _remove(node->right, temp->data.first);
        }
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int bal = getBalance(node);

    if (bal < -1 && getBalance(node->left) <= 0)
        return rightRotate(node);
    if (bal > 1 && getBalance(node->right) >= 0)
        return leftRotate(node);
    if (bal < -1 && getBalance(node->left) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (bal > 1 && getBalance(node->right) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// ------------------ Funções públicas -----------------

/**
 * @brief Remove todos os nós da árvore AVL, liberando a memória alocada.
 *
 * Esta função limpa completamente a árvore AVL, destruindo todos os seus nós
 * a partir da raiz. Após a execução, o ponteiro para a raiz é definido como nullptr,
 * e os contadores de nós, comparações e rotações são resetados para zero.
 *
 * @note Após chamar esta função, a árvore estará vazia e pronta para reutilização.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::clear(){
    destroy(root);
    root = nullptr;
    nodeCount = 0; // Reseta o contador de nós
    comparisons = 0; // Reseta o contador de comparações
    rotations = 0; // Reseta o contador de rotações
}

/**
 * @brief Insere um novo par chave-valor na árvore AVL.
 *
 * Esta função insere um valor associado a uma chave na árvore AVL.
 * Caso a chave já exista, o valor correspondente pode ser atualizado
 * conforme a implementação do método auxiliar _insert.
 *
 * @param key Chave a ser inserida na árvore.
 * @param value_to_add Valor associado à chave a ser inserido.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::add(const Key& key, const Value& value_to_add){
    root = _insert(root, key, value_to_add);
}

/**
 * @brief Remove um nó da árvore AVL com a chave especificada.
 * Esta função pública remove o nó que contém a chave fornecida da árvore AVL.
 * Internamente, ela chama a função privada _remove, que realiza a remoção e
 * faz os ajustes necessários para manter o balanceamento da árvore.
 *
 * @param key A chave do nó a ser removido.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::remove(const Key& key) {
    root = _remove(root, key);
}

/**
 * @brief Retorna uma representação em string dos elementos da árvore AVL em ordem.
 *
 * Esta função realiza um percurso in-ordem iterativo na árvore AVL, concatenando as chaves
 * dos nós em uma única string, separadas por espaço. O percurso in-ordem garante que as chaves
 * sejam visitadas em ordem crescente.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave nos nós da árvore.
 * @return std::string Uma string contendo as chaves da árvore em ordem, separadas por espaço.
 */
template <typename Key, typename Value>
std::string AVL<Key, Value>::in_Order() const {
    std::stack<Nodeptr> s;
    Nodeptr curr = root;
    std::string res;

    while (curr || !s.empty()) {
        while (curr) {
            s.push(curr);
            curr = curr->left;
        }
        curr = s.top(); s.pop();
        res += (curr->data.first) + " ";
        curr = curr->right;
    }
    return res;
}

/**
 * @brief Retorna uma representação em string da árvore AVL percorrida em pré-ordem.
 *
 * Este método realiza um percurso em pré-ordem iterativo na árvore AVL,
 * começando pela raiz e visitando cada nó na ordem: raiz, subárvore esquerda, subárvore direita.
 * Os valores das chaves dos nós visitados são concatenados em uma string, separados por espaço.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave nos nós da árvore.
 * @return Uma string contendo as chaves dos nós em ordem de percurso pré-ordem,
 *         separadas por espaço. Retorna uma string vazia se a árvore estiver vazia.
 */
template <typename Key, typename Value>
std::string AVL<Key, Value>::pre_Ordem() const {
    if (!root) return "";
    std::stack<Nodeptr> s;
    s.push(root);
    std::string res;

    while (!s.empty()) {
        Nodeptr curr = s.top(); s.pop();
        res += (curr->data.first) + " ";
        if (curr->right) s.push(curr->right);
        if (curr->left) s.push(curr->left);
    }
    return res;
}

/**
 * @brief Retorna uma string representando a travessia pós-ordem da árvore AVL.
 *
 * Esta função percorre a árvore AVL em pós-ordem (esquerda, direita, raiz) e
 * concatena as chaves dos nós em uma única string, separadas por espaço.
 * Caso a árvore esteja vazia, retorna uma string vazia.
 *
 * @return std::string String contendo as chaves dos nós em ordem pós-ordem, separadas por espaço.
 */
template <typename Key, typename Value>
std::string AVL<Key, Value>::pos_Ordem() const {
    if (!root) return "";
    std::stack<Nodeptr> s1, s2;
    s1.push(root);
    std::string res;

    while (!s1.empty()) {
        Nodeptr curr = s1.top(); s1.pop();
        s2.push(curr);
        if (curr->left) s1.push(curr->left);
        if (curr->right) s1.push(curr->right);
    }

    while (!s2.empty()) {
        res += (s2.top()->data.first) + " ";
        s2.pop();
    }
    return res;
}

/**
 * @brief Imprime a estrutura da árvore AVL no console de forma hierárquica.
 *
 * Esta função percorre recursivamente a árvore AVL a partir do nó fornecido,
 * exibindo cada chave e valor no formato "chave:valor", com indentação visual
 * que representa a estrutura da árvore. Os ramos esquerdos e direitos são
 * diferenciados por símbolos ("├──" para a esquerda e "└──" para a direita),
 * facilitando a visualização da hierarquia.
 *
 * @tparam Key Tipo da chave armazenada nos nós da árvore.
 * @tparam Value Tipo do valor associado à chave.
 * @param node Ponteiro para o nó atual a ser impresso.
 * @param prefix String utilizada para formatar a indentação dos nós.
 * @param isLeft Indica se o nó atual é filho à esquerda do seu pai.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::printTree(Nodeptr node, std::string prefix, bool isLeft) const {
    if (!node) return;
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──");
    std::cout << node->data.first << ":" << node->data.second << std::endl;
    printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
}

/**
 * @brief Procura o valor associado a uma chave na árvore AVL.
 *
 * Esta função busca um nó na árvore AVL que contenha a chave especificada.
 * Se a chave for encontrada, retorna o valor associado a ela.
 * Caso contrário, lança uma exceção std::runtime_error indicando que a chave não foi encontrada.
 *
 * @param key A chave a ser procurada na árvore.
 * @return O valor associado à chave fornecida.
 * @throws std::runtime_error Se a chave não for encontrada na árvore.
 */
template <typename Key, typename Value>
const Value& AVL<Key, Value>::get(const Key& key) const {
    Nodeptr node = findNode(root, key);

    if (node) {
        return node->data.second; // Retorna o valor associado à chave
    } else {
        throw std::runtime_error("Chave não encontrada");
    }
}

/**
 * @brief Verifica se uma chave está presente na árvore AVL.
 *
 * Esta função procura pela chave especificada na árvore AVL e retorna true se a chave for encontrada,
 * ou false caso contrário.
 *
 * @param key A chave a ser buscada na árvore.
 * @return true se a chave estiver presente na árvore, false caso contrário.
 */
template <typename Key, typename Value>
bool AVL<Key, Value>::contains(const Key& key) const {
    Nodeptr node = findNode(root, key);
    return node != nullptr; // Retorna true se o nó for encontrado, false caso contrário
}

/**
 * @brief Imprime as diferentes travessias da árvore AVL e sua estrutura.
 *
 * Esta função exibe no console as representações das travessias in-order, pré-ordem e pós-ordem da árvore AVL,
 * utilizando os métodos auxiliares in_Order(), pre_Ordem() e pos_Ordem(). Além disso, imprime a estrutura visual
 * da árvore a partir do nó raiz utilizando a função printTree().
 *
 * @note Esta função é destinada principalmente para fins de depuração e visualização da árvore.
 */
template <typename Key, typename Value>
void AVL<Key, Value>::print() const {
    std::cout << "In-order: " << in_Order() << std::endl;
    std::cout << "Pré-ordem: " << pre_Ordem() << std::endl;
    std::cout << "Pós-ordem: " << pos_Ordem() << std::endl;
    printTree(root, "", false);
}

/**
 * @brief Retorna o número de nós presentes na árvore AVL.
 *
 * Este método fornece a quantidade total de elementos armazenados na árvore,
 * permitindo ao usuário saber o tamanho atual da estrutura.
 *
 * @return int Número de nós na árvore.
 */
template <typename Key, typename Value>
size_t AVL<Key, Value>::size() const  {
    return nodeCount; // Retorna o número de nós na árvore
}

/**
 * @brief Verifica se a árvore AVL está vazia.
 *
 * Este método retorna verdadeiro se a árvore não contém nenhum nó,
 * ou seja, se o número de nós (nodeCount) é igual a zero.
 *
 * @return true se a árvore estiver vazia, false caso contrário.
 */
template <typename Key, typename Value>
bool AVL<Key, Value>::isEmpty() const {
    return nodeCount == 0;
}

/**
 * @brief Retorna o número de comparações realizadas nas operações da árvore AVL.
 *
 * Este método fornece acesso ao contador interno de comparações, útil para análise de desempenho
 * dos algoritmos de busca, inserção e remoção na árvore AVL.
 *
 * @return O número total de comparações realizadas como um valor do tipo long long.
 */
template <typename Key, typename Value>
long long AVL<Key, Value>::get_comparisons() const {
    return comparisons; // Retorna o número de comparações realizadas
}

/**
 * @brief Retorna o número de rotações realizadas pela árvore AVL.
 *
 * Esta função fornece uma métrica específica da árvore AVL, indicando
 * quantas rotações (simples ou duplas) foram executadas durante as operações
 * de inserção ou remoção para manter o balanceamento da árvore.
 *
 * @return long long O número total de rotações realizadas.
 */
template <typename Key, typename Value>
long long AVL<Key, Value>::get_rotations() const {
    return rotations; // Retorna o número de rotações realizadas
}

template <typename Key, typename Value>
long long AVL<Key, Value>::get_colors() const {
    return 0; // Retorna 0, pois AVL não utiliza cores como RB-Tree
}

template <typename Key, typename Value>
long long AVL<Key, Value>::get_collisions() const {
    return 0; // Retorna 0, pois AVL não utiliza colisões como RB-Tree
}

#endif