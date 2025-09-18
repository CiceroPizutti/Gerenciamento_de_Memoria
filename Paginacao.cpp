#include "Paginacao.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

Paginacao::Paginacao(int tam_memoria, int tam_pagina)
    : tamanho_memoria(tam_memoria), tamanho_pagina(tam_pagina), proximo_id(1) {
    if (tam_memoria <= 0 || tam_pagina <= 0) {
        cout << "Erro: Tamanho da memoria e da pagina devem ser positivos. Inicializando com valores padrao." << endl;
        this->tamanho_memoria = 1024; // Valor padrão
        this->tamanho_pagina = 4; // Valor padrão
    }
    if (this->tamanho_memoria % this->tamanho_pagina != 0) {
        cout << "Aviso: Tamanho da memoria nao e multiplo do tamanho da pagina. Ajustando memoria." << endl;
        this->tamanho_memoria = (this->tamanho_memoria / this->tamanho_pagina) * this->tamanho_pagina;
    }
    numero_frames = this->tamanho_memoria / this->tamanho_pagina;
    inicializarFrames();
}

void Paginacao::inicializarFrames() {
    frames.assign(numero_frames, Frame());
    frames_livres.clear();
    // Preenche a lista de frames livres (0, 1, 2, ..., n-1)
    for (int i = 0; i < numero_frames; ++i) {
        frames_livres.push_back(i);
    }
    // Embaralha os frames livres para simular alocação não contígua mais realisticamente
    // random_shuffle(frames_livres.begin(), frames_livres.end()); // C++11 deprecated
    // Para C++17 e posterior:
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(frames_livres.begin(), frames_livres.end(), g);
}

bool Paginacao::alocarProcesso(int tamanho_processo) {
    if (tamanho_processo <= 0) {
        cout << "Erro: Tamanho do processo deve ser positivo." << endl;
        return false;
    }

    int paginas_necessarias = static_cast<int>(ceil(static_cast<double>(tamanho_processo) / tamanho_pagina));

    if (paginas_necessarias == 0) { // Processos muito pequenos que cabem em 0 páginas (ex: 0.1KB com página de 1KB) ainda precisam de 1 página
        paginas_necessarias = 1;
    }

    if (paginas_necessarias > static_cast<int>(frames_livres.size())) {
        cout << "Erro: Nao ha frames suficientes para alocar o processo de " << tamanho_processo << " KB (requer " << paginas_necessarias << " paginas)." << endl;
        return false; // Não há frames suficientes
    }

    Processo novo_processo(proximo_id, tamanho_processo);
    map<int, int> tabela_de_paginas;

    for (int i = 0; i < paginas_necessarias; ++i) {
        if (frames_livres.empty()) {
            // Isso nao deveria acontecer se a verificacao inicial estiver correta
            cout << "Erro interno: Frames livres esgotados durante a alocacao." << endl;
            // Desfazer alocações parciais se necessário
            return false;
        }
        int frame_idx = frames_livres.back();
        frames_livres.pop_back();

        frames[frame_idx].ocupado = true;
        frames[frame_idx].processo_id = proximo_id;
        frames[frame_idx].pagina_id = i;
        
        tabela_de_paginas[i] = frame_idx; // Mapeia pagina logica 'i' para frame fisico 'frame_idx'
    }

    novo_processo.setTabelaDePaginas(tabela_de_paginas);
    novo_processo.setAtivo(true);
    processos[proximo_id] = novo_processo;
    proximo_id++;

    return true;
}

bool Paginacao::removerProcesso(int processo_id) {
    auto it = processos.find(processo_id);
    if (it == processos.end()) {
        cout << "Erro: Processo P" << processo_id << " nao encontrado para remocao." << endl;
        return false; // Processo não encontrado
    }

    const auto& tabela = it->second.getTabelaDePaginas();
    for (const auto& par : tabela) {
        int frame_idx = par.second;
        if (frame_idx >= 0 && frame_idx < numero_frames) { // Validação de índice
            frames[frame_idx] = Frame(); // Reseta o frame
            frames_livres.push_back(frame_idx); // Devolve à lista de livres
        }
    }
    // Reordena frames_livres para manter a consistência (opcional, mas bom para visualização)
    sort(frames_livres.begin(), frames_livres.end());

    processos.erase(it);
    return true;
}

