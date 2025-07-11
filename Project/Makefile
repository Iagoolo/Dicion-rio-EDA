# Makefile Final (Versão Robusta e Corrigida)

# Compilador
CXX = g++
# Flags de compilação e diretórios de include (simplificado)
CXXFLAGS = -Wall -Wextra -std=c++17 -I./src

# --- VARIÁVEIS DE DIRETÓRIO ---
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# --- VARIÁVEIS DE FICHEIRO ---
MAIN_TARGET = main
MAIN_SRC = $(SRC_DIR)/main.cpp
MAIN_BIN = $(BUILD_DIR)/$(MAIN_TARGET)

TEST_TARGET = teste_runner
TEST_SRC = $(TEST_DIR)/teste_runner.cpp
TEST_BIN = $(BUILD_DIR)/$(TEST_TARGET)

# --- Dependências Automáticas de Cabeçalhos (mais robusto) ---
# Encontra todos os ficheiros .hpp dentro de src e seus subdiretórios
HEADERS = $(shell find $(SRC_DIR) -name '*.hpp')


# --- REGRAS ---

# Regra padrão: compila o programa principal
all: $(MAIN_BIN)

# Compila o programa principal (main.cpp)
# CORRIGIDO: A criação do diretório agora faz parte da receita.
$(MAIN_BIN): $(MAIN_SRC) $(HEADERS)
	@echo "Verificando/Criando diretório de build..."
	@mkdir -p $(@D)
	@echo "Compilando o programa principal..."
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_SRC)

# Roda o programa principal (Nota: precisa de argumentos!)
run: all
	@echo "Executando o programa principal..."
	@echo "Lembre-se de fornecer os argumentos. Exemplo: ./build/main avl corpus/texto.txt"
	@./$(MAIN_BIN)

# --- REGRAS DE TESTE ---

# Alvo principal para testes: compila e roda o test_runner
test: $(TEST_BIN)
	@echo "Executando os testes..."
	@./$(TEST_BIN)

# Compila o programa de testes (test_runner.cpp)
# CORRIGIDO: A criação do diretório agora faz parte da receita.
$(TEST_BIN): $(TEST_SRC) $(HEADERS)
	@echo "Verificando/Criando diretório de build..."
	@mkdir -p $(@D)
	@echo "Compilando o runner de testes..."
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRC)

# --- REGRAS DE LIMPEZA ---

# Limpa todos os arquivos gerados
clean:
	@echo "Limpando o diretório de build..."
	@rm -rf $(BUILD_DIR)

# Declara alvos que não são arquivos
.PHONY: all clean run test
