#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// Função para gerar um número aleatório dentro de um intervalo [min, max]
int gerarAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
};

// Estrutura que representa um balcão de atendimento
typedef struct {
    char nome[10];  // Nome do balcão
    int id;         // ID único do balcão
    int counter;    // Contador de tickets atendidos pelo balcão
} balcao;

// Lista com os três balcões disponíveis
balcao listaBalcoes[3] = {
    {"A", 1, 0},
    {"B", 2, 0},
    {"C", 3, 0} // Balcão 3: Único balcão que também aceita Urgências (não exclusivamente)
};

// Função para calcular a soma dos contadores de todos os balcões
int somaContadoresBalcoes() {
    int soma = 0;
    for (int i=0; i<3; i++) soma += listaBalcoes[i].counter;
    return soma;
}

// Estrutura que representa um tipo de ticket (Urgência ou Marcada)
typedef struct {
    char nome[10];          // Nome do tipo de ticket
    balcao* balcoes[3];     // Lista de balcões onde o ticket pode ser atendido (pointer para balcão)
} tipoTicket;

// Lista com os dois tipos de tickets disponíveis
tipoTicket tiposTicket[2] = {
    {"Urgência", {&listaBalcoes[2], NULL, NULL}},   // Apenas no balcão 3
    {"Marcada", {&listaBalcoes[0], &listaBalcoes[1], &listaBalcoes[2]}} // Qualquer balcão
};

// Estrutura que representa uma especialidade médica
typedef struct {
    char nome[20];              // Nome da especialidade
    char listaMedicos[4][50];   // Lista de médicos disponíveis para a especialidade
    float valorConsulta;        // Valor da consulta
} especialidade;

// Número de especialidades disponíveis
int numEspecialidades = 4;

