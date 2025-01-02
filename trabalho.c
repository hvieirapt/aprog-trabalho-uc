#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

int gerarAleatorio (int min, int max){
/// Fun��o retorna um n�mero entre min e max considerando tambem as extremidades.
    return rand() % (max - min + 1) + min;;
};

///Struct Balc�o
typedef struct {
    char nome[10];  //Nome Descritivo
    int id;         //ID
    int counter;    //Contador
} balcao;
//Lista Balc�es
balcao listaBalcoes[3] = {
    {"A", 1, 0},
    {"B", 2, 0},
    {"C", 3, 0} //Balc�o 3: Apenas Urg�ncias
};

///Struct Tipo Ticket
typedef struct {
    char nome[10];  //Nome Descritivo
    balcao *balcoes[3]; //Lista Balc�es Permitidos
} tipoTicket;
//Lista Tipos Ticket
tipoTicket tiposTicket[2] = {
    {"Urg�ncia",{&listaBalcoes[2],NULL ,NULL}},
    {"Marcada",{&listaBalcoes[0],&listaBalcoes[1],&listaBalcoes[2]}}
};

///Struct Especialidade
typedef struct {
    char nome[20];
    char listaMedicos[4][50]; // Nome m�dicos
    float valorConsulta; // Valor Tabelado por consulta
} especialidade;
//Lista Especialidades
int numEspecialidades = 4; //Quantidade para randomizer
especialidade listaEspecialidades[4] = {
    {"Cardiologia", {"Dr. Jo�o", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90},
    {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Lu�s", "Dra. Paula"}, 25.50},
    {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00},
    {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. J�lia", "Dr. Mateus"}, 15.50}
};

//Struct Ticket
struct ticket {
    char id[15];
    tipoTicket tipo;
    time_t dataCriacao;
    time_t dataAtendimento;
    balcao *balcao; //Pointer para o balc�o
    char medico[50];
    especialidade especialidade;
    int gabinete;
    float valorPagar;
};

struct ticket criarTicket (tipoTicket tipo, time_t *dataAtual) {
    /// Fun��o para criar um ticket
    /*
        Balc�o � selecionado aleat�riamente, limitado pelo tipo de Ticket introduzido.
        ID � atribu�do sequencialmente com base no balc�o selecionado.
        Especialidade � selecionada aleat�riamente.
        Gabinete � selecionado aleat�riamente.
        M�dico � selecionado aleat�riamente, limitado pela especialidade.
        Valor a Pagar � tabelado na especialidade.
    */
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    int numBalcoes = 0;
    for (int i = 0; i < 3; i++) if (tipo.balcoes[i] != NULL) numBalcoes++; //Contagem do N�mero de Balc�es do Tipo Introduzido
    novoTicket.balcao = tipo.balcoes[gerarAleatorio (0, numBalcoes-1)];
    novoTicket.balcao->counter++; //Incrementar Counter do Balc�o Selecionado
    sprintf(novoTicket.id, "%s%d", novoTicket.balcao->nome, novoTicket.balcao->counter); // Atribuir ID no formato "Balc�oNome+Counter"d
    novoTicket.dataCriacao = *dataAtual; // Atribui o timestamp atual
    *dataAtual = (*dataAtual + (gerarAleatorio (1, 7)) * 60); //Avan�ar Data Atual entre 1 a 7 minutos entre marca��es
    novoTicket.especialidade = listaEspecialidades[gerarAleatorio (0, numEspecialidades-1)];
    novoTicket.gabinete = gerarAleatorio (1, 50); //Selecionar Gabinete Aleat�riamente
    strcpy(novoTicket.medico, novoTicket.especialidade.listaMedicos[gerarAleatorio(0, 3)]); //Copia do Array para a vari�vel do tipo String
    if (strcmp(tipo.nome, tiposTicket[0].nome) == 0) //Caso seja urg�ncia, valor a pagar ser� zero
        novoTicket.valorPagar = 0.0;
    else //Caso seja n�o urg�ncia, valor a pagar ser� o atribu�do na especialidade
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;
    return novoTicket;
};

void editarValorPagarTicket (struct ticket listaTickets[], int n, const char *id, float novoValorPagar){
    ///Fun��o para Editar Valor a Pagar de um Ticket, search feito pelo ID do Ticket
    for (int i = 0; i<n; i++){
        if (strcmp(listaTickets[i].id, id) == 0) { //Condi��o para encontrar Ticket com base no ID
            printf("\nTicket encontrado: %s\n", listaTickets[i].id);
            listaTickets[i].valorPagar = novoValorPagar; // Atualizar valor a pagar
            printf("Dados atualizados com sucesso.\n");
            return;
        }
    }
    printf("Ticket %s n�o encontrado.\n", id);
};

void criarTicketsAleatoriamente (struct ticket listaTickets[], int n, time_t *dataAtual){
    ///Wrapper para Criar Lista de n Tickets e Gerar os seus Atendimentos Aleat�riamente
    for (int i=0; i<n; i++){
        struct ticket novoTicket = criarTicket (tiposTicket[gerarAleatorio (0, 1)], dataAtual);
        listaTickets[i] = novoTicket; //Atribuir Novo Ticket � lista em Mem�ria
    }
};

void imprimirTicket(struct ticket t) {
/// Fun��o para exibir um ticket
    char buffer[20];
    struct tm *tm_info = localtime(&t.dataCriacao);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", tm_info);
    printf("Ticket: %s\t", t.id);
    printf("Tipo: %s\t", t.tipo.nome);
    printf("Balc�o: %d (%s)\t", t.balcao->id, t.balcao->nome);
    printf("Data de Cria��o: %s\t", buffer);
    printf("Especialidade: %s\t", t.especialidade.nome);
    printf("M�dico: %s\t", t.medico);
    printf("Valor a Pagar: %.2f\t", t.valorPagar);
    printf("Gabinete: %d\n", t.gabinete);
};

int main(){
    setlocale(LC_ALL, ""); // Usa a configura��o de localidade do sistema
    srand(time(NULL));
    time_t dataAtual = time(NULL); //Data atual para simular progress�o do tempo

    int quantidadeTickets = 30; //Tickets a Gerar
    struct ticket listaTickets[30]; //Lista Tickets em Mem�ria

    criarTicketsAleatoriamente (listaTickets, quantidadeTickets, &dataAtual);


    //Imprimir Tickets em Mem�ria
    for (int i=0; i<quantidadeTickets; i++) imprimirTicket(listaTickets[i]);

    //Imprimir Counter Balc�es
    for (int i=0; i<3; i++){
        printf("%s : %d", listaBalcoes[i].nome, listaBalcoes[i].counter);
        if (i < 3-1) printf("\t|\t");
    }

    //editarValorPagarTicket (listaTickets, quantidadeTickets, "A5", 110.50);

    //Imprimir Tickets em Mem�ria
    for (int i=0; i<quantidadeTickets; i++) imprimirTicket(listaTickets[i]);

    return 0;
};