void Paginacao::imprimirMemoria() const {
    cout << "\n==== MAPA DE FRAMES DA MEMORIA FISICA ====" << endl;
    cout << "Tamanho da Memoria: " << tamanho_memoria << " KB | ";
    cout << "Tamanho da Pagina/Frame: " << tamanho_pagina << " KB | ";
    cout << "Total de Frames: " << numero_frames << endl << endl;

    if (numero_frames == 0) {
        cout << "Nenhum frame configurado (memoria ou pagina de tamanho zero?)." << endl;
        return;
    }

    for (int i = 0; i < numero_frames; ++i) {
        cout << "Frame " << setw(3) << i << ": ";
        if (frames[i].ocupado) {
            cout << "[P" << setw(2) << frames[i].processo_id 
                 << " | Pag " << setw(2) << frames[i].pagina_id << "]";
        } else {
            cout << "[--- LIVRE ---]";
        }
        if ((i + 1) % 4 == 0) { // Imprime 4 frames por linha para melhor visualização
            cout << endl;
        } else {
            cout << "   ";
        }
    }
    cout << "\n" << string(50, '-') << endl;
}

void Paginacao::imprimirTabelaDePaginas(int processo_id) const {
    auto it = processos.find(processo_id);
    if (it == processos.end()) {
        cout << "Erro: Processo P" << processo_id << " nao encontrado." << endl;
        return;
    }

    const Processo& p = it->second;
    cout << "\n==== TABELA DE PAGINAS DO PROCESSO P" << p.getId() << " ====" << endl;
    cout << "Tamanho Logico do Processo: " << p.getTamanho() << " KB" << endl;
    cout << "Tamanho da Pagina: " << tamanho_pagina << " KB" << endl;
    cout << "Paginas Requeridas: " << p.getTabelaDePaginas().size() << endl;
    cout << setw(10) << "Pagina" << " | " << setw(10) << "Frame Fisico" << endl;
    cout << string(25, '-') << endl;

    const auto& tabela = p.getTabelaDePaginas();
    for (const auto& par : tabela) {
        cout << setw(10) << par.first << " | " << setw(10) << par.second << endl;
    }
    cout << endl;
}

void Paginacao::imprimirTabelaProcessos() const {
    if (processos.empty()) {
        cout << "\nNenhum processo na memoria (Paginacao)." << endl;
        return;
    }
    
    cout << "\n==== TABELA DE PROCESSOS (PAGINACAO) ====" << endl;
    cout << setw(5) << "PID" << setw(10) << "Tamanho" << setw(15) << "# Paginas" << setw(8) << "Status" << endl;
    cout << string(45, '-') << endl;
    
    for (const auto& pair : processos) {
        const Processo& p = pair.second;
        int num_paginas = static_cast<int>(ceil(static_cast<double>(p.getTamanho()) / tamanho_pagina));
        cout << setw(5) << p.getId()
             << setw(10) << p.getTamanho()
             << setw(15) << num_paginas
             << setw(8) << (p.isAtivo() ? "ATIVO" : "INATIVO")
             << endl;
    }
    cout << endl;
}

double Paginacao::calcularFragmentacaoInterna() const {
    double fragmentacao_total_kb = 0;
    int memoria_alocada_processos_kb = 0;

    for (const auto& par : processos) {
        const Processo& p = par.second;
        int paginas_alocadas_para_processo = p.getTabelaDePaginas().size();
        int memoria_realmente_usada_pelo_processo = p.getTamanho();
        
        // Memória alocada em frames para o processo
        int memoria_alocada_em_frames = paginas_alocadas_para_processo * tamanho_pagina;
        
        // Fragmentação interna para este processo
        fragmentacao_total_kb += (memoria_alocada_em_frames - memoria_realmente_usada_pelo_processo);
        memoria_alocada_processos_kb += memoria_alocada_em_frames;
    }

    if (memoria_alocada_processos_kb == 0) return 0.0; // Evita divisão por zero se não houver processos
    
    // Percentual de fragmentação interna em relação à memória total alocada para processos
    return (fragmentacao_total_kb / memoria_alocada_processos_kb) * 100.0;
}

double Paginacao::calcularUtilizacao() const {
    int frames_ocupados = numero_frames - frames_livres.size();
    if (numero_frames == 0) return 0.0; // Evita divisão por zero
    return (static_cast<double>(frames_ocupados) / numero_frames) * 100.0;
}

void Paginacao::limpar() {
    processos.clear();
    inicializarFrames();
    proximo_id = 1;
    cout << "Memoria de paginacao limpa e reconfigurada." << endl;
}


