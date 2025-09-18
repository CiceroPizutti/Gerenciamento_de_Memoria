#include <iostream>
#include <string>
#include <limits>
#include "MemoriaContigua.h"
// #include "Paginacao.h"  // Implementaremos depois

using namespace std;

class Simulador
{
private:
    MemoriaContigua *memoria_contigua;
    // Paginacao* paginacao;  // Para implementar depois
    int modo_atual; // 1 = Contígua, 2 = Paginação

public:
    Simulador() : memoria_contigua(nullptr), modo_atual(0) {}

    ~Simulador()
    {
        if (memoria_contigua)
            delete memoria_contigua;
    }

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

void Simulador::executar()
{
    cout << "==================================" << endl;
    cout << "  SIMULADOR DE GERENCIAMENTO DE   " << endl;
    cout << "           MEMÓRIA                " << endl;
    cout << "==================================" << endl;

    while (true)
    {
        mostrarMenuPrincipal();

        int opcao;
        cout << "\nEscolha uma opcao: ";
        cin >> opcao;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcao invalida! Tente novamente." << endl;
            pausar();
            continue;
        }

        switch (opcao)
        {
        case 1:
            configurarMemoriaContigua();
            if (memoria_contigua)
                menuMemoriaContigua();
            break;
        case 2:
            cout << "Paginacao sera implementada em breve..." << endl;
            pausar();
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

void Simulador::mostrarMenuPrincipal()
{
    limparTela();
    cout << "\n========== MENU PRINCIPAL ==========" << endl;
    cout << "1. Alocacao Contigua Dinamica" << endl;
    cout << "2. Paginacao Pura" << endl;
    cout << "0. Sair" << endl;
    cout << "====================================" << endl;
}

void Simulador::configurarMemoriaContigua()
{
    limparTela();
    cout << "\n=== CONFIGURACAO MEMORIA CONTIGUA ===" << endl;

    int tamanho;
    cout << "Digite o tamanho total da memoria (KB): ";
    cin >> tamanho;

    if (cin.fail() || tamanho <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Tamanho invalido!" << endl;
        pausar();
        return;
    }

    if (memoria_contigua)
        delete memoria_contigua;
    memoria_contigua = new MemoriaContigua(tamanho);
    modo_atual = 1;

    cout << "Memoria inicializada com " << tamanho << " KB" << endl;
    pausar();
}

void Simulador::menuMemoriaContigua()
{
    while (true)
    {
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

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcao invalida!" << endl;
            pausar();
            continue;
        }

        switch (opcao)
        {
        case 1:
        {
            int tamanho;
            cout << "Digite o tamanho do processo (KB): ";
            cin >> tamanho;

            if (cin.fail() || tamanho <= 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Tamanho invalido!" << endl;
                pausar();
                break;
            }

            AlgoritmoAlocacao algoritmo = escolherAlgoritmo();

            if (memoria_contigua->alocarProcesso(tamanho, algoritmo))
            {
                cout << "Processo criado com sucesso!" << endl;
            }
            else
            {
                cout << "Falha ao alocar processo! Memoria insuficiente." << endl;
            }
            pausar();
            break;
        }
        case 2:
        {
            int id;
            cout << "Digite o ID do processo para remover: ";
            cin >> id;

            if (memoria_contigua->removerProcesso(id))
            {
                cout << "Processo removido com sucesso!" << endl;
            }
            else
            {
                cout << "Processo nao encontrado!" << endl;
            }
            pausar();
            break;
        }
        case 3:
            memoria_contigua->imprimirMemoria();
            pausar();
            break;
        case 4:
            memoria_contigua->imprimirTabelaProcessos();
            pausar();
            break;
        case 5:
            cout << "Fragmentacao Externa: "
                 << memoria_contigua->calcularFragmentacaoExterna() << "%" << endl;
            cout << "Utilizacao: "
                 << memoria_contigua->calcularUtilizacao() << "%" << endl;
            pausar();
            break;
        case 6:
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

AlgoritmoAlocacao Simulador::escolherAlgoritmo()
{
    cout << "\nEscolha o algoritmo de alocacao:" << endl;
    cout << "1. First-Fit" << endl;
    cout << "2. Best-Fit" << endl;
    cout << "3. Worst-Fit" << endl;
    cout << "4. Circular-Fit" << endl;

    int opcao;
    cout << "Algoritmo: ";
    cin >> opcao;

    switch (opcao)
    {
    case 1:
        return FIRST_FIT;
    case 2:
        return BEST_FIT;
    case 3:
        return WORST_FIT;
    case 4:
        return CIRCULAR_FIT;
    default:
        return FIRST_FIT;
    }
}

void Simulador::limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Simulador::pausar()
{
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main()
{
    Simulador simulador;
    simulador.executar();
    return 0;
}