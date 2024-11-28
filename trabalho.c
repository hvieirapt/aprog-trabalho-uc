#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
//Struct Balc�o
typedef struct {
    char nome[10];  //Nome Descritivo
    int id;         //ID comparativo
    int counter;    //Contador
} balcao;
//Lista Balc�es
balcao listaBalcoes[3] = {
    {"Balc�o 1", 1, 0},
    {"Balc�o 2", 2, 0},
    {"Balc�o 3", 3, 0}
};

//Struct Tipo Ticket
typedef struct {
    char nome[10];  //Nome Descritivo
    balcao *balcoes[3]; //Lista Balc�es Permitidos
} tipoTicket;
//Lista Tipos Ticket
tipoTicket tiposTicket[2] = {
    {"Urg�ncia",{&listaBalcoes[2],NULL ,NULL}},
    {"Marcada",{&listaBalcoes[0],&listaBalcoes[1],&listaBalcoes[2]}}
};

//Struct Especialidade
typedef struct {
    char nome[20];
    char listaMedicos[4][50]; // Nome m�dicos
    float valorConsulta; // Valor Tabelado por consulta
} especialidade;
//Lista Especialidades
especialidade listaEspecialidades[4] = {
    {"cardiologia", {"Dr. Jo�o", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Lu�s", "Dra. Paula"}, 25.50},
    {"ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. J�lia", "Dr. Mateus"}, 15.50}
};

//Struct Ticket
struct ticket {
    int id;
    tipoTicket tipo;
    time_t dataCriacao;
    time_t dataAtendimento;
    balcao *balcao;    //Pointer para o balc�o
    char medico[50];
    especialidade especialidade;
    int gabinete;
    float valorPagar;
};

// Fun��o para criar um ticket
struct ticket criarTicket(tipoTicket tipo) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    novoTicket.dataCriacao = time(NULL); // Atribui o timestamp atual
    novoTicket.dataAtendimento = 0;
    novoTicket.especialidade = listaEspecialidades[3];
    novoTicket.gabinete = 0;

    //Selecionar Balc�o Aleat�riamente com base no tipo de Ticket
    int numBalcoes = 0;
    for (int i = 0; i < 3; i++) {
        if (tipo.balcoes[i] != NULL) {
            numBalcoes++;
        }
    }
    int randomIndex = rand() % numBalcoes;
    novoTicket.balcao = tipo.balcoes[randomIndex];

    //Definir ID do Ticket com base no Counter do Balc�o Selecionado
    novoTicket.id = novoTicket.balcao->counter+1;

    //Incrementar Counter do Balc�o Selecionado
    novoTicket.balcao->counter++;

    //Verificar Valor a Pagar (Urg�ncias n�o pagam)
    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0) {
        novoTicket.valorPagar = 0.0;
    } else {
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;
    }

    return novoTicket;
};

// Fun��o para exibir um ticket
void imprimirTicket(struct ticket t) {
    char buffer[20];
    struct tm *tm_info = localtime(&t.dataCriacao);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("Ticket: %d\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    printf("Balc�o: %d\t", t.balcao->id);
    printf("Data de Cria��o: %s\n", buffer);
};

void criarTicketsAleatoriamente (struct ticket listaTickets[], int n){
    for (int i=0; i<n; i++){
        struct ticket novoTicket = criarTicket (tiposTicket[rand() % 2]);
        listaTickets[i] = novoTicket;
    }
};

int main(){
    setlocale(LC_ALL, ""); // Usa a configura��o de localidade do sistema
    srand(time(NULL));

    //Tickets em Mem�ria
    int quantidadeTickets = 10;
    struct ticket listaTickets[20];
    criarTicketsAleatoriamente (listaTickets, quantidadeTickets);

    //Imprimir Tickets em Mem�ria
    for (int i=0; i<quantidadeTickets; i++){
        imprimirTicket(listaTickets[i]);
    }

    //Imprimir Counter Balc�es
    for (int i=0; i<3; i++){
        printf("%s : %d", listaBalcoes[i].nome, listaBalcoes[i].counter);
        if (i < 3-1) printf("\t|\t");
    }

    return 0;
};
