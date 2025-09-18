CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = main.cpp Simulador.cpp MemoriaContigua.cpp Paginacao.cpp Processo.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = simulador

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) simulador_memoria.zip

run:
	./$(TARGET)


