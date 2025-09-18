#include "Simulador.h"

using namespace std;

Simulador::Simulador() : memoria_contigua(nullptr), paginacao(nullptr), modo_atual(0) {}

Simulador::~Simulador() {
    if (memoria_contigua) delete memoria_contigua;
    if (paginacao) delete paginacao;
}

void Simulador::executar() {
    cout << "==================================" << endl;
    cout << "  SIMULADOR DE GERENCIAMENTO DE   " << endl;
    cout << "           MEMÓRIA                " << endl;
    cout << "==================================" << endl;
    
    while (true) {
        mostrarMenuPrincipal();
        
        int opcao;
        cout << "\nEscolha uma opcao: ";
        cin >> opcao;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcao invalida! Tente novamente." << endl;
            pausar();
            continue;
        }
        
        switch (opcao) {
            case 1:
                configurarMemoriaContigua();
                if (memoria_contigua) menuMemoriaContigua();
                break;
            case 2:
                configurarPaginacao();
                if (paginacao) menuPaginacao();
                break;
            case 0:
                cout << "Encerrando simulador..." << endl;
                return;
            default:
                cout << "Opcao invalida!" << endl;
                pausar();
        }
    }
}

void Simulador::mostrarMenuPrincipal() {
    limparTela();
    cout << "\n========== MENU PRINCIPAL ==========" << endl;
    cout << "1. Alocacao Contigua Dinamica" << endl;
    cout << "2. Paginacao Pura" << endl;
    cout << "0. Sair" << endl;
    cout << "====================================" << endl;
}

void Simulador::configurarMemoriaContigua() {
    limparTela();
    cout << "\n=== CONFIGURACAO MEMORIA CONTIGUA ===" << endl;
    
    int tamanho;
    cout << "Digite o tamanho total da memoria (KB): ";
    cin >> tamanho;
    
    if (cin.fail() || tamanho <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tamanho invalido!" << endl;
        pausar();
        return;
    }
    
    if (memoria_contigua) delete memoria_contigua;
    if (paginacao) { delete paginacao; paginacao = nullptr; } // Garante que apenas um modo esteja ativo
    memoria_contigua = new MemoriaContigua(tamanho);
    modo_atual = 1;
    
    cout << "Memoria Contigua inicializada com " << tamanho << " KB" << endl;
    pausar();
}

void Simulador::menuMemoriaContigua() {
    while (true) {
        limparTela();
        cout << "\n====== ALOCACAO CONTIGUA ======" << endl;
        cout << "1. Criar processo" << endl;
        cout << "2. Remover processo" << endl;
        cout << "3. Visualizar memoria" << endl;
        cout << "4. Tabela de processos" << endl;
        cout << "5. Metricas de fragmentacao" << endl;
        cout << "6. Limpar memoria" << endl;
        cout << "0. Voltar ao menu principal" << endl;
        cout << "===============================" << endl;
        
        int opcao;
        cout << "\nEscolha uma opcao: ";
        cin >> opcao;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcao invalida!" << endl;
            pausar();
            continue;
        }
        
        switch (opcao) {
            case 1: { // Criar processo
                int tamanho;
                cout << "Digite o tamanho do processo (KB): ";
                cin >> tamanho;
                
                if (cin.fail() || tamanho <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Tamanho invalido!" << endl;
                    pausar();
                    break;
                }
                
                AlgoritmoAlocacao algoritmo = escolherAlgoritmo();
                if (algoritmo == static_cast<AlgoritmoAlocacao>(-1)) { // Algoritmo inválido
                    break;
                }
                
                if (memoria_contigua->alocarProcesso(tamanho, algoritmo)) {
                    cout << "Processo criado com sucesso!" << endl;
                } else {
                    cout << "Falha ao alocar processo! Memoria insuficiente ou fragmentada." << endl;
                }
                pausar();
                break;
            }
            case 2: { // Remover processo
                int id;
                cout << "Digite o ID do processo para remover: ";
                cin >> id;
                
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "ID invalido!" << endl;
                    pausar();
                    break;
                }

                if (memoria_contigua->removerProcesso(id)) {
                    cout << "Processo removido com sucesso!" << endl;
                } else {
                    cout << "Processo nao encontrado!" << endl;
                }
                pausar();
                break;
            }
            case 3: // Visualizar memoria
                memoria_contigua->imprimirMemoria();
                pausar();
                break;
            case 4: // Tabela de processos
                memoria_contigua->imprimirTabelaProcessos();
                pausar();
                break;
            case 5: // Metricas de fragmentacao
                cout << "\n--- Metricas de Fragmentacao (Alocacao Contigua) ---" << endl;
                cout << "Fragmentacao Externa: " 
                     << fixed << setprecision(2) << memoria_contigua->calcularFragmentacaoExterna() << "%" << endl;
                cout << "Utilizacao da Memoria: " 
                     << fixed << setprecision(2) << memoria_contigua->calcularUtilizacao() << "%" << endl;
                pausar();
                break;
            case 6: // Limpar memoria
                memoria_contigua->limpar();
                cout << "Memoria limpa!" << endl;
                pausar();
                break;
            case 0:
                return;
            default:
                cout << "Opcao invalida!" << endl;
                pausar();
        }
    }
}

