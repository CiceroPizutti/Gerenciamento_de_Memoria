#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>

class Processo {
private:
    int id;
    int tamanho;
    int endereco_base;  // Para alocação contígua
    bool ativo;
    
public:
    // Construtores
    Processo();
    Processo(int id, int tamanho);
    
    // Getters
    int getId() const { return id; }
    int getTamanho() const { return tamanho; }
    int getEnderecoBase() const { return endereco_base; }
    bool isAtivo() const { return ativo; }
    
    // Setters
    void setEnderecoBase(int endereco) { endereco_base = endereco; }
    void setAtivo(bool status) { ativo = status; }
    
    // Métodos utilitários
    std::string toString() const;
    int getEnderecoFinal() const { return endereco_base + tamanho - 1; }
};

#endif