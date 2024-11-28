#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
//Struct Balcão
typedef struct {
    char nome[10];  //Nome Descritivo
    int id;         //ID comparativo
    int counter;    //Contador
} balcao;
//Lista Balcões
balcao listaBalcoes[3] = {
    {"Balcão 1", 1, 0},
    {"Balcão 2", 2, 0},
    {"Balcão 3", 3, 0}
};

//Struct Tipo Ticket
typedef struct {
    char nome[10];  //Nome Descritivo
    balcao *balcoes[3]; //Lista Balcões Permitidos
} tipoTicket;
//Lista Tipos Ticket
tipoTicket tiposTicket[2] = {
    {"Urgência",{&listaBalcoes[2],NULL ,NULL}},
    {"Marcada",{&listaBalcoes[0],&listaBalcoes[1],&listaBalcoes[2]}}
};

//Struct Especialidade
typedef struct {
    char nome[20];
    char listaMedicos[4][50]; // Nome médicos
    float valorConsulta; // Valor Tabelado por consulta
} especialidade;
//Lista Especialidades
especialidade listaEspecialidades[4] = {
    {"cardiologia", {"Dr. João", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Luís", "Dra. Paula"}, 25.50},
    {"ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. Júlia", "Dr. Mateus"}, 15.50}
};

//Struct Ticket
struct ticket {
    int id;
    tipoTicket tipo;
    time_t dataCriacao;
    time_t dataAtendimento;
    balcao *balcao;    //Pointer para o balcão
    char medico[50];
    especialidade especialidade;
    int gabinete;
    float valorPagar;
};

// Função para criar um ticket
struct ticket criarTicket(tipoTicket tipo) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    novoTicket.dataCriacao = time(NULL); // Atribui o timestamp atual
    novoTicket.dataAtendimento = 0;
    novoTicket.especialidade = listaEspecialidades[3];
    novoTicket.gabinete = 0;

    //Selecionar Balcão Aleatóriamente com base no tipo de Ticket
    int numBalcoes = 0;
    for (int i = 0; i < 3; i++) {
        if (tipo.balcoes[i] != NULL) {
            numBalcoes++;
        }
    }
    int randomIndex = rand() % numBalcoes;
    novoTicket.balcao = tipo.balcoes[randomIndex];

    //Definir ID do Ticket com base no Counter do Balcão Selecionado
    novoTicket.id = novoTicket.balcao->counter+1;

    //Incrementar Counter do Balcão Selecionado
    novoTicket.balcao->counter++;

    //Verificar Valor a Pagar (Urgências não pagam)
    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0) {
        novoTicket.valorPagar = 0.0;
    } else {
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;
    }

    return novoTicket;
};

// Função para exibir um ticket
void imprimirTicket(struct ticket t) {
    char buffer[20];
    struct tm *tm_info = localtime(&t.dataCriacao);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("Ticket: %d\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    printf("Balcão: %d\t", t.balcao->id);
    printf("Data de Criação: %s\n", buffer);
};

void criarTicketsAleatoriamente (struct ticket listaTickets[], int n){
    for (int i=0; i<n; i++){
        struct ticket novoTicket = criarTicket (tiposTicket[rand() % 2]);
        listaTickets[i] = novoTicket;
    }
};

int main(){
    setlocale(LC_ALL, ""); // Usa a configuração de localidade do sistema
    srand(time(NULL));

    //Tickets em Memória
    int quantidadeTickets = 10;
    struct ticket listaTickets[20];
    criarTicketsAleatoriamente (listaTickets, quantidadeTickets);

    //Imprimir Tickets em Memória
    for (int i=0; i<quantidadeTickets; i++){
        imprimirTicket(listaTickets[i]);
    }

    //Imprimir Counter Balcões
    for (int i=0; i<3; i++){
        printf("%s : %d", listaBalcoes[i].nome, listaBalcoes[i].counter);
        if (i < 3-1) printf("\t|\t");
    }

    return 0;
};
