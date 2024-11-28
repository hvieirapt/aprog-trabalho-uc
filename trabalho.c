#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para time_t e manipula��o de datas
#include <locale.h>


// Declara��o e inicializa��o de structs globais

typedef struct {
    char nome[10];
    int balcoes[3];
} tipoTicket;

tipoTicket urgencia = {"Urg�ncia",{3}};
tipoTicket marcada = {"Marcada",{1, 2, 3}};

typedef struct {
    char nome[20];
    char listaMedicos[4][50]; // Nome m�dicos
    float valorConsulta; // Valor Tabelado por consulta
} especialidade;

especialidade cardiologia = {"Cardiologia", {"Dr. Jo�o", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90};
especialidade dermatologia = {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Lu�s", "Dra. Paula"}, 25.50};
especialidade ortopedia = {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00};
especialidade pediatria = {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. J�lia", "Dr. Mateus"}, 15.50};

struct ticket {
    int id;
    tipoTicket tipo;       // Tipo do ticket (marcada ou urgencia)
    time_t dataCriacao;
    time_t dataAtendimento;
    int balcao;
    char medico[50];
    especialidade especialidade;
    int gabinete;
    float valorPagar;
};

especialidade listaEspecialidades[4] = {"cardiologia", "dermatologia", "ortopedia", "pediatria"};

// Fun��o para criar um ticket
struct ticket criarTicket(int id, tipoTicket tipo) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    novoTicket.dataCriacao = time(NULL); // Atribui o timestamp atual
    novoTicket.dataAtendimento = 0;     // N�o atendido ainda
    novoTicket.medico[0] = '\0';        // Inicializa como vazio
    novoTicket.especialidade = cardiologia;
    novoTicket.gabinete = 0;

    // Comparar o nome do tipo para decidir o balc�o
    if (strcmp(tipo.nome, "Urg�ncia") == 0) {
        novoTicket.balcao = 3; // Balc�o fixo para urg�ncias
        novoTicket.valorPagar = 0.0;
    } else {
        novoTicket.balcao = 1; // Balc�o padr�o para outros tipos (tempor�rio antes de randomizer)
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;
    }

    return novoTicket;
}

// Fun��o para exibir um ticket
void imprimirTicket(struct ticket t) {
    char buffer[20];
    struct tm *tm_info = localtime(&t.dataCriacao);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("ID: %d\n", t.id);
    printf("Tipo: %s\n", t.tipo.nome);
    printf("Data de Cria��o: %s\n", buffer);
    printf("Balc�o: %d\n\n", t.balcao);
}

void main(){
    setlocale(LC_ALL, ""); // Usa a configura��o de localidade do sistema
    // Defini��o de tipos de tickets
 //   tipoTicket marcada = {"Marcada", {1, 2, 3}};
 //   tipoTicket urgencia = {"Urg�ncia", {3}};

    // Criar tickets
    struct ticket t1 = criarTicket(1, marcada);
    struct ticket t2 = criarTicket(2, urgencia);

    // Imprimir resultados
    printf("Ticket 1 - Tipo: %s, Balc�o: %d, Custo: %.2f\n", t1.tipo.nome, t1.balcao, t1.valorPagar);
    printf("Ticket 2 - Tipo: %s, Balc�o: %d, Custo: %.2f\n", t2.tipo.nome, t2.balcao, t2.valorPagar);

}
