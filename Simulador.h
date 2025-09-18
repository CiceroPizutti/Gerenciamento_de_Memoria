#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include "MemoriaContigua.h"
#include "Paginacao.h"

using namespace std;

class Simulador {
private:
    MemoriaContigua* memoria_contigua;
    Paginacao* paginacao;
    int modo_atual;  // 1 = Contígua, 2 = Paginação
    
public:
    Simulador();
    ~Simulador();
    
    void executar();
    
private:
    void mostrarMenuPrincipal();
    void configurarMemoriaContigua();
    void menuMemoriaContigua();
    void configurarPaginacao();
    void menuPaginacao();
    
    AlgoritmoAlocacao escolherAlgoritmo();
    void limparTela();
    void pausar();
};

#endif


