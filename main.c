#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
    int dia;
    int mes;
    int ano;
}NASCIMENTO;

typedef struct {
    int dia;
    int mes;
    int ano;
}ULTIMA_CONSULTA;

typedef struct {
    char nome[50];
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
}OBJETO_AVL;

typedef struct objeto {
    char nome[50];
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
    struct objeto *ant;
    struct objeto *prox;
}OBJETO_LISTA;

typedef struct node {
    OBJETO_AVL info;
    int altura;
    struct node *esq;
    struct node *dir;
}NODE;

typedef struct {
    OBJETO_LISTA *inicio;
    int tamanho;
}LISTA_DUPLAMENTE;

// Usa strcasecmp para saber qual nome vem antes, checando o ascii da letra (não considera case)
bool isBigger(const char *__s1, const char *__s2) {
    return strcasecmp(__s1, __s2) < 0;
}

char *strdup(const char *s) {
    char *d = malloc(strlen(s) + 1);
    if (d != NULL) strcpy(d, s);
    return d;
}


int retornaIdade(const int dia, const int mes, const int ano) {
    time_t timeNow = time(NULL);
    
    struct tm nascimento = {0};
    nascimento.tm_year = ano - 1900;  
    nascimento.tm_mon = mes - 1;  
    nascimento.tm_mday = dia;  

    // Converte o tempo de nascimento para time_t
    time_t nascimentoTime = mktime(&nascimento);

    long int idade = (long int)(timeNow - nascimentoTime) / (60*60*24*365);  

    return idade; 
}

int retornaDiasConsulta(const int dia, const int mes, const int ano) {
    time_t timeNow = time(NULL);
    
    struct tm consulta = {0};
    consulta.tm_year = ano - 1900;  
    consulta.tm_mon = mes - 1;  
    consulta.tm_mday = dia;  

    // Converte o tempo de nascimento para time_t
    time_t dataConsulta = mktime(&consulta);

    long int diasPassados = (long int)(timeNow - dataConsulta) / (60*60*24);  

    return diasPassados; 
}

// Operações da AVL
void inicializaAVL(NODE **raiz) {
    (*raiz) = NULL;
}

bool estahVaziaAVL(NODE **raiz) {
    return (*raiz) == NULL;
}

int maxAlturaFilhos(NODE **no) {
    int hE;
    int hD;
    hE = (*no)->esq != NULL ? (*no)->esq->altura : -1;
    hD = (*no)->dir != NULL ? (*no)->dir->altura : -1;

    return hE > hD ? hE : hD;
}

int calculaFB(NODE **no) {
    int hD, hE;
    hD = (*no)->dir != NULL ? (*no)->dir->altura : -1;
    hE = (*no)->esq != NULL ? (*no)->esq->altura : -1;

    return (hD - hE);
}

void rotacaoSimplesEsquerda(NODE **no) {
    NODE *aux = (*no)->dir;
    (*no)->dir = aux->esq;
    aux->esq = (*no);
    (*no) = aux;

    (*no)->esq->altura = 1 + maxAlturaFilhos(&(*no)->esq);
    (*no)->altura = 1 + maxAlturaFilhos(no);
}

void rotacaoSimplesDireita(NODE **no) {
    NODE *aux = (*no)->esq;
    (*no)->esq = aux->dir;
    aux->dir = (*no);
    (*no) = aux;

    (*no)->dir->altura = 1 + maxAlturaFilhos(&(*no)->dir);
    (*no)->altura = 1 + maxAlturaFilhos(no);
}

