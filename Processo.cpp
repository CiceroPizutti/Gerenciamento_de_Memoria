#include "Processo.h"
#include <sstream>
#include <iomanip>

// Construtor padrão
Processo::Processo() : id(0), tamanho(0), endereco_base(-1), ativo(false) {
}

// Construtor com parâmetros
Processo::Processo(int id, int tamanho) 
    : id(id), tamanho(tamanho), endereco_base(-1), ativo(false) {
}

// Método para converter processo em string para exibição
std::string Processo::toString() const {
    std::stringstream ss;
    ss << "P" << std::setw(2) << std::setfill('0') << id 
       << " [" << std::setw(4) << tamanho << " KB]";
    
    if (ativo && endereco_base != -1) {
        ss << " @ " << std::setw(4) << endereco_base 
           << "-" << std::setw(4) << getEnderecoFinal();
    } else {
        ss << " [NAO ALOCADO]";
    }
    
    return ss.str();
}