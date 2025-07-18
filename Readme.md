# Projeto: Análise Comparativa de Estruturas de Dados para Dicionários

Este projeto consiste na implementação e na análise de desempenho de quatro estruturas de dados fundamentais, aplicadas ao problema de um dicionário. O objetivo é comparar empiricamente os trade-offs de cada abordagem através de uma suíte de testes e de uma aplicação final que processa ficheiros de texto.

As estruturas implementadas são:

* Árvore AVL (com ordenação alfabética interna via lexicalStr)

* Árvore Rubro-Negra (com ordenação alfabética interna via lexicalStr)

* Tabela Hash com Encadeamento Separado

* Tabela Hash com Endereçamento Aberto (usando Hash Duplo)

## Requisitos

* Compilador C++ com suporte ao C++17 ou superior (GCC recomendado)
* Pacote de língua pt_BR.UTF-8 instalado no sistema para a correta ordenação alfabética.

## Estrutura dos arquivos

```txt
📁 Organização do Projeto
📦 Projeto_Dicionario_EDA/
├── build/              (Ficheiros compilados)
├── input/             (Ficheiros .txt de entrada)
├── docs/               (Documentação)
├── include/
│   ├── AVL/, RB-TREE/, Chained_Hash/, Open_Hash/.
│   ├── Dictionaty/IDictionary.hpp
│   ├── ReadTxt/TextProcessor.hpp
│   └── utils/lexicalStr.hpp, outputWriter.hpp
├── output/             (Ficheiros de resultado gerados)
├── src/
│   └── main.cpp
├── tests/
│   └── teste_runner.cpp
├── Makefile
└── README.md

```

## Como Compilar e Executar

Este projeto utiliza um Makefile para automatizar a compilação e execução. Os comandos principais são executados a partir do terminal na raiz do projeto.

### Executando a Aplicação Principal

Para compilar e executar o programa de análise de frequência de palavras, utilize o make e depois execute o binário build/main, fornecendo os argumentos necessários.

Sintaxe de Execução:

```bash
./build/main <tipo_estrutura> <caminho_arquivo_entrada> [--out <caminho_arquivo_saida>]

<tipo_estrutura>: avl, rb, chained_hash, ou open_hash.

<caminho_arquivo_entrada>: O caminho para o ficheiro de texto a ser analisado (ex: outupt/teste.txt).
[--out ...] (Opcional): Permite especificar um nome e local para o ficheiro de resultados. Se omitido, um ficheiro padrão será criado na pasta output/.
```

## Compila o programa

```bash
make
```

Aviso: a compilação pode demorar em média 7 segundos.

## Executa com a Árvore AVL

```bash
./build/main avl input/teste_final.txt
```

(Isto irá gerar um ficheiro como output/resultado_avl.txt)

Execução com Saída Personalizada:

## Executa com a Tabela Hash e especifica o nome da saída

```bash
./build/main chained_hash input/biblia.txt --out output/analise_hash_biblia.txt
```

### Observação

Você pode usar todas as estruturas de uma vez para um mesmo arquivo .txt. Para isso, use --all no lugar do <tipo_estrutura>

## Rodando os Testes

Para compilar e executar a suíte de testes de correção e o benchmark de desempenho (que não gera ficheiros, apenas imprime na tela):

```bash
make test
```

## Limpando os Arquivos

Para remover o diretório build e todos os arquivos compilados:

```bash
make clean
```

## Deu erro?

Apague os arquivos que estão na pasta build com o seguinte comando:

```bash
make clean
```
