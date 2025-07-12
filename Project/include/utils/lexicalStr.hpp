#ifndef LEXICAL_STR_HPP
#define LEXICAL_STR_HPP

#include <string>
#include <locale>
#include <iostream>
#include <stdexcept>
#include <functional>

/**
 * @class lexicalStr
 * @brief Classe para manipulação de strings com comparação lexical baseada em locale.
 *
 * Esta classe encapsula uma std::string e fornece operadores de comparação (<, >, ==, !=)
 * que utilizam o locale definido para realizar comparações lexicais corretas, levando em conta
 * regras de ordenação específicas de idioma/região.
 *
 * Membros:
 * - m_str: Armazena a string interna.
 * - m_locale: Locale estático utilizado para comparação lexical.
 *
 * Construtores:
 * - lexicalStr(): Construtor padrão.
 * - lexicalStr(const std::string& s): Inicializa com uma std::string.
 * - lexicalStr(const char* s): Inicializa com uma string C.
 *
 * Métodos:
 * - get(): Retorna uma referência constante para a string interna.
 * - operator<, operator>: Compara duas lexicalStr usando o locale.
 * - operator==, operator!=: Compara igualdade/desigualdade das strings internas.
 * - operator const std::string&(): Conversão explícita para std::string.
 * - operator<<: Permite saída em streams (ex: std::cout).
 */
class lexicalStr {
private:
    std::string m_str;
    static std::locale m_locale; 

public:
    lexicalStr() = default;
    lexicalStr(const std::string& s) : m_str(s) {}
    lexicalStr(const char* s) : m_str(s) {}

    const std::string& get() const { return m_str; }

    /**
     * @brief Operador de comparação menor (<) para objetos lexicalStr.
     *
     * Compara duas instâncias de lexicalStr utilizando regras de ordenação
     * específicas de localidade (locale), garantindo que a comparação siga
     * as convenções linguísticas apropriadas para o idioma configurado.
     *
     * @param other Outro objeto lexicalStr a ser comparado.
     * @return true se esta instância for considerada menor que 'other'
     *         de acordo com as regras de collation da localidade; caso contrário, false.
     */
    bool operator<(const lexicalStr& other) const {
        const auto& collate = std::use_facet<std::collate<char>>(m_locale);
        return collate.compare(
            m_str.data(), m_str.data() + m_str.size(),
            other.m_str.data(), other.m_str.data() + other.m_str.size()
        ) < 0;
    }

    /**
     * @brief Operador de comparação maior (>) para objetos lexicalStr.
     *
     * Compara esta instância de lexicalStr com outra, utilizando as regras de ordenação
     * específicas da localidade (locale). Retorna true se esta instância for considerada
     * maior que 'other' de acordo com as regras de collation do locale configurado.
     * A comparação é realizada utilizando o facet std::collate para garantir a correta
     * ordenação sensível ao idioma.
     *
     * @param other Outro objeto lexicalStr a ser comparado.
     * @return true se esta instância for maior que 'other' segundo o locale; caso contrário, false.
     */
    bool operator>(const lexicalStr& other) const {
        const auto& coll = std::use_facet<std::collate<char>>(m_locale);
        return coll.compare(m_str.data(), m_str.data() + m_str.size(),
                            other.m_str.data(), other.m_str.data() + other.m_str.size()) > 0;
    }

    /**
     * @brief Compara se duas instâncias de lexicalStr são iguais.
     *
     * Esta função sobrecarrega o operador de igualdade (==) para a classe lexicalStr,
     * permitindo comparar se o conteúdo da string interna (m_str) de dois objetos é igual.
     *
     * @param other Referência constante para o outro objeto lexicalStr a ser comparado.
     * @return true se as strings internas forem iguais, false caso contrário.
     */
    bool operator==(const lexicalStr& other) const { return this->m_str == other.m_str; }

    /**
     * @brief Operador de desigualdade para objetos lexicalStr.
     *
     * Compara o objeto atual com outro objeto lexicalStr para verificar se suas strings internas são diferentes.
     *
     * @param other O outro objeto lexicalStr a ser comparado.
     * @return true se as strings internas forem diferentes, false caso contrário.
     */
    bool operator!=(const lexicalStr& other) const { return this->m_str != other.m_str; }

    /**
     * @brief Converte explicitamente o objeto para uma referência constante de std::string.
     *
     * Permite acessar a string interna encapsulada pela classe, realizando a conversão explícita
     * para uma referência constante de std::string. Útil para obter o valor armazenado sem cópia adicional.
     *
     * @return Referência constante para o std::string interno.
     */
    explicit operator const std::string&() const { return m_str; }

    /**
     * @brief Sobrecarga do operador de inserção (<<) para a classe lexicalStr.
     *
     * Permite que objetos da classe lexicalStr sejam impressos diretamente em fluxos de saída,
     * como std::cout, utilizando o valor retornado pelo método get().
     *
     * @param os Referência para o fluxo de saída (std::ostream) onde o objeto será impresso.
     * @param s Referência constante para o objeto lexicalStr a ser impresso.
     * @return Referência para o fluxo de saída após a inserção do objeto.
     */
    friend std::ostream& operator<<(std::ostream& os, const lexicalStr& s){ return os << s.get(); }
};

/**
 * @brief Inicializa o membro estático m_locale da classe lexicalStr com a localidade "pt_BR.UTF-8".
 *
 * Tenta criar um objeto std::locale utilizando a localidade brasileira UTF-8 ("pt_BR.UTF-8").
 * Caso essa localidade não esteja disponível no sistema, captura a exceção std::runtime_error,
 * exibe um aviso na saída de erro padrão e utiliza a localidade padrão do sistema.
 *
 * @note Esta configuração é importante para garantir o correto tratamento de caracteres especiais
 *       e regras de ordenação específicas do português brasileiro.
 */
inline std::locale lexicalStr::m_locale = [] {
    try {
        return std::locale("pt_BR.UTF-8");
    } catch (const std::runtime_error& e) {
        std::cerr << "\nAVISO: Locale 'pt_BR.UTF-8' nao encontrado." << std::endl;
        return std::locale("");
    }
}();

/**
 * @brief Especialização do functor std::hash para o tipo lexicalStr.
 *
 * Esta especialização permite que objetos do tipo lexicalStr sejam utilizados
 * em containers associativos baseados em hash, como std::unordered_map e std::unordered_set.
 * O valor do hash é calculado a partir da representação em std::string retornada pelo método get().
 *
 * @see lexicalStr
 */
namespace std {
    template <>
    struct hash<lexicalStr> {
        size_t operator()(const lexicalStr& s) const {
            return std::hash<std::string>()(s.get());
        }
    };
}

#endif