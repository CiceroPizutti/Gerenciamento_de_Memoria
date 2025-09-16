#ifndef MEMORIA_CONTIGUA_H
#define MEMORIA_CONTIGUA_H

#include <vector>
#include <map>
#include "Processo.h"

enum AlgoritmoAlocacao {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    CIRCULAR_FIT
};

struct BlocoMemoria {
    int inicio;
    int tamanho;
    bool ocupado;
    int processo_id;  // -1 se livre
    
    BlocoMemoria(int ini, int tam, bool ocup = false, int pid = -1) 
        : inicio(ini), tamanho(tam), ocupado(ocup), processo_id(pid) {}
};

class MemoriaContigua {
private:
    int tamanho_total;
    std::vector<BlocoMemoria> blocos;
    std::map<int, Processo> processos;
    int proximo_id;
    int ponteiro_circular;  // Para Circular-Fit
    
    // Métodos auxiliares
    void consolidarBlocosLivres();
    int encontrarMelhorBloco(int tamanho, AlgoritmoAlocacao algoritmo);
    
public:
    MemoriaContigua(int tamanho);
    
    // Operações principais
    bool alocarProcesso(int tamanho, AlgoritmoAlocacao algoritmo);
    bool removerProcesso(int processo_id);
    
    // Visualização e métricas
    void imprimirMemoria() const;
    void imprimirTabelaProcessos() const;
    double calcularFragmentacaoExterna() const;
    double calcularUtilizacao() const;
    
    // Getters
    int getTamanhoTotal() const { return tamanho_total; }
    int getNumeroProcessos() const { return processos.size(); }
    
    // Limpar memória
    void limpar();
};

#endif