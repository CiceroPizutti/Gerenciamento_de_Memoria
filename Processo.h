#ifndef PROCESSO_H
#define PROCESSO_H

#include <string>
#include <vector>
#include <map>

class Processo {
private:
    int id;
    int tamanho;
    int endereco_base;  // Para alocação contígua
    bool ativo;
    std::map<int, int> tabela_de_paginas; // Para paginação <pagina, frame>
    
public:
    // Construtores
    Processo();
    Processo(int id, int tamanho);
    
    // Getters
    int getId() const { return id; }
    int getTamanho() const { return tamanho; }
    int getEnderecoBase() const { return endereco_base; }
    bool isAtivo() const { return ativo; }
    const std::map<int, int>& getTabelaDePaginas() const { return tabela_de_paginas; }
    
    // Setters
    void setEnderecoBase(int endereco) { endereco_base = endereco; }
    void setAtivo(bool status) { ativo = status; }
    void setTabelaDePaginas(const std::map<int, int>& tabela) { tabela_de_paginas = tabela; }
    
    // Métodos utilitários
    std::string toString() const;
    int getEnderecoFinal() const { return endereco_base + tamanho - 1; }
};

#endif
