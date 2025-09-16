CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Processo.cpp $(SRCDIR)/MemoriaContigua.cpp
TARGET = simulador-memoria

# Regra principal
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Regra para compilar e executar
run: $(TARGET)
	./$(TARGET)

# Limpeza
clean:
	rm -f $(TARGET)

# Regras que não geram arquivos
.PHONY: clean run

# Compilação com debug
debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

# Compilação otimizada para release
release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)