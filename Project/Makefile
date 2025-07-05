# Makefile Final com Dependências Corretas

# Compilador
CXX = g++
# Flags de compilação e diretórios de include
CXXFLAGS = -Wall -Wextra -std=c++17 -I./src -I./src/AVL -I./src/RB-TREE -I./src/Chained_Hash -I./src/Open_Hash

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

# --- CORREÇÃO: Lista automática de todos os ficheiros de cabeçalho ---
HEADERS = $(wildcard src/AVL/*.hpp) \
          $(wildcard src/RB-TREE/*.hpp) \
          $(wildcard src/Chained_Hash/*.hpp) \
          $(wildcard src/Open_Hash/*.hpp)


# --- REGRAS ---

# Cria o diretório build se ele não existir
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Regra padrão: compila o programa principal
all: $(BUILD_DIR) $(MAIN_BIN)

# Compila o programa principal (main.cpp)
# CORRIGIDO: Agora depende dos cabeçalhos
$(MAIN_BIN): $(MAIN_SRC) $(HEADERS)
	@echo "Compilando o programa principal (dependências atualizadas)..."
	@$(CXX) $(CXXFLAGS) -o $@ $(MAIN_SRC)

# Roda o programa principal
run: all
	@echo "Executando o programa principal..."
	@./$(MAIN_BIN)

# --- REGRAS DE TESTE ---

# Alvo principal para testes: compila e roda o test_runner
test: $(BUILD_DIR) $(TEST_BIN)
	@echo "Executando os testes..."
	@./$(TEST_BIN)

# Compila o programa de testes (test_runner.cpp)
# CORRIGIDO: Agora depende dos cabeçalhos
$(TEST_BIN): $(TEST_SRC) $(HEADERS)
	@echo "Compilando o runner de testes (dependências atualizadas)..."
	@$(CXX) $(CXXFLAGS) -o $@ $(TEST_SRC)

# --- REGRAS DE LIMPEZA ---

# Limpa todos os arquivos gerados
clean:
	@echo "Limpando o diretório de build..."
	@rm -rf $(BUILD_DIR)

# Declara alvos que não são arquivos
.PHONY: all clean run test