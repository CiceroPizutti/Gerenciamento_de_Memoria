#include "MemoriaContigua.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Construtor
MemoriaContigua::MemoriaContigua(int tamanho) 
    : tamanho_total(tamanho), proximo_id(1), ponteiro_circular(0) {
    // Inicializa com um único bloco livre de todo o tamanho
    blocos.push_back(BlocoMemoria(0, tamanho, false, -1));
}

// Método principal para alocar processo
bool MemoriaContigua::alocarProcesso(int tamanho, AlgoritmoAlocacao algoritmo) {
    int indice_bloco = encontrarMelhorBloco(tamanho, algoritmo);
    
    if (indice_bloco == -1) {
        return false; // Não há bloco suficiente
    }
    
    BlocoMemoria& bloco = blocos[indice_bloco];
    
    // Criar novo processo
    Processo novo_processo(proximo_id, tamanho);
    novo_processo.setEnderecoBase(bloco.inicio);
    novo_processo.setAtivo(true);
    
    // Marcar bloco como ocupado
    bloco.ocupado = true;
    bloco.processo_id = proximo_id;
    
    // Se o bloco é maior que o processo, dividir o bloco
    if (bloco.tamanho > tamanho) {
        int inicio_livre = bloco.inicio + tamanho;
        int tamanho_livre = bloco.tamanho - tamanho;
        
        // Reduzir tamanho do bloco atual
        bloco.tamanho = tamanho;
        
        // Inserir novo bloco livre após o atual
        blocos.insert(blocos.begin() + indice_bloco + 1,
                     BlocoMemoria(inicio_livre, tamanho_livre, false, -1));
    }
    
    // Adicionar processo à lista
    processos[proximo_id] = novo_processo;
    proximo_id++;
    
    return true;
}

// Remover processo da memória
bool MemoriaContigua::removerProcesso(int processo_id) {
    auto it = processos.find(processo_id);
    if (it == processos.end()) {
        return false; // Processo não encontrado
    }
    
    // Encontrar o bloco correspondente
    for (auto& bloco : blocos) {
        if (bloco.processo_id == processo_id) {
            bloco.ocupado = false;
            bloco.processo_id = -1;
            break;
        }
    }
    
    // Remover processo da lista
    processos.erase(it);
    
    // Consolidar blocos livres adjacentes
    consolidarBlocosLivres();
    
    return true;
}

// Encontrar melhor bloco para alocação baseado no algoritmo
int MemoriaContigua::encontrarMelhorBloco(int tamanho, AlgoritmoAlocacao algoritmo) {
    int melhor_indice = -1;
    
    switch (algoritmo) {
        case FIRST_FIT:
            for (size_t i = 0; i < blocos.size(); i++) {
                if (!blocos[i].ocupado && blocos[i].tamanho >= tamanho) {
                    return i;
                }
            }
            break;
            
        case BEST_FIT: {
            int menor_desperdicio = tamanho_total + 1;
            for (size_t i = 0; i < blocos.size(); i++) {
                if (!blocos[i].ocupado && blocos[i].tamanho >= tamanho) {
                    int desperdicio = blocos[i].tamanho - tamanho;
                    if (desperdicio < menor_desperdicio) {
                        menor_desperdicio = desperdicio;
                        melhor_indice = i;
                    }
                }
            }
            break;
        }
        
        case WORST_FIT: {
            int maior_desperdicio = -1;
            for (size_t i = 0; i < blocos.size(); i++) {
                if (!blocos[i].ocupado && blocos[i].tamanho >= tamanho) {
                    int desperdicio = blocos[i].tamanho - tamanho;
                    if (desperdicio > maior_desperdicio) {
                        maior_desperdicio = desperdicio;
                        melhor_indice = i;
                    }
                }
            }
            break;
        }
        
        case CIRCULAR_FIT: {
            size_t inicio = ponteiro_circular;
            
            // Buscar a partir da posição atual do ponteiro
            for (size_t count = 0; count < blocos.size(); count++) {
                size_t i = (inicio + count) % blocos.size();
                if (!blocos[i].ocupado && blocos[i].tamanho >= tamanho) {
                    ponteiro_circular = (i + 1) % blocos.size();
                    return i;
                }
            }
            break;
        }
    }
    
    return melhor_indice;
}