// Lista com as especialidades disponíveis e os seus respetivos médicos
especialidade listaEspecialidades[4] = {
    {"Cardiologia", {"Dr. João", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Luís", "Dra. Paula"}, 25.50},
    {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. Júlia", "Dr. Mateus"}, 15.50}
};

// Estrutura que representa um ticket de atendimento
struct ticket {
    char id[15];                    // ID do ticket (gerado com base no balcão e número sequencial)
    tipoTicket tipo;                // Tipo do ticket (Urgência ou Marcada)
    time_t dataCriacao;             // Data e hora de criação do ticket
    time_t dataAtendimento;         // Data e hora do atendimento
    balcao* balcao;                 // Balcão onde o ticket será atendido
    char medico[50];                // Médico que irá realizar o atendimento
    especialidade especialidade;    // Especialidade da consulta
    int gabinete;                   // Gabinete onde o atendimento será realizado
    float valorPagar;               // Valor a pagar pela consulta
};

// Função para gerar a data de atendimento com base na data de criação
void gerarDataAtendimento(struct ticket* t) {
    t->dataAtendimento = t->dataCriacao + gerarAleatorio(300, 14400);   // Atendimento entre 5 minutos e 4 horas (em segundos)
};

// Função para converter uma string em formato de data para um tipo `time_t`
time_t stringParaTimeT(const char* dataStr) {
    struct tm tm = {0};

    if (sscanf(dataStr, "%4d-%2d-%2d %2d:%2d",
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min) != 5) {
        return (time_t)-1;  // Retorna um valor inválido em caso de erro
    }

    tm.tm_year -= 1900;     // Ajusta o ano para o formato time_t
    tm.tm_mon -= 1;         // Ajusta o mês para o formato time_t

    return mktime(&tm);     // Retorna a data convertida
};

// Função para criar um novo ticket com os dados fornecidos
struct ticket criarTicket(tipoTicket tipo, time_t* dataAtual, especialidade espSelecionada, int indexMedico) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;

    // Conta o número de balcões disponíveis para o tipo de ticket
    int numBalcoes = 0;
    for (int i=0; i<3; i++) if (tipo.balcoes[i] != NULL) numBalcoes++;

    novoTicket.balcao = tipo.balcoes[gerarAleatorio(0, numBalcoes - 1)];    // Seleciona um balcão aleatório dos disponíveis
    novoTicket.balcao->counter++;   // Incrementa o contador do balcão
    sprintf(novoTicket.id, "%s%d", novoTicket.balcao->nome, novoTicket.balcao->counter);    // Gera o ID do ticket
    novoTicket.dataCriacao = *dataAtual;    // Define a data de criação do ticket
    gerarDataAtendimento(&novoTicket);  // Gera a data de atendimento
    *dataAtual += gerarAleatorio(1, 7) * 60;    // Atualiza a data atual para o próximo ticket

    novoTicket.especialidade = espSelecionada;  // Define a especialidade
    novoTicket.gabinete = gerarAleatorio(1, 50);    // Define um gabinete aleatório
    strcpy(novoTicket.medico, novoTicket.especialidade.listaMedicos[indexMedico]);  // Define o médico

    // Define o valor a pagar dependendo do tipo de ticket
    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0) novoTicket.valorPagar = 0.0;   // Urgência é gratuita
    else novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;    // Valor da consulta marcada

    return novoTicket;
};

// Função para criar múltiplos tickets aleatórios
void criarTicketsAleatoriamente(struct ticket listaTickets[], int inicio, int n, time_t* dataAtual) {
    for (int i=0; i<n; i++) {
        // Seleciona uma especialidade aleatória
        especialidade randomEspecialidade = listaEspecialidades[gerarAleatorio(0, numEspecialidades - 1)];
        // Seleciona um médico aleatório dentro da especialidade
        int randomMedicoIndex = gerarAleatorio(0,3);
        // Cria o ticket com os dados gerados
        struct ticket novoTicket = criarTicket(tiposTicket[gerarAleatorio(0, 1)], dataAtual, randomEspecialidade, randomMedicoIndex);
        // Armazena o ticket na lista
        listaTickets[inicio + i] = novoTicket;
    }
}

// Função para imprimir um ticket de forma resumida
void imprimirTicket(struct ticket t) {
    char bufferCriacao[20], bufferAtendimento[20];
    struct tm* tm_info;

    printf("Id: %s\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    //printf("Balcão: %d (%s)\t", t.balcao->id, t.balcao->nome);

    tm_info = localtime(&t.dataCriacao);
    strftime(bufferCriacao, sizeof(bufferCriacao), "%Y-%m-%d %H:%M", tm_info);
    printf("Entrada: %s\t", bufferCriacao);

    tm_info = localtime(&t.dataAtendimento);
    strftime(bufferAtendimento, sizeof(bufferAtendimento), "%Y-%m-%d %H:%M", tm_info);
    printf("Atendimento: %s\t", bufferAtendimento);

    printf("Especialidade: %s\t", t.especialidade.nome);
    //printf("Médico: %s\t", t.medico);
    printf("Valor a Pagar: %.2f\t", t.valorPagar);
    printf("Gabinete: %d\n", t.gabinete);
};

// Função para imprimir os detalhes completos de um ticket
void imprimirTicketDetalhado(struct ticket listaTickets[], int n) {
    char id[15];
    printf("Digite o ID do ticket para ver os detalhes: ");
    scanf("%s", id);

    for (int i = 0; i < n; i++) {
        if (strcmp(listaTickets[i].id, id) == 0) {
            // Ticket encontrado, imprime os detalhes
            printf("\n#### DETALHES DO TICKET ####\n");
            printf("ID: %s\n", listaTickets[i].id);
            printf("Tipo: %s\n", listaTickets[i].tipo.nome);
            printf("Balcão: %d (%s)\n", listaTickets[i].balcao->id, listaTickets[i].balcao->nome);
            // Formata e imprime as datas
            char bufferCriacao[20], bufferAtendimento[20];
            struct tm* tm_info;

            tm_info = localtime(&listaTickets[i].dataCriacao);
            strftime(bufferCriacao, sizeof(bufferCriacao), "%Y-%m-%d %H:%M", tm_info);
            printf("Data de Criação: %s\n", bufferCriacao);

            tm_info = localtime(&listaTickets[i].dataAtendimento);
            strftime(bufferAtendimento, sizeof(bufferAtendimento), "%Y-%m-%d %H:%M", tm_info);
            printf("Data de Atendimento: %s\n", bufferAtendimento);

            printf("Especialidade: %s\n", listaTickets[i].especialidade.nome);
            printf("Médico: %s\n", listaTickets[i].medico);
            printf("Gabinete: %d\n", listaTickets[i].gabinete);
            printf("Valor a Pagar: %.2f\n", listaTickets[i].valorPagar);
            return;
        }
    }
    // Caso o ticket não seja encontrado
    printf("\nErro: Ticket com ID '%s' não encontrado.\n", id);
}

// Função para imprimir a lista completa de tickets
void imprimirListaTickets (struct ticket listaTickets[], int n) {
    for (int i = 0; i<n; i++) imprimirTicket(listaTickets[i]);
}

// Função para editar o valor a pagar de um ticket
void editarValorPagarTicket (struct ticket listaTickets[], int n, const char *id, float novoValorPagar) {
    ///Função para Editar Valor a Pagar de um Ticket, search feito pelo ID do Ticket
    for (int i = 0; i<n; i++){
        if (strcmp(listaTickets[i].id, id) == 0){ //Condição para encontrar Ticket com base no ID
            printf("\nTicket encontrado: %s\n", listaTickets[i].id);
            listaTickets[i].valorPagar = novoValorPagar; // Atualizar valor a pagar
            printf("Dados atualizados com sucesso.\n");
            return;
        }
    }
    // Caso o ticket não seja encontrado
    printf("Ticket %s não encontrado.\n", id);
}

// Função para editar a especialidade de um ticket
void editarEspecialidadeTicket (struct ticket listaTickets[], int n, const char *id, especialidade novaEspecialidade) {
    ///Função para Editar a Especialidade de um Ticket, atribuir aleatóriamente um médico da Especialidade e um balcão aleatório, search feito pelo ID do Ticket
    for (int i = 0; i<n; i++){
        if (strcmp(listaTickets[i].id, id) == 0){ //Condição para encontrar Ticket com base no ID
            printf("\nTicket encontrado: %s\n", listaTickets[i].id);
            listaTickets[i].especialidade = novaEspecialidade; //Atualizar especialidade
            strcpy(listaTickets[i].medico, listaTickets[i].especialidade.listaMedicos[gerarAleatorio(0, 3)]); //Copia do Array para a variável do tipo String
            listaTickets[i].gabinete = gerarAleatorio (1, 50); //Selecionar Gabinete Aleatóriamente
            if (strcmp(listaTickets[i].tipo.nome, tiposTicket[0].nome) == 0) //Caso seja urgência, valor a pagar será zero
                listaTickets[i].valorPagar = 0.0;
            else //Caso seja não urgência, valor a pagar será o atribuído na especialidade
                listaTickets[i].valorPagar = listaTickets[i].especialidade.valorConsulta;
            printf("Dados atualizados com sucesso.\n");
            return;
        }
    }
    // Caso o ticket não seja encontrado
    printf("Ticket %s não encontrado.\n", id);
}

// Função para filtrar tickets dentro de um intervalo de datas fornecido pelo utilizador
struct ticket* ticketsFiltradosNumIntervalo(struct ticket listaTickets[], int n, int* quantidadeFiltrada) {
    /*
    Recebe uma lista de Tickets
    Pede input do intervalo de datas
    Retorna a uma lista de Tickets contida no intervalo introduzido
    */
    char dataInicioStr[20], dataFimStr[20];
    time_t dataInicio, dataFim;

    // Solicita a data inicial ao utilizador
    while (1) {
        printf("\nInsira a data inicial (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataInicioStr) != 1) {
            printf("Erro: Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        dataInicio = stringParaTimeT(dataInicioStr);    // Converte a string para time_t
        if (dataInicio != (time_t)-1) break;            // Sai do loop se a data for válida

        printf("Erro: Formato inválido. Tente novamente.\n");
    }
    // Solicita a data final ao utilizador
    while (1) {
        printf("\nInsira a data final (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataFimStr) != 1) {
            printf("Erro: Entrada inválida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        dataFim = stringParaTimeT(dataFimStr);  // Converte a string para time_t
        if (dataFim != (time_t)-1 && dataFim > dataInicio) break;   // Sai do loop se a data final for válida e posterior à inicial

        printf("Erro: Formato inválido ou data final anterior à inicial. Tente novamente.\n");
    }

    // Aloca memória para a lista filtrada de tickets
    struct ticket* ticketsFiltrados = malloc(n * sizeof(struct ticket));
    if (!ticketsFiltrados) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE); // Encerra o programa em caso de erro de alocação
    }

    *quantidadeFiltrada = 0;    // Inicializa o contador de tickets filtrados

    // Filtra os tickets que estão dentro do intervalo fornecido
    for (int i = 0; i < n; i++) {
        if (listaTickets[i].dataAtendimento >= dataInicio && listaTickets[i].dataAtendimento <= dataFim) {
            ticketsFiltrados[*quantidadeFiltrada] = listaTickets[i];
            (*quantidadeFiltrada)++;
        }
    }

    // Redimensiona a memória para armazenar apenas os tickets filtrados
    ticketsFiltrados = realloc(ticketsFiltrados, (*quantidadeFiltrada) * sizeof(struct ticket));
    if (!ticketsFiltrados && *quantidadeFiltrada > 0) {
        printf("Erro ao redimensionar memória.\n");
        exit(EXIT_FAILURE); // Encerra o programa em caso de erro de redimensionamento
    }

    return ticketsFiltrados;    // Retorna a lista de tickets filtrados
}

// Função para gerar um mapa de volume de tickets atendidos num intervalo de datas
void mapaVolumeAtendidos (struct ticket listaTickets[], int n) {
    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);

    // Exibe o número de tickets encontrados e a lista filtrada
    printf("\n#### MAPA VOLUME ATENDIMENTO (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    free(ticketsFiltrados); // Limpa os tickets filtrados da memória
}

// Função para gerar um mapa de receitas num intervalo de datas
void mapaReceitaGerada (struct ticket listaTickets[], int n) {
    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    float totalReceita = 0.0;

    // Calcula o total das receitas no intervalo fornecido
    for (int i=0; i<quantidadeFiltrada; i++) totalReceita += ticketsFiltrados[i].valorPagar;

    printf("\n#### MAPA RECEITA GERADA (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> Total de receita gerada no intervalo pedido: %.2f\n", totalReceita);
    free(ticketsFiltrados); // Limpa os tickets filtrados da memória
}

// Função para calcular e exibir o tempo médio de espera entre a criação e o atendimento dos tickets
void mapaMediaAtendimento(struct ticket listaTickets[], int n) {
    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    float somaEspera = 0.0;

    // Calcula a soma do tempo de espera (em minutos) para os tickets filtrados
    for (int i=0; i<quantidadeFiltrada; i++) somaEspera += (float)difftime(ticketsFiltrados[i].dataAtendimento, ticketsFiltrados[i].dataCriacao) / 60;

    // Calcula a média do tempo de espera
    float mediaEspera = somaEspera / quantidadeFiltrada;

    // Exibe a lista de tickets considerados e a média de espera
    printf("\n#### MAPA TEMPO MÉDIO ATENDIMENTO (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> Média de espera para atendimento: %.2f minutos\n", mediaEspera);

    free(ticketsFiltrados); // Limpa os tickets filtrados da memória
}

// Função para gerar um mapa de produtividade dos balcões num intervalo de datas
void mapaProdutividadeBalcoes(struct ticket listaTickets[], int n) {
    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    int contadores[3] = {0, 0, 0};  // Contadores de tickets por balcão

    // Conta o número de tickets atendidos por cada balcão
    for (int i = 0; i < quantidadeFiltrada; i++) contadores[ticketsFiltrados[i].balcao->id - 1]++;

    // Identifica o balcão mais produtivo e o menos produtivo
    int maisProdutivo = 0, menosProdutivo = 0;
    for (int i = 1; i < 3; i++) {
        if (contadores[i] > contadores[maisProdutivo]) maisProdutivo = i;
        if (contadores[i] < contadores[menosProdutivo]) menosProdutivo = i;
    }

    printf("\n#### MAPA PRODUTIVIDADE BALCÕES (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> Balcão mais produtivo: %s (%d) (%d tickets atendidos)", listaBalcoes[maisProdutivo].nome, listaBalcoes[maisProdutivo].id, contadores[maisProdutivo]);
    printf("\n-> Balcão menos produtivo: %s (%d) (%d tickets atendidos)\n", listaBalcoes[menosProdutivo].nome, listaBalcoes[menosProdutivo].id, contadores[menosProdutivo]);

    free(ticketsFiltrados); // Limpa os tickets filtrados da memória
}

// Função para exibir o menu principal
void exibirMenu() {
    system("clear || cls"); // Limpa a tela
    printf("===============================================\n");
    printf("       Simulador Atendimento Centro Saúde       \n");
    printf("===============================================\n");
    printf("Contadores de Atendimentos:\n");
    for (int i = 0; i < 3; i++) printf("  Balcão %s: %d\n", listaBalcoes[i].nome, listaBalcoes[i].counter);

    printf("===============================================\n");
    printf("Menu:\n");
    printf("  1. Registar Tickets Aleatóriamente em Massa\n");
    printf("  2. Registar Ticket Manualmente\n");
    printf("  3. Visualizar Tickets em Memória\n");
    printf("  4. Visualizar Detalhes de Ticket\n");
    printf("  5. Mapas de Atendimento\n");
    printf("  6. Editar Ticket\n");
    printf("  q. Sair\n");
    printf("===============================================\n");
}

// Função para esperar por um input do utilizador antes de continuar
void esperarInput() {
    printf("\nPressione ENTER para continuar...");
    getchar(); // Consome o ENTER anterior
    getchar(); // Aguarda o novo ENTER
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));  // Inicializa o gerador de números aleatórios com o tempo atual
    time_t dataAtual = time(NULL);  // Obtém a data e hora atuais

    int quantidadeMaxTickets = 40; // Quantidade máxima de tickets permitidos em memória
    int quantidadeTickets = 0;    // Quantidade de tickets solicitados pelo utilizador
    int totalTickets = 0;         // Total de tickets atualmente em memória
    struct ticket listaTickets[40]; // Lista de Tickets em memória

    char escolha;   // Variável para armazenar a escolha do utilizador no menu
    do {
        exibirMenu();   // Mostra o menu principal
        printf("\nEscolha uma opção: ");
        scanf(" %c", &escolha);
        switch (escolha) {
            case '1': { // Registar tickets aleatórios em massa
                printf("Quantos Tickets deseja gerar? (máx. %d): ", quantidadeMaxTickets - totalTickets);
                scanf("%d", &quantidadeTickets);

                // Verifica se há espaço suficiente para os tickets solicitados
                int somaAtual = somaContadoresBalcoes();
                if (somaAtual + quantidadeTickets > quantidadeMaxTickets) {
                    printf("Erro: O total de tickets ultrapassa o máximo permitido (%d).\n", quantidadeMaxTickets);
                } else {
                    // Gera os tickets e atualiza o contador total
                    criarTicketsAleatoriamente(listaTickets, totalTickets, quantidadeTickets, &dataAtual);
                    totalTickets += quantidadeTickets; // Atualiza o total de tickets
                    printf("%d tickets foram gerados com sucesso!\n", quantidadeTickets);
                }
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            }
            case '2': { // Registar ticket manualmente
                if (totalTickets >= quantidadeMaxTickets) {
                    printf("Erro: Número máximo de tickets atingido (%d).\n", quantidadeMaxTickets);
                    esperarInput();
                    break;
                }

                // Seções para escolher tipo de ticket, especialidade e médico
                int tipoSelecionado;
                do {
                    printf("\nSelecione o tipo de ticket:\n");
                    printf("1. Urgência\n");
                    printf("2. Marcada\n");
                    printf("Escolha: ");
                    scanf("%d", &tipoSelecionado);

                    if (tipoSelecionado < 1 || tipoSelecionado > 2) {
                        printf("Opção inválida. Tente novamente.\n");
                    }
                } while (tipoSelecionado < 1 || tipoSelecionado > 2);

                tipoTicket tipoEscolhido = tiposTicket[tipoSelecionado - 1];

                // Submenu 2: Selecionar especialidade
                int especialidadeSelecionada;
                do {
                    printf("\nSelecione a especialidade:\n");
                    for (int i = 0; i < numEspecialidades; i++) {
                        printf("%d. %s\n", i + 1, listaEspecialidades[i].nome);
                    }
                    printf("Escolha: ");
                    scanf("%d", &especialidadeSelecionada);

                    if (especialidadeSelecionada < 1 || especialidadeSelecionada > numEspecialidades) {
                        printf("Opção inválida. Tente novamente.\n");
                    }
                } while (especialidadeSelecionada < 1 || especialidadeSelecionada > numEspecialidades);

                especialidade espEscolhida = listaEspecialidades[especialidadeSelecionada - 1];

                int medicoSelecionado;
                do {
                    printf("\nSelecione o médico para a especialidade %s:\n", espEscolhida.nome);
                    for (int i = 0; i < 4; i++) {
                        printf("%d. %s\n", i + 1, espEscolhida.listaMedicos[i]);
                    }
                    printf("Escolha: ");
                    scanf("%d", &medicoSelecionado);

                    if (medicoSelecionado < 1 || medicoSelecionado > 4) {
                        printf("Opção inválida. Tente novamente.\n");
                    }
                } while (medicoSelecionado < 1 || medicoSelecionado > 4);

                // Criar ticket com as opções selecionadas
                struct ticket novoTicket = criarTicket(tipoEscolhido, &dataAtual, espEscolhida, medicoSelecionado - 1);
                listaTickets[totalTickets] = novoTicket;
                totalTickets++;

                printf("\nTicket criado com sucesso! ID: %s\n", novoTicket.id);
                esperarInput();
                break;
            }
            case '3':   // Visualizar todos os tickets
                imprimirListaTickets(listaTickets, totalTickets); // Usa totalTickets para impressão
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            case '4':   // Visualizar os detalhes de um ticket específico
                imprimirTicketDetalhado(listaTickets, totalTickets); // Usa totalTickets para impressão
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            case '5': { // Mapas de atendimento
                char escolhaSubmenu;
                do {
                    // Exibir submenu
                    system("clear || cls"); // Limpa a tela (funciona no Linux e Windows)
                    printf("\n===============================================\n");
                    printf("            Mapas de Atendimento\n");
                    printf("===============================================\n");
                    printf("Selecione uma das opções abaixo:\n");
                    printf("  1. Mapa Volume de Tickets Atendidos num Intervalo de Datas\n");
                    printf("  2. Mapa Receita Gerada num Intervalo de Datas\n");
                    printf("  3. Mapa Tempo Médio de Atendimento num Intervalo de Datas\n");
                    printf("  4. Mapa Produtividade dos Balcões num Intervalo de Datas\n");
                    printf("  r. Voltar ao menu inicial\n");
                    printf("===============================================\n");
                    printf("\nEscolha uma opção: ");
                    scanf(" %c", &escolhaSubmenu);

                    switch (escolhaSubmenu) {
                        case '1':   // Mapa de volume de tickets atendidos
                            mapaVolumeAtendidos(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualização
                            break;
                        case '2':   // Mapa de receitas geradas
                            mapaReceitaGerada(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualização
                            break;
                        case '3':   // Mapa de tempo médio de atendimento
                            mapaMediaAtendimento(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualização
                            break;
                        case '4':   // Mapa de produtividade dos balcões
                            mapaProdutividadeBalcoes(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualização
                            break;
                        case 'r':   // Voltar ao menu principal
                            printf("Voltando ao menu inicial...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                            esperarInput(); // Pausa antes de retornar ao submenu
                            break;
                    }
                } while (escolhaSubmenu != 'r');
                break;
            }
            case '6': { // Editar ticket
                char escolhaEditar;
                do {
                    // Exibir submenu
                    system("clear || cls");
                    printf("\n===============================================\n");
                    printf("                 Editar Ticket\n");
                    printf("===============================================\n");
                    printf("Selecione uma das opções abaixo:\n");
                    printf("  1. Editar Especialidade\n");
                    printf("  r. Voltar ao menu inicial\n");
                    printf("===============================================\n");
                    printf("\nEscolha uma opção: ");
                    scanf(" %c", &escolhaEditar);

                    switch (escolhaEditar) {
                        case '1': { // Editar Especialidade
                            char id[15];
                            printf("\nDigite o ID do ticket a ser editado: ");
                            scanf("%s", id);

                            int ticketEncontrado = 0;
                            for (int i = 0; i < totalTickets; i++) {
                                if (strcmp(listaTickets[i].id, id) == 0) {
                                    ticketEncontrado = 1;

                                    // Submenu para selecionar nova especialidade
                                    int novaEspecialidade;
                                    do {
                                        printf("\nSelecione a nova especialidade:\n");
                                        for (int j = 0; j < numEspecialidades; j++) {
                                            printf("%d. %s\n", j + 1, listaEspecialidades[j].nome);
                                        }
                                        printf("Escolha: ");
                                        scanf("%d", &novaEspecialidade);

                                        if (novaEspecialidade < 1 || novaEspecialidade > numEspecialidades) {
                                            printf("Opção inválida. Tente novamente.\n");
                                        }
                                    } while (novaEspecialidade < 1 || novaEspecialidade > numEspecialidades);

                                    // Atualizar especialidade do ticket
                                    editarEspecialidadeTicket(listaTickets, totalTickets, id, listaEspecialidades[novaEspecialidade - 1]);
                                    break;
                                }
                            }

                            if (!ticketEncontrado) {
                                printf("\nErro: Ticket com ID '%s' não encontrado.\n", id);
                            }
                            esperarInput();
                            break;
                        }
                        case 'r':
                            printf("Voltando ao menu inicial...\n");
                            break;
                        default:
                            printf("Opção inválida! Tente novamente.\n");
                            esperarInput();
                            break;
                    }
                } while (escolhaEditar != 'r');
                break;
            }
            case 'q':   // Sair do programa
                printf("Programa Encerrado!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                esperarInput();
                break;
        }
    } while (escolha != 'q');
    return 0;
}
