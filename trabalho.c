#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

int gerarAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
};

typedef struct {
    // Struct Balc�o
    char nome[10];
    int id;
    int counter;
} balcao;

balcao listaBalcoes[3] = {
    {"A", 1, 0},
    {"B", 2, 0},
    {"C", 3, 0} // Balc�o 3: Apenas Urg�ncias
};

int somaContadoresBalcoes() {
    int soma = 0;
    for (int i=0; i<3; i++) soma += listaBalcoes[i].counter;
    return soma;
}

typedef struct {
    // Struct Tipo Ticket
    char nome[10];
    balcao* balcoes[3];
} tipoTicket;

tipoTicket tiposTicket[2] = {
    {"Urg�ncia", {&listaBalcoes[2], NULL, NULL}},
    {"Marcada", {&listaBalcoes[0], &listaBalcoes[1], &listaBalcoes[2]}}
};

typedef struct {
    // Struct Especialidade
    char nome[20];
    char listaMedicos[4][50];
    float valorConsulta;
} especialidade;

int numEspecialidades = 4;
especialidade listaEspecialidades[4] = {
    {"Cardiologia", {"Dr. Jo�o", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Lu�s", "Dra. Paula"}, 25.50},
    {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. J�lia", "Dr. Mateus"}, 15.50}
};

struct ticket {
    // Struct Ticket
    char id[15];
    tipoTicket tipo;
    time_t dataCriacao;
    time_t dataAtendimento;
    balcao* balcao;
    char medico[50];
    especialidade especialidade;
    int gabinete;
    float valorPagar;
};

void gerarDataAtendimento(struct ticket* t) {
    t->dataAtendimento = t->dataCriacao + gerarAleatorio(300, 14400);
};

time_t stringParaTimeT(const char* dataStr) {
    struct tm tm = {0};

    if (sscanf(dataStr, "%4d-%2d-%2d %2d:%2d",
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min) != 5) {
        return (time_t)-1;
    }

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    return mktime(&tm);
};

struct ticket criarTicket(tipoTicket tipo, time_t* dataAtual) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    int numBalcoes = 0;

    for (int i = 0; i < 3; i++) if (tipo.balcoes[i] != NULL) numBalcoes++;

    novoTicket.balcao = tipo.balcoes[gerarAleatorio(0, numBalcoes - 1)];
    novoTicket.balcao->counter++;
    sprintf(novoTicket.id, "%s%d", novoTicket.balcao->nome, novoTicket.balcao->counter);
    novoTicket.dataCriacao = *dataAtual;
    gerarDataAtendimento(&novoTicket);
    *dataAtual += gerarAleatorio(1, 7) * 60;
    novoTicket.especialidade = listaEspecialidades[gerarAleatorio(0, numEspecialidades - 1)];
    novoTicket.gabinete = gerarAleatorio(1, 50);
    strcpy(novoTicket.medico, novoTicket.especialidade.listaMedicos[gerarAleatorio(0, 3)]);

    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0) novoTicket.valorPagar = 0.0;
    else novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;

    return novoTicket;
};

void criarTicketsAleatoriamente(struct ticket listaTickets[], int inicio, int n, time_t* dataAtual) {
    for (int i = 0; i < n; i++) {
        struct ticket novoTicket = criarTicket(tiposTicket[gerarAleatorio(0, 1)], dataAtual);
        listaTickets[inicio + i] = novoTicket;
    }
}

void imprimirTicket(struct ticket t) {
    char bufferCriacao[20], bufferAtendimento[20];
    struct tm* tm_info;

    printf("Id: %s\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    //printf("Balc�o: %d (%s)\t", t.balcao->id, t.balcao->nome);

    tm_info = localtime(&t.dataCriacao);
    strftime(bufferCriacao, sizeof(bufferCriacao), "%Y-%m-%d %H:%M", tm_info);
    printf("Entrada: %s\t", bufferCriacao);

    tm_info = localtime(&t.dataAtendimento);
    strftime(bufferAtendimento, sizeof(bufferAtendimento), "%Y-%m-%d %H:%M", tm_info);
    printf("Atendimento: %s\t", bufferAtendimento);

    printf("Especialidade: %s\t", t.especialidade.nome);
    //printf("M�dico: %s\t", t.medico);
    printf("Valor a Pagar: %.2f\t", t.valorPagar);
    printf("Gabinete: %d\n", t.gabinete);
};

void imprimirTicketDetalhado(struct ticket listaTickets[], int n) {
    char id[15];
    printf("Digite o ID do ticket para ver os detalhes: ");
    scanf("%s", id);

    for (int i = 0; i < n; i++) {
        if (strcmp(listaTickets[i].id, id) == 0) {
            printf("\n#### DETALHES DO TICKET ####\n");
            printf("ID: %s\n", listaTickets[i].id);
            printf("Tipo: %s\n", listaTickets[i].tipo.nome);
            printf("Balc�o: %d (%s)\n", listaTickets[i].balcao->id, listaTickets[i].balcao->nome);

            char bufferCriacao[20], bufferAtendimento[20];
            struct tm* tm_info;

            tm_info = localtime(&listaTickets[i].dataCriacao);
            strftime(bufferCriacao, sizeof(bufferCriacao), "%Y-%m-%d %H:%M", tm_info);
            printf("Data de Cria��o: %s\n", bufferCriacao);

            tm_info = localtime(&listaTickets[i].dataAtendimento);
            strftime(bufferAtendimento, sizeof(bufferAtendimento), "%Y-%m-%d %H:%M", tm_info);
            printf("Data de Atendimento: %s\n", bufferAtendimento);

            printf("Especialidade: %s\n", listaTickets[i].especialidade.nome);
            printf("M�dico: %s\n", listaTickets[i].medico);
            printf("Gabinete: %d\n", listaTickets[i].gabinete);
            printf("Valor a Pagar: %.2f\n", listaTickets[i].valorPagar);
            return;
        }
    }
    printf("\nErro: Ticket com ID '%s' n�o encontrado.\n", id);
}

void imprimirListaTickets (struct ticket listaTickets[], int n) {
    printf("\n#### TICKETS EM MEM�RIA (%d) ####\n", n);
    for (int i = 0; i<n; i++) imprimirTicket(listaTickets[i]);
}

void editarValorPagarTicket (struct ticket listaTickets[], int n, const char *id, float novoValorPagar) {
    ///Fun��o para Editar Valor a Pagar de um Ticket, search feito pelo ID do Ticket
    for (int i = 0; i<n; i++){
        if (strcmp(listaTickets[i].id, id) == 0){ //Condi��o para encontrar Ticket com base no ID
            printf("\nTicket encontrado: %s\n", listaTickets[i].id);
            listaTickets[i].valorPagar = novoValorPagar; // Atualizar valor a pagar
            listaTickets[i].valorPagar = novoValorPagar; //Atualizar valor a pagar
            printf("Dados atualizados com sucesso.\n");
            return;
        }
    }
    printf("Ticket %s n�o encontrado.\n", id);
}

void editarEspecialidadeTicket (struct ticket listaTickets[], int n, const char *id, especialidade novaEspecialidade) {
    ///Fun��o para Editar a Especialidade de um Ticket, atribuir aleat�riamente um m�dico da Especialidade e um balc�o aleat�rio, search feito pelo ID do Ticket
    for (int i = 0; i<n; i++){
        if (strcmp(listaTickets[i].id, id) == 0){ //Condi��o para encontrar Ticket com base no ID
            printf("\nTicket encontrado: %s\n", listaTickets[i].id);
            listaTickets[i].especialidade = novaEspecialidade; //Atualizar especialidade
            strcpy(listaTickets[i].medico, listaTickets[i].especialidade.listaMedicos[gerarAleatorio(0, 3)]); //Copia do Array para a vari�vel do tipo String
            listaTickets[i].gabinete = gerarAleatorio (1, 50); //Selecionar Gabinete Aleat�riamente
            if (strcmp(listaTickets[i].tipo.nome, tiposTicket[0].nome) == 0) //Caso seja urg�ncia, valor a pagar ser� zero
                listaTickets[i].valorPagar = 0.0;
            else //Caso seja n�o urg�ncia, valor a pagar ser� o atribu�do na especialidade
                listaTickets[i].valorPagar = listaTickets[i].especialidade.valorConsulta;
            printf("Dados atualizados com sucesso.\n");
            return;
        }
    }
    printf("Ticket %s n�o encontrado.\n", id);
}

struct ticket* ticketsFiltradosNumIntervalo(struct ticket listaTickets[], int n, int* quantidadeFiltrada) {
    /*
    Recebe uma lista de Tickets
    Pede input do intervalo de datas
    Retorna a uma lista de Tickets contida no intervalo introduzido
    */
    char dataInicioStr[20], dataFimStr[20];
    time_t dataInicio, dataFim;

    while (1) {
        printf("\nInsira a data inicial (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataInicioStr) != 1) {
            printf("Erro: Entrada inv�lida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        dataInicio = stringParaTimeT(dataInicioStr);
        if (dataInicio != (time_t)-1) break;

        printf("Erro: Formato inv�lido. Tente novamente.\n");
    }

    while (1) {
        printf("\nInsira a data final (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataFimStr) != 1) {
            printf("Erro: Entrada inv�lida. Tente novamente.\n");
            while (getchar() != '\n');
            continue;
        }

        dataFim = stringParaTimeT(dataFimStr);
        if (dataFim != (time_t)-1 && dataFim > dataInicio) break;

        printf("Erro: Formato inv�lido ou data final anterior � inicial. Tente novamente.\n");
    }

    struct ticket* ticketsFiltrados = malloc(n * sizeof(struct ticket));
    if (!ticketsFiltrados) {
        printf("Erro ao alocar mem�ria para tickets filtrados.\n");
        exit(EXIT_FAILURE);
    }

    *quantidadeFiltrada = 0;

    for (int i = 0; i < n; i++) {
        if (listaTickets[i].dataAtendimento >= dataInicio && listaTickets[i].dataAtendimento <= dataFim) {
            ticketsFiltrados[*quantidadeFiltrada] = listaTickets[i];
            (*quantidadeFiltrada)++;
        }
    }

    ticketsFiltrados = realloc(ticketsFiltrados, (*quantidadeFiltrada) * sizeof(struct ticket));
    if (!ticketsFiltrados && *quantidadeFiltrada > 0) {
        printf("Erro ao redimensionar mem�ria para tickets filtrados.\n");
        exit(EXIT_FAILURE);
    }

    return ticketsFiltrados;
}

void mapaVolumeAtendidos (struct ticket listaTickets[], int n) {
    ///Gerar mapas de volume de tickets atendidos, num intervalo de datas

    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    printf("\n#### MAPA VOLUME ATENDIMENTO (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    free(ticketsFiltrados);
}

void mapaReceitaGerada (struct ticket listaTickets[], int n) {
    ///Gerar mapas de receitas de consultas marcadas, num intervalo de datas

    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    printf("\n#### MAPA RECEITA GERADA (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    float totalReceita = 0.0;
    for (int i=0; i<quantidadeFiltrada; i++) totalReceita += ticketsFiltrados[i].valorPagar;
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> Total de receita gerada no intervalo pedido: %.2f\n", totalReceita);
    free(ticketsFiltrados);
}

void mapaMediaAtendimento(struct ticket listaTickets[], int n) {
    ///Gerar mapas de m�dias de espera entre atendimento, num intervalo de datas

    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    float somaEspera = 0.0;

    for (int i=0; i<quantidadeFiltrada; i++) somaEspera += (float)difftime(ticketsFiltrados[i].dataAtendimento, ticketsFiltrados[i].dataCriacao) / 60;

    float mediaEspera = somaEspera / quantidadeFiltrada;

    printf("\n#### MAPA TEMPO M�DIO ATENDIMENTO (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> M�dia de espera para atendimento: %.2f minutos\n", mediaEspera);
    free(ticketsFiltrados);
}

void mapaProdutividadeBalcoes(struct ticket listaTickets[], int n) {
    ///Gerar mapas de balc�es mais e menos produtivos, num intervalo de datas

    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = ticketsFiltradosNumIntervalo(listaTickets, n, &quantidadeFiltrada);
    int contadores[3] = {0, 0, 0};

    for (int i = 0; i < quantidadeFiltrada; i++) contadores[ticketsFiltrados[i].balcao->id - 1]++;

    int maisProdutivo = 0, menosProdutivo = 0;
    for (int i = 1; i < 3; i++) {
        if (contadores[i] > contadores[maisProdutivo]) maisProdutivo = i;
        if (contadores[i] < contadores[menosProdutivo]) menosProdutivo = i;
    }

    printf("\n#### MAPA PRODUTIVIDADE BALC�ES (%d Tickets Encontrados) ####\n", quantidadeFiltrada);
    printf("\nLista Tickets Considerados:\n");
    imprimirListaTickets (ticketsFiltrados, quantidadeFiltrada);
    printf("\n-> Balc�o mais produtivo: %s (%d) (%d tickets atendidos)", listaBalcoes[maisProdutivo].nome, listaBalcoes[maisProdutivo].id, contadores[maisProdutivo]);
    printf("\n-> Balc�o menos produtivo: %s (%d) (%d tickets atendidos)\n", listaBalcoes[menosProdutivo].nome, listaBalcoes[menosProdutivo].id, contadores[menosProdutivo]);
    free(ticketsFiltrados);
}


void exibirMenu() {
    // Fun��o para exibir o menu principal
    system("clear || cls"); // Limpa a tela (funciona no Linux e Windows)
    printf("===============================================\n");
    printf("       Simulador Atendimento Centro Sa�de       \n");
    printf("===============================================\n");
    printf("Contadores de Atendimentos:\n");
    for (int i = 0; i < 3; i++) printf("  Balc�o %s: %d atendimentos\n", listaBalcoes[i].nome, listaBalcoes[i].counter);

    printf("===============================================\n");
    printf("Menu:\n");
    printf("  1. Registar Tickets Aleat�riamente em Massa\n");
    printf("  2. Registar Ticket Manualmente\n");
    printf("  3. Visualizar Tickets em Mem�ria\n");
    printf("  4. Visualizar Detalhes de Ticket\n");
    printf("  5. Mapas de Atendimento\n");
    printf("  q. Sair\n");
    printf("===============================================\n");
}

void esperarInput() {
    // Fun��o para esperar input do utilizador antes de continuar
    printf("\nPressione ENTER para continuar...");
    getchar(); // Consome o ENTER anterior
    getchar(); // Aguarda o novo ENTER
}


int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    time_t dataAtual = time(NULL);

    int quantidadeMaxTickets = 40; // Quantidade m�xima permitida de tickets em mem�ria
    int quantidadeTickets = 0;    // Quantidade de tickets solicitados pelo utilizador
    int totalTickets = 0;         // Total de tickets em mem�ria
    struct ticket listaTickets[40]; // Lista de Tickets em mem�ria

    char escolha;
    do {
        exibirMenu();
        printf("\nEscolha uma op��o: ");
        scanf(" %c", &escolha);
        switch (escolha) {
            case '1': {
                printf("Quantos Tickets deseja gerar? (m�x. %d): ", quantidadeMaxTickets - totalTickets);
                scanf("%d", &quantidadeTickets);

                // Soma os contadores atuais com o input do utilizador
                int somaAtual = somaContadoresBalcoes();
                if (somaAtual + quantidadeTickets > quantidadeMaxTickets) {
                    printf("Erro: O total de tickets ultrapassa o m�ximo permitido (%d).\n", quantidadeMaxTickets);
                } else {
                    criarTicketsAleatoriamente(listaTickets, totalTickets, quantidadeTickets, &dataAtual);
                    totalTickets += quantidadeTickets; // Atualiza o total de tickets
                    printf("%d tickets foram gerados com sucesso!\n", quantidadeTickets);
                }
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            }
            case '2':
                break;
            case '3':
                imprimirListaTickets(listaTickets, totalTickets); // Usa totalTickets para impress�o
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            case '4':
                imprimirTicketDetalhado(listaTickets, totalTickets); // Usa totalTickets para impress�o
                esperarInput(); // Pausa antes de retornar ao menu
                break;
            case '5': {
                char escolhaSubmenu;
                do {
                    // Exibir submenu
                    system("clear || cls"); // Limpa a tela (funciona no Linux e Windows)
                    printf("\n===============================================\n");
                    printf("            Mapas de Atendimento\n");
                    printf("===============================================\n");
                    printf("Selecione uma das op��es abaixo:\n");
                    printf("  1. Mapa Volume de Tickets Atendidos num Intervalo de Datas\n");
                    printf("  2. Mapa Receita Gerada num Intervalo de Datas\n");
                    printf("  3. Mapa Tempo M�dio de Atendimento num Intervalo de Datas\n");
                    printf("  4. Mapa Produtividade dos Balc�es num Intervalo de Datas\n");
                    printf("  r. Voltar ao menu inicial\n");
                    printf("===============================================\n");
                    printf("\nEscolha uma op��o: ");
                    scanf(" %c", &escolhaSubmenu);

                    switch (escolhaSubmenu) {
                        case '1':
                            mapaVolumeAtendidos(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualiza��o
                            break;
                        case '2':
                            mapaReceitaGerada(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualiza��o
                            break;
                        case '3':
                            mapaMediaAtendimento(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualiza��o
                            break;
                        case '4':
                            mapaProdutividadeBalcoes(listaTickets, totalTickets);
                            esperarInput(); // Pausa para visualiza��o
                            break;
                        case 'r':
                            // Voltar ao menu principal
                            printf("Voltando ao menu inicial...\n");
                            break;
                        default:
                            printf("Op��o inv�lida! Tente novamente.\n");
                            esperarInput(); // Pausa antes de retornar ao submenu
                            break;
                    }
                } while (escolhaSubmenu != 'r');
                break;
            }
            case 'q':
                printf("At� logo!\n");
                break;
            default:
                printf("Op��o inv�lida! Tente novamente.\n");
                break;
        }
    } while (escolha != 'q');

    return 0;
}