// Consolidar blocos livres adjacentes
void MemoriaContigua::consolidarBlocosLivres() {
    if (blocos.size() <= 1) return;
    
    // Ordenar blocos por posição inicial
    sort(blocos.begin(), blocos.end(), 
         [](const BlocoMemoria& a, const BlocoMemoria& b) {
             return a.inicio < b.inicio;
         });
    
    // Consolidar blocos adjacentes livres
    for (size_t i = 0; i < blocos.size() - 1; ) {
        BlocoMemoria& atual = blocos[i];
        BlocoMemoria& proximo = blocos[i + 1];
        
        // Se ambos são livres e adjacentes
        if (!atual.ocupado && !proximo.ocupado && 
            atual.inicio + atual.tamanho == proximo.inicio) {
            
            // Expandir bloco atual
            atual.tamanho += proximo.tamanho;
            
            // Remover próximo bloco
            blocos.erase(blocos.begin() + i + 1);
        } else {
            i++;
        }
    }
}

// Visualizar estado da memória
void MemoriaContigua::imprimirMemoria() const {
    cout << "\n==== ESTADO DA MEMORIA ====" << endl;
    cout << "Tamanho total: " << tamanho_total << " KB\n" << endl;
    
    // Cabeçalho visual
    cout << "Endereco:  ";
    for (const auto& bloco : blocos) {
        int largura = max(6, bloco.tamanho / 8 + 1);
        cout << setw(largura) << bloco.inicio << "-" << setw(largura-1) << (bloco.inicio + bloco.tamanho - 1);
    }
    cout << "\nMemoria:   ";
    
    // Representação visual da memória
    for (const auto& bloco : blocos) {
        int largura = max(6, bloco.tamanho / 8 + 1);
        
        if (bloco.ocupado) {
            cout << "[P" << setfill('0') << setw(2) << bloco.processo_id << setfill(' ') << "]";
            if (largura > 6) {
                cout << string(largura - 6, ' ');
            }
        } else {
            cout << "[" << string(largura - 2, '-') << "]";
        }
    }
    cout << endl;
    
    // Tabela detalhada
    cout << "\n==== DETALHES DOS BLOCOS ====" << endl;
    cout << setw(8) << "Inicio" << setw(8) << "Fim" << setw(10) << "Tamanho" 
         << setw(8) << "Status" << setw(10) << "Processo" << endl;
    cout << string(50, '-') << endl;
    
    for (const auto& bloco : blocos) {
        cout << setw(8) << bloco.inicio 
             << setw(8) << (bloco.inicio + bloco.tamanho - 1)
             << setw(10) << bloco.tamanho;
        
        if (bloco.ocupado) {
            cout << setw(8) << "OCUPADO" << setw(10) << ("P" + to_string(bloco.processo_id));
        } else {
            cout << setw(8) << "LIVRE" << setw(10) << "---";
        }
        cout << endl;
    }
    cout << endl;
}

// Imprimir tabela de processos
void MemoriaContigua::imprimirTabelaProcessos() const {
    if (processos.empty()) {
        cout << "\nNenhum processo na memoria." << endl;
        return;
    }
    
    cout << "\n==== TABELA DE PROCESSOS ====" << endl;
    cout << setw(5) << "PID" << setw(10) << "Tamanho" << setw(12) << "End. Base" 
         << setw(12) << "End. Limite" << setw(8) << "Status" << endl;
    cout << string(55, '-') << endl;
    
    for (const auto& pair : processos) {
        const Processo& p = pair.second;
        cout << setw(5) << p.getId()
             << setw(10) << p.getTamanho()
             << setw(12) << p.getEnderecoBase()
             << setw(12) << p.getEnderecoFinal()
             << setw(8) << (p.isAtivo() ? "ATIVO" : "INATIVO")
             << endl;
    }
    cout << endl;
}

// Calcular fragmentação externa
double MemoriaContigua::calcularFragmentacaoExterna() const {
    int total_livre = 0;
    int maior_bloco_livre = 0;
    
    for (const auto& bloco : blocos) {
        if (!bloco.ocupado) {
            total_livre += bloco.tamanho;
            maior_bloco_livre = max(maior_bloco_livre, bloco.tamanho);
        }
    }
    
    if (total_livre == 0) {
        return 0.0; // Sem espaço livre
    }
    
    // Fragmentação = (Total Livre - Maior Bloco Livre) / Total * 100
    double fragmentacao = ((double)(total_livre - maior_bloco_livre) / tamanho_total) * 100.0;
    return max(0.0, fragmentacao);
}

// Calcular utilização da memória
double MemoriaContigua::calcularUtilizacao() const {
    int total_usado = 0;
    
    for (const auto& bloco : blocos) {
        if (bloco.ocupado) {
            total_usado += bloco.tamanho;
        }
    }
    
    return ((double)total_usado / tamanho_total) * 100.0;
}

// Limpar toda a memória
void MemoriaContigua::limpar() {
    processos.clear();
    blocos.clear();
    
    // Recriar como um único bloco livre
    blocos.push_back(BlocoMemoria(0, tamanho_total, false, -1));
    
    proximo_id = 1;
    ponteiro_circular = 0;
}