bool insereAVL(NODE **no, OBJETO_AVL cliente) {
    if(estahVaziaAVL(no) == true){
        (*no)= (NODE*) malloc(sizeof(NODE));
        
        if((*no) == NULL){ 
            printf("\nErro de alocação de memória \n\n");
            return EXIT_FAILURE;
        }

        strcpy((*no)->info.nome, cliente.nome);
        (*no)->info.sexo = cliente.sexo;
        (*no)->info.nascimento = cliente.nascimento;
        (*no)->info.ultimaConsulta = cliente.ultimaConsulta;
        (*no)->altura = 0;
        (*no)->esq = (*no)->dir = NULL;
        /*
            planejamento de implementação: tratar os dados, jogar para um objeto cliente (struct de objeto avl), e então copiar

            Explicação para o ordenamento da AVL, e da lista
            Cada caracter tem um decimal unico, onde A < Z, ou seja, o valor do ascii segue a ordem do maior ao menor, ou seja
            para avl os menores ficarão a esquerda, e os maiores a direita
        */
        return EXIT_SUCCESS;
    }

    if(!(strcmp(cliente.nome, (*no)->info.nome))) {
        return EXIT_FAILURE; // Não insere dois clientes com mesmo nome
    } 

    if(isBigger((*no)->info.nome, cliente.nome)) 
        return insereAVL(&((*no)->esq), cliente);
    else 
        return insereAVL(&((*no)->dir), cliente);

    (*no)->altura = 1 + maxAlturaFilhos(no);
    int fb = calculaFB(no);

    switch (fb) {
        case 2: // Rotação a esquerda
            switch (calculaFB((&(*no)->dir))) {
                case 0:
                case 1: // Simples esquerda
                    rotacaoSimplesEsquerda(no);
                    break;

                case -1: // dupla esquerda
                    rotacaoSimplesDireita(&(*no)->dir);
                    rotacaoSimplesEsquerda(no);
                    break;
            }
            break;
        
        case -2:
            switch (calculaFB((&(*no)->esq))) {
                case 0:
                case -1: // Simples direita
                    rotacaoSimplesDireita(no);
                    break;

                case 1: // dupla direita
                    rotacaoSimplesEsquerda(&(*no)->esq);
                    rotacaoSimplesDireita(no);
                    break;
            }
            break;

        default:
            break;
        }
    return EXIT_SUCCESS;
}

void buscarElementoAVL(NODE **no, char *nome) {
    if(estahVaziaAVL(no)) {
        printf("\nPaciente não encontrado: %s\n\n", nome);
        return;
    }

    if(!strcmp((*no)->info.nome, nome)) {
        printf("\n--------------------------------------");
        printf("\nPaciente encontrado: %s", nome);
        printf("\nIdade do paciente: %d", retornaIdade((*no)->info.nascimento.dia, (*no)->info.nascimento.mes, (*no)->info.nascimento.ano));
        printf("\nDias da última consulta: %d", retornaDiasConsulta((*no)->info.ultimaConsulta.dia, (*no)->info.ultimaConsulta.mes, (*no)->info.ultimaConsulta.ano));
        printf("\n--------------------------------------\n");
        return;
    }

    else {
        if(isBigger((*no)->info.nome, nome)) buscarElementoAVL(&(*no)->esq, nome);
        else buscarElementoAVL(&(*no)->dir, nome);
    }
}

void destruirAVL(NODE **no) {
    if(estahVaziaAVL(no)) return;

    destruirAVL(&(*no)->esq);
    destruirAVL(&(*no)->dir);
    free(*no);
    (*no) = NULL;
}

void posOrdem(NODE** no){
    if(estahVaziaAVL(no)==true) return;
    posOrdem(&(*no)->esq);
    posOrdem(&(*no)->dir);
    printf(" %s ", (*no)->info.nome);
}

// Operações de Lista Felipão

void inicializaLista(LISTA_DUPLAMENTE *lista){
    lista->inicio = NULL;
    lista->tamanho = 0;
}

void loadProgram(const char *filename, NODE **raizAVL) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return;
    }
    
    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char *start = linha + 1;
        char *end = strchr(start, '>');
        if(end) *end = '\0';

        OBJETO_AVL cliente;

        char *token = strtok(start, ",");
        if(!token) continue;
        strcpy(cliente.nome, token);

        if(!(token = strtok(NULL, ","))) continue;
        cliente.sexo = token[1];

        if(!(token = strtok(NULL, ","))) continue;
        sscanf(token + 1, "%d/%d/%d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);

        if(!(token = strtok(NULL, ","))) continue;
        sscanf(token + 1, "%d/%d/%d", &cliente.ultimaConsulta.dia, &cliente.ultimaConsulta.mes, &cliente.ultimaConsulta.ano);

        bool valid = (int) cliente.sexo == 'F';

        if(valid) {
            insereAVL(raizAVL, cliente); // insere apenas mulheres
        }

        // else implementa a lista para homens

    }
    
    fclose(file);
}