void Simulador::configurarPaginacao() {
    limparTela();
    cout << "\n====== CONFIGURACAO PAGINACAO PURA ======" << endl;
    int tam_memoria, tam_pagina;

    cout << "Digite o tamanho total da memoria (KB): ";
    cin >> tam_memoria;
    if (cin.fail() || tam_memoria <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tamanho de memoria invalido!" << endl;
        pausar();
        return;
    }

    cout << "Digite o tamanho da pagina/frame (KB): ";
    cin >> tam_pagina;
    if (cin.fail() || tam_pagina <= 0 || tam_pagina > tam_memoria) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tamanho de pagina invalido! Deve ser maior que 0 e menor ou igual ao tamanho da memoria." << endl;
        pausar();
        return;
    }
    if (tam_memoria % tam_pagina != 0) {
        cout << "Aviso: O tamanho da memoria nao e multiplo do tamanho da pagina. A memoria sera truncada para o maior multiplo de pagina." << endl;
        tam_memoria = (tam_memoria / tam_pagina) * tam_pagina;
        cout << "Novo tamanho de memoria: " << tam_memoria << " KB" << endl;
        pausar();
    }

    if (paginacao) delete paginacao;
    if (memoria_contigua) { delete memoria_contigua; memoria_contigua = nullptr; } // Garante que apenas um modo esteja ativo
    paginacao = new Paginacao(tam_memoria, tam_pagina);
    modo_atual = 2;

    cout << "Memoria para paginacao configurada com sucesso. Memoria: " << tam_memoria << " KB, Pagina: " << tam_pagina << " KB." << endl;
    pausar();
}

void Simulador::menuPaginacao() {
    while (true) {
        limparTela();
        cout << "\n========== PAGINACAO PURA ==========" << endl;
        cout << "1. Criar processo" << endl;
        cout << "2. Remover processo" << endl;
        cout << "3. Visualizar mapa de frames" << endl;
        cout << "4. Tabela de paginas de um processo" << endl;
        cout << "5. Tabela de todos os processos" << endl;
        cout << "6. Metricas de fragmentacao" << endl;
        cout << "7. Limpar memoria" << endl;
        cout << "0. Voltar ao menu principal" << endl;
        cout << "====================================" << endl;
        
        int opcao;
        cout << "\nEscolha uma opcao: ";
        cin >> opcao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcao invalida!" << endl;
            pausar();
            continue;
        }

        switch (opcao) {
            case 1: { // Criar processo
                int tamanho;
                cout << "Digite o tamanho do processo (KB): ";
                cin >> tamanho;
                if (cin.fail() || tamanho <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Tamanho invalido!" << endl;
                    pausar();
                    break;
                }
                if (paginacao->alocarProcesso(tamanho)) {
                    cout << "Processo P" << paginacao->getUltimoIdProcesso() << " criado com sucesso!" << endl;
                } else {
                    cout << "Falha ao alocar processo!" << endl;
                }
                pausar();
                break;
            }
            case 2: { // Remover processo
                int id;
                cout << "Digite o ID do processo para remover: ";
                cin >> id;
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "ID invalido!" << endl;
                    pausar();
                    break;
                }
                if (paginacao->removerProcesso(id)) {
                    cout << "Processo P" << id << " removido com sucesso!" << endl;
                } else {
                    cout << "Processo nao encontrado ou falha na remocao!" << endl;
                }
                pausar();
                break;
            }
            case 3: // Visualizar mapa de frames
                paginacao->imprimirMemoria();
                pausar();
                break;
            case 4: { // Tabela de paginas de um processo
                int id;
                cout << "Digite o ID do processo: ";
                cin >> id;
                if (cin.fail() || id <= 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "ID invalido!" << endl;
                    pausar();
                    break;
                }
                paginacao->imprimirTabelaDePaginas(id);
                pausar();
                break;
            }
            case 5: // Tabela de todos os processos
                paginacao->imprimirTabelaProcessos();
                pausar();
                break;
            case 6: // Metricas de fragmentacao
                cout << "\n--- Metricas de Fragmentacao (Paginacao Pura) ---" << endl;
                cout << "Fragmentacao Interna: " 
                     << fixed << setprecision(2) << paginacao->calcularFragmentacaoInterna() << "%" << endl;
                cout << "Utilizacao da Memoria: " 
                     << fixed << setprecision(2) << paginacao->calcularUtilizacao() << "%" << endl;
                pausar();
                break;
            case 7: // Limpar memoria
                paginacao->limpar();
                pausar();
                break;
            case 0:
                return;
            default:
                cout << "Opcao invalida!" << endl;
                pausar();
        }
    }
}

AlgoritmoAlocacao Simulador::escolherAlgoritmo() {
    cout << "\nEscolha o algoritmo de alocacao:" << endl;
    cout << "1. First-Fit" << endl;
    cout << "2. Best-Fit" << endl;
    cout << "3. Worst-Fit" << endl;
    cout << "4. Circular-Fit" << endl;
    
    int opcao;
    cout << "Algoritmo: ";
    cin >> opcao;
    
    if (cin.fail() || opcao < 1 || opcao > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcao invalida! Usando First-Fit como padrao." << endl;
        pausar();
        return FIRST_FIT;
    }

    switch (opcao) {
        case 1: return FIRST_FIT;
        case 2: return BEST_FIT;
        case 3: return WORST_FIT;
        case 4: return CIRCULAR_FIT;
        default: return FIRST_FIT; // Nao deve ser alcancado devido a validacao acima
    }
}

void Simulador::limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Simulador::pausar() {
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


