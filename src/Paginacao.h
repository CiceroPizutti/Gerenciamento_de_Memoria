#ifndef PAGINACAO_H
#define PAGINACAO_H

#include <vector>
#include <map>
#include "Processo.h"

// Estrutura para representar um frame na memória física
struct Frame {
    bool ocupado;
    int processo_id;
    int pagina_id; // Qual página do processo está neste frame

    Frame() : ocupado(false), processo_id(-1), pagina_id(-1) {}
};

class Paginacao {
private:
    int tamanho_memoria;
    int tamanho_pagina;
    int numero_frames;
    std::vector<Frame> frames;
    std::vector<int> frames_livres; // Armazena os índices dos frames livres
    std::map<int, Processo> processos;
    int proximo_id;

    void inicializarFrames();

public:
    Paginacao(int tam_memoria, int tam_pagina);

    // Operações principais
    bool alocarProcesso(int tamanho_processo);
    bool removerProcesso(int processo_id);

    // Visualização e métricas
    void imprimirMemoria() const;
    void imprimirTabelaDePaginas(int processo_id) const;
    void imprimirTabelaProcessos() const;
    double calcularFragmentacaoInterna() const;
    double calcularUtilizacao() const;

    // Limpar memória
    void limpar();
    int getUltimoIdProcesso() const { return proximo_id - 1; }
};

#endif