void geraSaida(NODE **no) {
    int a =0;
    //implementar
}

void menuLiz(NODE **no) {
    int choice;
    char nome[50];
    OBJETO_AVL clienteNova;
    char temp;

    do {
        printf("\n-----------------------------------------\n");
        printf("Menu pacientes:");
        printf("\n-----------------------------------------\n");
        printf("\n(1) Consultar paciente");
        printf("\n(2) Cadastrar paciente");
        printf("\n(3) Alterar cadastro de paciente");
        printf("\n(4) Sair do Sistema");
        printf("\n\nEscolha: ");

        scanf("%d", &choice);

        getchar();

        switch (choice) {
            case 1:
                printf("\nDigite o nome completo da paciente: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                buscarElementoAVL(no, nome);
                break;

            case 2:
                printf("\n-----------------------------------\n");
                printf("Menu de Cadastro:\n");
                printf("Digite o nome da paciente: ");
                
                fgets(clienteNova.nome, 50, stdin);
                clienteNova.nome[strcspn(clienteNova.nome, "\n")] = '\0';


                printf("\nDigite a data de nascimento (dd/mm/aaaa): ");
                scanf("%d/%d/%d", &clienteNova.nascimento.dia, &clienteNova.nascimento.mes, &clienteNova.nascimento.ano);
                if(clienteNova.nascimento.dia > 31 || clienteNova.nascimento.dia < 1 ||
                    clienteNova.nascimento.mes > 12 || clienteNova.nascimento.mes < 1 ||
                    clienteNova.nascimento.ano > 2025 || clienteNova.nascimento.ano < 1900
                ) {
                    printf("\nErro, data inválida.");
                    break;
                }
                getchar();

                printf("\nDigite a data da última consulta (dd/mm/aaaa): ");
                scanf("%d/%d/%d", &clienteNova.ultimaConsulta.dia, &clienteNova.ultimaConsulta.mes, &clienteNova.ultimaConsulta.ano);
                if(clienteNova.ultimaConsulta.dia > 31 || clienteNova.ultimaConsulta.dia < 1 ||
                    clienteNova.ultimaConsulta.mes > 12 || clienteNova.ultimaConsulta.mes < 1 ||
                    clienteNova.ultimaConsulta.ano > 2025 || clienteNova.ultimaConsulta.ano < 1970
                ) {
                    printf("\nErro, data inválida.");
                    break;
                }
                
                if((insereAVL(no, clienteNova))) printf("\nPaciente já cadastrada.\n");
                else printf("\nPaciente Cadastrada com Sucesso!\n");
                break;  
            case 3:
                // implementar
                break;

            case 4:
                printf("\nSaindo do sistema...\n");
                return;

            default:
                printf("\nOpção inválida! Escolha de 1 a 4.\n");
                break;
        }
    } while (1); 
}

void MenuInicial(NODE **no) {
    int choice = 0;
    printf("\n-----------------------------------------\n");
    printf("Menu inicial:");
    printf("\n-----------------------------------------\n");

    printf("\n\n(1) Pacientes da Liz");
    printf("\n(2) Pacientes do Moises");
    printf("\n(3) Sair do Sistema");

    printf("\n\nEscolha: ");
    scanf("%d", &choice);

    while(choice != -1) {
        switch(choice) {
            case 1: 
                menuLiz(no);
                choice = - 1;
                break;
            case 2: 
                // menuMoises();
                break;
            case 3:
                choice = -1;
                choice = -1;
                break;
            default:
                printf("\n Opção inválida");
                printf("\n\nEscolha: ");
                scanf("%d", &choice);
                break;
        }
    }
}

int main(int argc, char *argv[]) {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    


    if(argc != 2) {
        printf("Erro. Uso: %s <arquivo.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    NODE *clientesFeminias;

    inicializaAVL(&clientesFeminias);
    
    loadProgram(argv[1], &clientesFeminias);

    MenuInicial(&clientesFeminias);

    destruirAVL(&clientesFeminias);


    return EXIT_SUCCESS;
}