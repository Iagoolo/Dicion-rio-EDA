# 📊 Projeto: Análise Comparativa de Estruturas de Dados para Dicionários

Este projeto consiste na implementação e na análise de desempenho de quatro estruturas de dados fundamentais, aplicadas ao problema de um dicionário. O objetivo é comparar empiricamente os trade-offs de cada abordagem através de uma suíte de testes e benchmarks rigorosos.

---

## As estruturas implementadas são

* Árvore AVL

* Árvore Rubro-Negra

* Tabela Hash com Encadeamento Separado

* Tabela Hash com Endereçamento Aberto (usando Hash Duplo)

---

## ✅ Requisitos

* Compilador C++ com suporte ao C++17 ou superior (GCC recomendado)

* make instalado

---

## 📁 Organização do Projeto

```plaintext
📦 Projeto_Dicionario_EDA/
├── src/
│   ├── AVL/
│   │   └── avl.hpp
│   ├── RB-TREE/
│   │   └── rb_tree.hpp
│   ├── Chained_Hash/
│   │   └── ChainedHashTable.hpp
│   └── Open_Hash/
│       └── OpenAddressingHashTable.hpp
├── tests/
│   └── teste_runner.cpp
├── build/
│   ├── (Executáveis compilados)
├── Makefile
└── README.md
```

---

## ⚙️ Como Compilar e Executar

Este projeto utiliza um Makefile para automatizar a compilação e execução. Os comandos principais são executados a partir do terminal na raiz do projeto.

---

## 🧪 Rodando os Testes e Benchmarks (Comando Principal)

Este é o comando mais importante para a fase de análise do projeto. Ele compila o arquivo teste_runner.cpp e executa tanto os testes de correção quanto o benchmark de desempenho, exibindo a tabela comparativa no final.

```bash
make test
```

---

## ▶️ Rodando o Programa Principal (se aplicável)

Para rodar o programa principal, você pode compilá-lo e executá-lo com os seguintes comandos:

* Compilar:

```bash
make
```

* Executar:

```bash
make run
```

---

## 🧹 Limpando os Arquivos

Para remover o diretório build e todos os arquivos compilados:

```bash
make clean
```

Este comando é útil para forçar uma recompilação completa do zero, o que pode resolver problemas de cache ou de dependências desatualizadas.

## 📌 Observações

* Implementações Genéricas: Todas as estruturas de dados foram implementadas como classes template em C++17, permitindo o uso com diferentes tipos de chave e valor.

* Análise de Desempenho: O foco do projeto é a análise empírica. O teste_runner.cpp coleta métricas detalhadas, incluindo:

* Tempo de execução (usando `chrono`)

* Número de comparações de chave

* Rotações (para árvores AVL e Rubro-Negra)

* Trocas de Cor (para Árvore Rubro-Negra)

* Colisões (para Tabelas Hash)

* Confiabilidade dos Testes: Para garantir resultados robustos, o benchmark executa cada teste 5 vezes e apresenta a média aritmética das métricas coletadas.

### ⚠️ Problemas ao rodar?

Se ocorrer algum erro (como falha de execução, conflitos de compilação ou binário desatualizado), use:

```bash
make clean
make run
```

Isso irá limpar os arquivos gerados e recompilar do zero, o que costuma resolver a maioria dos problemas.

---

### Quer criar seus próprios testes?

Já há um arquivo de testes. No entanto, caso queira criar próprios, você pode fazer isso modficiando tanto a main e depois fazendo um make run como também modificando o arquivo de testes e fazendo um make test no terminal. Lembre-se de, sempre ao fazer uma modificação, executar o comando make clean antes de qualquer outro.
