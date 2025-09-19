# Simulador de Gerenciamento de Memória

## 👥 Integrantes da Dupla
- **Cícero Pizutti** - 68612
- **Samuel Rait** - 1999543

## 📋 Descrição do Projeto

Este projeto implementa um simulador educacional para demonstrar conceitos fundamentais de **Gerenciamento de Memória** em Sistemas Operacionais 2. O simulador permite comparar diferentes estratégias de alocação de memória através de duas abordagens principais:

1. **Alocação Contígua Dinâmica**: Demonstra algoritmos clássicos de alocação e o problema da fragmentação externa
2. **Paginação Pura**: Mostra como a paginação resolve a fragmentação externa, mas introduz fragmentação interna

### 🎯 Tópico Escolhido
**Tópico 1: Gerenciamento de Memória (Alocação Contígua e Paginação)**

### 🎓 Objetivos Educacionais
- Demonstrar a diferença entre endereçamento lógico e físico
- Visualizar o problema da fragmentação externa na alocação contígua
- Comparar algoritmos de alocação (First-Fit, Best-Fit, Worst-Fit, Circular-Fit)
- Mostrar como a paginação resolve a fragmentação externa
- Calcular e comparar métricas de fragmentação e utilização

## 🛠️ Especificações Técnicas

### Linguagem de Programação
- **C++11** ou superior
- Compilador: g++ (GCC) ou equivalente

### Interface
- **Terminal/Console**: Interface textual com menus interativos
- Visualização ASCII da memória e tabelas de processos
- Navegação por menus numerados

## 🚀 Instruções de Compilação e Execução

### Pré-requisitos
- Compilador g++ com suporte a C++11
- Sistema operacional: Linux, macOS ou Windows (com MinGW/MSYS2)

### Compilação
```bash
# Clonar o repositório
git clone [URL_DO_REPOSITORIO]

# Compilar o projeto
make

```

### Execução
```bash
# Executar o simulador
./simulador-memoria

```

## 📖 Funcionalidades Implementadas

### 🔄 Modo 1: Alocação Contígua Dinâmica

#### Algoritmos de Alocação
- **First-Fit**: Aloca no primeiro bloco livre suficiente
- **Best-Fit**: Aloca no menor bloco livre que caiba o processo
- **Worst-Fit**: Aloca no maior bloco livre disponível
- **Circular-Fit**: First-Fit com ponteiro circular

#### Operações Disponíveis
- ✅ Criar processos com tamanhos variados
- ✅ Remover processos da memória
- ✅ Visualizar estado atual da memória
- ✅ Exibir tabela de processos com endereços base e limite
- ✅ Calcular fragmentação externa e utilização

#### Visualização da Memória
```
==== ESTADO DA MEMORIA ====
[P1  ][P1  ][----][P2  ][P2  ][P2  ][----][----]
 0-15  16-31  32-47 48-63 64-79 80-95 96-111 112-127

Legenda: [P1] = Processo 1, [----] = Livre
```

### 📄 Modo 2: Paginação Pura

#### Configurações
- ✅ Tamanho configurável de páginas
- ✅ Número configurável de frames
- ✅ Múltiplos processos simultâneos

#### Funcionalidades
- ✅ Alocação não-contígua de páginas em frames
- ✅ Tabela de páginas por processo
- ✅ Visualização de mapeamento páginas → frames
- ✅ Cálculo de fragmentação interna

#### Visualização da Paginação
```
==== FRAMES DE MEMORIA ====
Frame 0: [Pag 2 - P1] Frame 1: [Pag 0 - P2] Frame 2: [Pag 1 - P1]
Frame 3: [Pag 1 - P2] Frame 4: [Livre   ] Frame 5: [Livre   ]

==== TABELA DE PAGINAS - PROCESSO 1 ====
Página 0 → Frame 3   Página 1 → Frame 2   Página 2 → Frame 0
```

### 📊 Métricas Calculadas

#### Alocação Contígua
- **Fragmentação Externa**: `(Total Livre - Maior Bloco Livre) / Total * 100%`
- **Utilização**: `Total Usado / Total * 100%`

#### Paginação
- **Fragmentação Interna**: `(Espaço Desperdiçado nas Páginas) / Total * 100%`
- **Aproveitamento**: `Total Usado / Total Alocado * 100%`

## 🎯 Decisões de Projeto

### Estruturas de Dados Escolhidas
- **`std::vector<BlocoMemoria>`**: Para representar a memória como sequência de blocos
- **`std::map<int, Processo>`**: Para armazenar processos ativos (busca eficiente por ID)
- **`std::vector<int>`**: Para tabelas de páginas (mapeamento página → frame)

### Algoritmos Implementados
- **Consolidação de blocos**: Une blocos livres adjacentes automaticamente
- **Circular-Fit**: Mantém ponteiro para evitar sempre começar do início
- **Best-Fit**: Percorre todos os blocos para encontrar o melhor ajuste

### Tratamento de Erros
- Validação de entradas do usuário
- Verificação de memória insuficiente
- Tratamento de IDs de processos inexistentes
- Limpeza automática de ponteiros

### Comparação de Algoritmos
| Algoritmo    | Fragmentação | Velocidade | Uso Típico |
|-------------|-------------|------------|------------|
| First-Fit   | Moderada    | Rápida     | Geral      |
| Best-Fit    | Baixa       | Lenta      | Otimização |
| Worst-Fit   | Alta        | Lenta      | Casos especiais |
| Circular-Fit| Moderada    | Rápida     | Distribuída |

## 🤝 Contribuições

Ambos os integrantes da dupla contribuíram igualmente para o desenvolvimento:
- **Implementação**: Divisão por módulos (contígua vs paginação)
- **Documentação**: Revisão cruzada
- **Testes**: Validação conjunta de cenários
- **Mini-aula**: Apresentação compartilhada

## 📚 Referências Bibliográficas

- Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). *Operating System Concepts* (10th ed.)
- Tanenbaum, A. S., & Bos, H. (2014). *Modern Operating Systems* (4th ed.)
- Stallings, W. (2017). *Operating Systems: Internals and Design Principles* (8th ed.)

## 📝 Licença

Este projeto foi desenvolvido para fins educacionais como parte da disciplina de Sistemas Operacionais.
