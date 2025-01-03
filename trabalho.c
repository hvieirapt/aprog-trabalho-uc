#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

int gerarAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
};

// Struct Balcão
typedef struct {
    char nome[10];
    int id;
    int counter;
} balcao;

balcao listaBalcoes[3] = {
    {"A", 1, 0},
    {"B", 2, 0},
    {"C", 3, 0} // Balcão 3: Apenas Urgências
};

// Struct Tipo Ticket
typedef struct {
    char nome[10];
    balcao* balcoes[3];
} tipoTicket;

tipoTicket tiposTicket[2] = {
    {"Urgência", {&listaBalcoes[2], NULL, NULL}},
    {"Marcada", {&listaBalcoes[0], &listaBalcoes[1], &listaBalcoes[2]}}
};

// Struct Especialidade
typedef struct {
    char nome[20];
    char listaMedicos[4][50];
    float valorConsulta;
} especialidade;

int numEspecialidades = 4;
especialidade listaEspecialidades[4] = {
    {"Cardiologia", {"Dr. João", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Luís", "Dra. Paula"}, 25.50},
    {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. Júlia", "Dr. Mateus"}, 15.50}
};

// Struct Ticket
struct ticket {
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
}

struct ticket criarTicket(tipoTicket tipo, time_t* dataAtual) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    int numBalcoes = 0;

    for (int i = 0; i < 3; i++) {
        if (tipo.balcoes[i] != NULL) numBalcoes++;
    }

    novoTicket.balcao = tipo.balcoes[gerarAleatorio(0, numBalcoes - 1)];
    novoTicket.balcao->counter++;
    sprintf(novoTicket.id, "%s%d", novoTicket.balcao->nome, novoTicket.balcao->counter);
    novoTicket.dataCriacao = *dataAtual;
    gerarDataAtendimento(&novoTicket);
    *dataAtual += gerarAleatorio(1, 7) * 60;
    novoTicket.especialidade = listaEspecialidades[gerarAleatorio(0, numEspecialidades - 1)];
    novoTicket.gabinete = gerarAleatorio(1, 50);
    strcpy(novoTicket.medico, novoTicket.especialidade.listaMedicos[gerarAleatorio(0, 3)]);

    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0)
        novoTicket.valorPagar = 0.0;
    else
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;

    return novoTicket;
};

void criarTicketsAleatoriamente(struct ticket listaTickets[], int n, time_t* dataAtual) {
    for (int i = 0; i < n; i++) {
        struct ticket novoTicket = criarTicket(tiposTicket[gerarAleatorio(0, 1)], dataAtual);
        listaTickets[i] = novoTicket;
    }
};

void imprimirTicket(struct ticket t) {
    char bufferCriacao[20], bufferAtendimento[20];
    struct tm* tm_info;

    printf("Ticket: %s\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    printf("Balcão: %d (%s)\t", t.balcao->id, t.balcao->nome);

    tm_info = localtime(&t.dataCriacao);
    strftime(bufferCriacao, sizeof(bufferCriacao), "%Y-%m-%d %H:%M", tm_info);
    printf("Data de Criação: %s\t", bufferCriacao);

    tm_info = localtime(&t.dataAtendimento);
    strftime(bufferAtendimento, sizeof(bufferAtendimento), "%Y-%m-%d %H:%M", tm_info);
    printf("Data de Atendimento: %s\t", bufferAtendimento);

    printf("Especialidade: %s\t", t.especialidade.nome);
    printf("Médico: %s\t", t.medico);
    printf("Valor a Pagar: %.2f\t", t.valorPagar);
    printf("Gabinete: %d\n", t.gabinete);
};

struct ticket* filtrarTicketsPorIntervalo(struct ticket listaTickets[], int n, time_t dataInicio, time_t dataFim, int* quantidadeFiltrada) {
    struct ticket* ticketsFiltrados = malloc(n * sizeof(struct ticket));
    if (!ticketsFiltrados) {
        printf("Erro ao alocar memória para tickets filtrados.\n");
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
        printf("Erro ao redimensionar memória para tickets filtrados.\n");
        exit(EXIT_FAILURE);
    }

    return ticketsFiltrados;
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct ticket* obterTicketsFiltrados(struct ticket listaTickets[], int n, int* quantidadeFiltrada) {
    char dataInicioStr[20], dataFimStr[20];
    time_t dataInicio, dataFim;

    while (1) {
        printf("Insira a data inicial (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataInicioStr) != 1) {
            printf("Erro: Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }

        dataInicio = stringParaTimeT(dataInicioStr);
        if (dataInicio != (time_t)-1) break;

        printf("Erro: Formato inválido. Tente novamente.\n");
    }

    while (1) {
        printf("Insira a data final (formato YYYY-MM-DD HH:MM): ");
        if (scanf(" %[^\n]", dataFimStr) != 1) {
            printf("Erro: Entrada inválida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }

        dataFim = stringParaTimeT(dataFimStr);
        if (dataFim != (time_t)-1 && dataFim > dataInicio) break;

        printf("Erro: Formato inválido ou data final anterior à inicial. Tente novamente.\n");
    }

    return filtrarTicketsPorIntervalo(listaTickets, n, dataInicio, dataFim, quantidadeFiltrada);
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    time_t dataAtual = time(NULL);

    int quantidadeTickets = 30;
    struct ticket listaTickets[30];

    criarTicketsAleatoriamente(listaTickets, quantidadeTickets, &dataAtual);

    for (int i = 0; i < quantidadeTickets; i++) {
        imprimirTicket(listaTickets[i]);
    }

    int quantidadeFiltrada;
    struct ticket* ticketsFiltrados = obterTicketsFiltrados(listaTickets, quantidadeTickets, &quantidadeFiltrada);

    printf("Tickets filtrados (%d encontrados):\n", quantidadeFiltrada);
    for (int i = 0; i < quantidadeFiltrada; i++) {
        imprimirTicket(ticketsFiltrados[i]);
    }

    free(ticketsFiltrados);
    return 0;
}
