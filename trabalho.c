#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para time_t e manipulação de datas
#include <locale.h>


// Declaração e inicialização de structs globais

typedef struct {
    char nome[10];
    int balcoes[3];
} tipoTicket;

tipoTicket urgencia = {"Urgência",{3}};
tipoTicket marcada = {"Marcada",{1, 2, 3}};

typedef struct {
    char nome[20];
    char listaMedicos[4][50]; // Nome médicos
    float valorConsulta; // Valor Tabelado por consulta
} especialidade;

especialidade cardiologia = {"Cardiologia", {"Dr. João", "Dra. Maria", "Dr. Silva", "Dra. Clara"}, 85.90};
especialidade dermatologia = {"Dermatologia", {"Dr. Pedro", "Dra. Ana", "Dr. Luís", "Dra. Paula"}, 25.50};
especialidade ortopedia = {"Ortopedia", {"Dr. Ricardo", "Dra. Vanessa", "Dr. Carlos", "Dra. Sofia"}, 40.00};
especialidade pediatria = {"Pediatria", {"Dra. Helena", "Dr. Gabriel", "Dra. Júlia", "Dr. Mateus"}, 15.50};

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

// Função para criar um ticket
struct ticket criarTicket(int id, tipoTicket tipo) {
    struct ticket novoTicket;
    novoTicket.tipo = tipo;
    novoTicket.dataCriacao = time(NULL); // Atribui o timestamp atual
    novoTicket.dataAtendimento = 0;     // Não atendido ainda
    novoTicket.medico[0] = '\0';        // Inicializa como vazio
    novoTicket.especialidade = cardiologia;
    novoTicket.gabinete = 0;

    // Comparar o nome do tipo para decidir o balcão
    if (strcmp(tipo.nome, "Urgência") == 0) {
        novoTicket.balcao = 3; // Balcão fixo para urgências
        novoTicket.valorPagar = 0.0;
    } else {
        novoTicket.balcao = 1; // Balcão padrão para outros tipos (temporário antes de randomizer)
        novoTicket.valorPagar = novoTicket.especialidade.valorConsulta;
    }

    return novoTicket;
}

// Função para exibir um ticket
void imprimirTicket(struct ticket t) {
    char buffer[20];
    struct tm *tm_info = localtime(&t.dataCriacao);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("ID: %d\n", t.id);
    printf("Tipo: %s\n", t.tipo.nome);
    printf("Data de Criação: %s\n", buffer);
    printf("Balcão: %d\n\n", t.balcao);
}

void main(){
    setlocale(LC_ALL, ""); // Usa a configuração de localidade do sistema
    // Definição de tipos de tickets
 //   tipoTicket marcada = {"Marcada", {1, 2, 3}};
 //   tipoTicket urgencia = {"Urgência", {3}};

    // Criar tickets
    struct ticket t1 = criarTicket(1, marcada);
    struct ticket t2 = criarTicket(2, urgencia);

    // Imprimir resultados
    printf("Ticket 1 - Tipo: %s, Balcão: %d, Custo: %.2f\n", t1.tipo.nome, t1.balcao, t1.valorPagar);
    printf("Ticket 2 - Tipo: %s, Balcão: %d, Custo: %.2f\n", t2.tipo.nome, t2.balcao, t2.valorPagar);

}
