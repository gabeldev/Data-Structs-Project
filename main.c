#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


/*-----------------------------------------
               ESTRUTURAS DE DADOS
------------------------------------------*/
// Estrutura para datas de nascimento
typedef struct {
    int dia;
    int mes;
    int ano;
}NASCIMENTO;

// Estrutura para datas de última consulta
typedef struct {
    int dia;
    int mes;
    int ano;
}ULTIMA_CONSULTA;

// Estrutura base para armazenamento na AVL (Mulheres)
typedef struct {
    char nome[50];
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
}OBJETO_AVL;

// Estrutura para lista duplamente encadeada (Homens)
typedef struct objeto {
    char nome[50];
    char sexo;
    NASCIMENTO nascimento;
    ULTIMA_CONSULTA ultimaConsulta;
    struct objeto *ant;
    struct objeto *prox;
}OBJETO_LISTA;

// Nó da Árvore AVL
typedef struct node {
    OBJETO_AVL info;
    int altura;
    struct node *esq;
    struct node *dir;
}NODE;

// Lista Duplamente Encadeada
typedef struct {
    OBJETO_LISTA *inicio;
    int tamanho;
}LISTA_DUPLAMENTE;

bool insereLista(LISTA_DUPLAMENTE *lista, OBJETO_LISTA clienteLista); // Tive que definir para usar no Alterar Cadastro AVL

// Usa strcasecmp para saber qual nome vem antes, checando o ascii da letra (não considera case)
bool isBigger(const char *__s1, const char *__s2) {
    return strcasecmp(__s1, __s2) < 0;
}

// Função para retornar idade 
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

// Função retorno a quantos dias a consulta
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

// Max de altura
int maxAlturaFilhos(NODE **no) {
    int hE;
    int hD;
    hE = (*no)->esq != NULL ? (*no)->esq->altura : -1;
    hD = (*no)->dir != NULL ? (*no)->dir->altura : -1;

    return hE > hD ? hE : hD;
}

// Calcula fator de balanceamento
int calculaFB(NODE **no) {
    int hD, hE;
    hD = (*no)->dir != NULL ? (*no)->dir->altura : -1;
    hE = (*no)->esq != NULL ? (*no)->esq->altura : -1;

    return (hD - hE);
}

// Faz rotação simples esquerda
void rotacaoSimplesEsquerda(NODE **no) {
    NODE *aux = (*no)->dir;
    (*no)->dir = aux->esq;
    aux->esq = (*no);
    (*no) = aux;

    (*no)->esq->altura = 1 + maxAlturaFilhos(&(*no)->esq);
    (*no)->altura = 1 + maxAlturaFilhos(no);
}

// Faz rotação simples direita
void rotacaoSimplesDireita(NODE **no) {
    NODE *aux = (*no)->esq;
    (*no)->esq = aux->dir;
    aux->dir = (*no);
    (*no) = aux;

    (*no)->dir->altura = 1 + maxAlturaFilhos(&(*no)->dir);
    (*no)->altura = 1 + maxAlturaFilhos(no);
}

// Insere elementos mantendo o balanceamento AVL
bool insereAVL(NODE **no, OBJETO_AVL cliente) {
    if(estahVaziaAVL(no) == true){
        (*no)= (NODE*) malloc(sizeof(NODE));
        
        if((*no) == NULL){ 
            printf("\nErro de alocação de memória \n\n");
            return EXIT_FAILURE;
        }

        strcpy((*no)->info.nome, cliente.nome);
        (*no)->info.sexo = 'F';
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

// Deleta elemento da árvore AVL
void deletaElementoAVL(NODE **no, char *nome){
    if(!strcmp((*no)->info.nome, nome)){
        // Se é nó folha
        if((*no)->esq == NULL && (*no)->dir == NULL){
            free(*no);
            (*no) = NULL;
            return;
        }

        // Elemento com apenas um filho a esquerda
        if((*no)->esq != NULL && (*no)->dir == NULL){
            NODE *aux = (*no);
            (*no)=(*no)->esq;
            free(aux);
            return;
        }

        // Elemento com apenas um filho a esquerda
        if((*no)->esq == NULL && (*no)->dir != NULL){
            NODE *aux=(*no);
            (*no)=(*no)->dir;
            free(aux);
            return;
        }

        // Elemento a ser retirado possui 2 filhos
        // Quem substitui? Maior elemento da subárvore da esquerda
        if((*no)->esq != NULL && (*no)->dir != NULL){
            NODE *subEsq = (*no)->esq;
            while(subEsq->dir != NULL){
                subEsq = subEsq->dir;
            }
            strcpy((*no)->info.nome, subEsq->info.nome);
            deletaElementoAVL(&(*no)->esq, subEsq->info.nome); 
            return;
        }
    }
    else{
        if(isBigger((*no)->info.nome, nome)) deletaElementoAVL(&(*no)->esq, nome);
        else deletaElementoAVL(&(*no)->dir, nome);
    }
    //======================================
    (*no)->altura = 1 + maxAlturaFilhos(no);
    int fb = calculaFB(no);  //cálculo do fator de balanceamento do no

    // Balanceamento
    switch(fb){ 
        case  2: //rotação à esquerda            
            switch(calculaFB(&(*no)->dir)){ //consultar o fb do filho à direita
                case  0:;
                case  1: //rotação simples à esquerda
                    rotacaoSimplesEsquerda(no);
                    break;

                case -1: //rotação dupla à esquerda
                    rotacaoSimplesDireita(&(*no)->dir);
                    rotacaoSimplesEsquerda(no);
                    break;
            }            
            break;
        case -2: //rotação à direita
            switch(calculaFB(&(*no)->esq)){ //consultar o fb do filho à esquerda
                case  0:;
                case -1: //rotação simples à direita
                    rotacaoSimplesDireita(no);
                    break;

                case  1: //rotação dupla à direita
                    rotacaoSimplesEsquerda(&(*no)->esq);
                    rotacaoSimplesDireita(no);
                    break;
            }
            break;
    }
}

// Alterar Cadastro das pacientes (Caso de digitação errada, ou mudança de sexo e/ou nome)
void alterarCadastroAVL(NODE **no, LISTA_DUPLAMENTE *lista, char *nome) {
    OBJETO_LISTA clienteLista;
    int aux;
    char novoNome[50];
    int diaN, mesN, anoN;
    int diaC, mesC, anoC;
    if (estahVaziaAVL(no)) {
        printf("\nPaciente não encontrado: %s\n\n", nome);
        return;
    }
    if (!strcmp((*no)->info.nome, nome)) {
        
        do {
            printf("\nDigite o que gostaria de alterar\n");
            printf("1 - Alterar nome\n");
            printf("2 - Alterar sexo\n");
            printf("3 - Alterar data de nascimento\n");
            printf("4 - Alterar data da consulta\n");
            printf("5 - Sair");
            printf("\nOpção: ");
            scanf("%d", &aux);
            setbuf(stdin, NULL);
            switch(aux) {
                case 1:
                    printf("\nDigite o novo nome: ");
                    fgets(novoNome, 50, stdin);
                    novoNome[strcspn(novoNome, "\n")] = '\0';
                    strcpy((*no)->info.nome, novoNome);
                    printf("\nAlterado com sucesso!");
                    setbuf(stdin, NULL);
                    break;
                case 2: 
                    clienteLista = *(OBJETO_LISTA*)&(*no)->info; // Copia os dados de OBJETO_AVL para OBJETO_LISTA

                    insereLista(lista, clienteLista);
                    deletaElementoAVL(no, nome);
                    printf("\nAlterado com sucesso!");
                    break;
                case 3:
                    printf("\nDigite a nova data de nascimento: ");
                    scanf(" %d/%d/%d", &diaN, &mesN, &anoN);
                    if(diaN > 31 || diaN < 1 ||
                        mesN > 12 || mesN < 1 ||
                        anoN > 2025 || anoN < 1900
                    ) {
                        printf("\nErro, data inválida.");
                        break;
                    }
                    (*no)->info.nascimento.dia = diaN;
                    (*no)->info.nascimento.mes = mesN;
                    (*no)->info.nascimento.ano = anoN;
                    printf("\nAlterado com sucesso!");
                    break;
                case 4:
                    printf("\nDigite a nova data da consulta: ");
                    scanf(" %d/%d/%d", &diaC, &mesC, &anoC);
                    if(diaC > 31 || diaC < 1 ||
                        mesC > 12 || mesC < 1 ||
                        anoC > 2025 || anoC < 1900
                    ) {
                        printf("\nErro, data inválida.");
                        break;
                    }
                    (*no)->info.ultimaConsulta.dia = diaC;
                    (*no)->info.ultimaConsulta.mes = mesC;
                    (*no)->info.ultimaConsulta.ano = anoC;
                    printf("\nAlterado com sucesso!");
                    break;
                case 5:
                    break;
                default:
                    break;
            }
        }while(aux != 5);
        return;
    }
    if (isBigger((*no)->info.nome, nome)) {
        alterarCadastroAVL(&(*no)->esq, lista, nome);
    } else {
        alterarCadastroAVL(&(*no)->dir, lista, nome);
    }
}

void destruirAVL(NODE **no) {
    if(estahVaziaAVL(no)) return;

    destruirAVL(&(*no)->esq);
    destruirAVL(&(*no)->dir);
    free(*no);
    (*no) = NULL;
}

// Operações de Lista
void inicializaLista(LISTA_DUPLAMENTE *lista){
    lista->inicio = NULL;
    lista->tamanho = 0;
}

bool insereLista(LISTA_DUPLAMENTE *lista, OBJETO_LISTA cliente) {
    OBJETO_LISTA *novo = (OBJETO_LISTA*) malloc(sizeof(OBJETO_LISTA));
    if (!novo) return false;

    strcpy(novo->nome, cliente.nome);
    novo->sexo = 'M';
    novo->nascimento = cliente.nascimento;
    novo->ultimaConsulta = cliente.ultimaConsulta;
    novo->prox = NULL;
    novo->ant = NULL;

    if (!lista->inicio) {
        lista->inicio = novo;
    } else {
        OBJETO_LISTA *atual = lista->inicio;
        while (atual->prox && strcasecmp(novo->nome, atual->nome) < 0) {
            atual = atual->prox;
        }

        if (strcasecmp(novo->nome, atual->nome) < 0) {
            novo->prox = atual->prox;
            novo->ant = atual;
            if (atual->prox) atual->prox->ant = novo;
            atual->prox = novo;
        } else {
            novo->prox = atual;
            novo->ant = atual->ant;
            if (atual->ant) atual->ant->prox = novo;
            else lista->inicio = novo;
            atual->ant = novo;
        }
    }
    lista->tamanho++;
    return true;
}

OBJETO_LISTA* buscaLista(LISTA_DUPLAMENTE *lista, char *nome) {
    OBJETO_LISTA *atual = lista->inicio;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void removerDaLista(LISTA_DUPLAMENTE *lista, char *nome) {
    OBJETO_LISTA *atual = buscaLista(lista, nome);
    
    if (atual) {
        // Ajusta os ponteiros da lista
        if (atual->ant) {
            atual->ant->prox = atual->prox;
        } else {
            lista->inicio = atual->prox;
        }
        
        if (atual->prox) {
            atual->prox->ant = atual->ant;
        }
        
        free(atual);
        lista->tamanho--;
    }
}

void alterarCadastroLista(LISTA_DUPLAMENTE *lista, NODE **raizAVL, char *nome) {
    OBJETO_LISTA *atual = buscaLista(lista, nome);
    if (atual) {
        int aux;
        char novoNome[50];
        int diaN, mesN, anoN;
        int diaC, mesC, anoC;
        
        do {
            printf("\nDigite o que gostaria de alterar\n");
            printf("1 - Alterar nome\n");
            printf("2 - Alterar sexo\n");
            printf("3 - Alterar data de nascimento\n");
            printf("4 - Alterar data da consulta\n");
            printf("5 - Sair");
            printf("\nOpção: ");
            scanf("%d", &aux);
            setbuf(stdin, NULL);
            
            switch(aux) {
                case 1:
                    printf("\nDigite o novo nome: ");
                    fgets(novoNome, 50, stdin);
                    novoNome[strcspn(novoNome, "\n")] = '\0';
                    strcpy(atual->nome, novoNome);
                    printf("\nAlterado com sucesso!");
                    setbuf(stdin, NULL);
                    break;
                    
                case 2: {
                    OBJETO_AVL clienteAVL;
                    clienteAVL = *(OBJETO_AVL*)atual; // Copia os dados de OBJETO_LISTA para OBJETO_AVL
                    clienteAVL.sexo = 'F';
                    
                    insereAVL(raizAVL, clienteAVL);
                    removerDaLista(lista, nome);
                    printf("\nAlterado com sucesso!");
                    return;
                }
                    
                case 3:
                    printf("\nDigite a nova data de nascimento: ");
                    scanf(" %d/%d/%d", &diaN, &mesN, &anoN);
                    if(diaN > 31 || diaN < 1 ||
                       mesN > 12 || mesN < 1 ||
                       anoN > 2025 || anoN < 1900
                    ) {
                        printf("\nErro, data inválida.");
                        break;
                    }
                    atual->nascimento.dia = diaN;
                    atual->nascimento.mes = mesN;
                    atual->nascimento.ano = anoN;
                    printf("\nAlterado com sucesso!");
                    break;
                    
                case 4:
                    printf("\nDigite a nova data da consulta: ");
                    scanf(" %d/%d/%d", &diaC, &mesC, &anoC);
                    if(diaC > 31 || diaC < 1 ||
                       mesC > 12 || mesC < 1 ||
                       anoC > 2025 || anoC < 1900
                    ) {
                        printf("\nErro, data inválida.");
                        break;
                    }
                    atual->ultimaConsulta.dia = diaC;
                    atual->ultimaConsulta.mes = mesC;
                    atual->ultimaConsulta.ano = anoC;
                    printf("\nAlterado com sucesso!");
                    break;
                    
                case 5:
                    break;
                    
                default:
                    printf("\nOpção inválida!");
                    break;
            }
        } while(aux != 5);
    } else {
        printf("\nPaciente não encontrado.\n");
    }
}

void destruirLista(LISTA_DUPLAMENTE *lista) {
    OBJETO_LISTA *atual = lista->inicio;
    while (atual) {
        OBJETO_LISTA *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
}

// Carrega lê o txt o programa todo nas estruturas
void loadProgram(const char *filename, NODE **raizAVL, LISTA_DUPLAMENTE *lista) {
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
        OBJETO_LISTA clienteLista;

        char *token = strtok(start, ",");
        if(!token) continue;
        strcpy(cliente.nome, token);

        if(!(token = strtok(NULL, ","))) continue;
        cliente.sexo = token[1];

        if(!(token = strtok(NULL, ","))) continue;
        sscanf(token + 1, "%d/%d/%d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);

        if(!(token = strtok(NULL, ","))) continue;
        sscanf(token + 1, "%d/%d/%d", &cliente.ultimaConsulta.dia, &cliente.ultimaConsulta.mes, &cliente.ultimaConsulta.ano);

        bool valid = (int) cliente.sexo == 'F'; // Booleano control fazendo casting do char

        if(valid) {
            insereAVL(raizAVL, cliente); // insere apenas mulheres
        } else {
            clienteLista = *(OBJETO_LISTA*)&cliente; // Copia os dados de OBJETO_AVL para OBJETO_LISTA
            insereLista(lista, clienteLista);
        }
    }
    
    fclose(file);
}

// Escreve o arquivo de saida AVL
void escreveAVL(NODE *no, FILE *file) {
    if (!no) return;
    escreveAVL(no->dir, file);
    fprintf(file, "%s, %c, %02d/%02d/%04d, %02d/%02d/%04d\n",
            no->info.nome, no->info.sexo,
            no->info.nascimento.dia, no->info.nascimento.mes, no->info.nascimento.ano,
            no->info.ultimaConsulta.dia, no->info.ultimaConsulta.mes, no->info.ultimaConsulta.ano);
    escreveAVL(no->esq, file);
}

// Escreve o arquivo de saida Lista
void escreveLista(LISTA_DUPLAMENTE *lista, FILE *file) {
    if (!file) return;

    ftruncate(fileno(file), 0);
    rewind(file);
    
    OBJETO_LISTA *atual = lista->inicio;
    while (atual) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s, %c, %02d/%02d/%04d, %02d/%02d/%04d\n",
                atual->nome, atual->sexo,
                atual->nascimento.dia, atual->nascimento.mes, atual->nascimento.ano,
                atual->ultimaConsulta.dia, atual->ultimaConsulta.mes, atual->ultimaConsulta.ano);
                
        char *clean_buffer = buffer;
        while (*clean_buffer && (unsigned char)*clean_buffer > 127) {
            clean_buffer++;
        }
        
        fputs(clean_buffer, file);
        atual = atual->prox;
    }
}

// Gera a saida dos arquivos
void geraSaida(NODE **no, LISTA_DUPLAMENTE *lista) {
    FILE *fileLiz = fopen("saidaLiz.txt", "w");
    FILE *fileMoises = fopen("saidaMoises.txt", "w");
    if (!fileLiz || !fileMoises) {
        perror("Erro ao criar arquivos de saída");
        if (fileLiz) fclose(fileLiz);
        if (fileMoises) fclose(fileMoises);
        return;
    }

    fseek(fileLiz, 0, SEEK_SET);
    fseek(fileMoises, 0, SEEK_SET);

    escreveAVL(*no, fileLiz);
    fclose(fileLiz);

    escreveLista(lista, fileMoises);
    fclose(fileMoises);
    printf("Arquivos gerados com sucesso!\n");
}

// Menu da Liz
void menuLiz(NODE **no, LISTA_DUPLAMENTE *lista) {
    int choice;
    char nome[50];
    OBJETO_AVL clienteNova;

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
                scanf(" %d/%d/%d", &clienteNova.nascimento.dia, &clienteNova.nascimento.mes, &clienteNova.nascimento.ano);
                if(clienteNova.nascimento.dia > 31 || clienteNova.nascimento.dia < 1 ||
                    clienteNova.nascimento.mes > 12 || clienteNova.nascimento.mes < 1 ||
                    clienteNova.nascimento.ano > 2025 || clienteNova.nascimento.ano < 1900
                ) {
                    printf("\nErro, data inválida.");
                    break;
                }
                getchar();

                printf("\nDigite a data da última consulta (dd/mm/aaaa): ");
                scanf(" %d/%d/%d", &clienteNova.ultimaConsulta.dia, &clienteNova.ultimaConsulta.mes, &clienteNova.ultimaConsulta.ano);
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
                printf("\nDigite o nome completo da paciente para alteração: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                setbuf(stdin, NULL);
                alterarCadastroAVL(no, lista, nome);
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

// Menu do Moises
void menuMoises(LISTA_DUPLAMENTE *lista, NODE **no) {

    int choice;
    char nome[50];
    OBJETO_LISTA clienteNovo;

    do {
        printf("\n-----------------------------------------\n");
        printf("Menu pacientes de Moisés:");
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
                printf("\nDigite o nome completo do paciente: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                OBJETO_LISTA *paciente = buscaLista(lista, nome);
                if (paciente) {
                    printf("\n--------------------------------------");
                    printf("\nPaciente encontrado: %s", paciente->nome);
                    printf("\nIdade do paciente: %d", retornaIdade(paciente->nascimento.dia, paciente->nascimento.mes, paciente->nascimento.ano));
                    printf("\nDias da última consulta: %d", retornaDiasConsulta(paciente->ultimaConsulta.dia, paciente->ultimaConsulta.mes, paciente->ultimaConsulta.ano));
                    printf("\n--------------------------------------\n");
                } else {
                    printf("\nPaciente não encontrado.\n");
                }
                break;

            case 2:
                printf("\n-----------------------------------\n");
                printf("Menu de Cadastro:\n");
                printf("Digite o nome do paciente: ");
                fgets(clienteNovo.nome, 50, stdin);
                clienteNovo.nome[strcspn(clienteNovo.nome, "\n")] = '\0';
                clienteNovo.sexo = 'M';
                
                printf("\nDigite a data de nascimento (dd/mm/aaaa): ");
                scanf(" %d/%d/%d", &clienteNovo.nascimento.dia, &clienteNovo.nascimento.mes, &clienteNovo.nascimento.ano);
                getchar();
                
                printf("\nDigite a data da última consulta (dd/mm/aaaa): ");
                scanf(" %d/%d/%d", &clienteNovo.ultimaConsulta.dia, &clienteNovo.ultimaConsulta.mes, &clienteNovo.ultimaConsulta.ano);
                getchar();
                
                if (insereLista(lista, clienteNovo)) {
                    printf("\nPaciente Cadastrado com Sucesso!\n");
                } else {
                    printf("\nErro ao cadastrar paciente.\n");
                }
                break;

            case 3:
                printf("\nDigite o nome completo do paciente para alteração: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                alterarCadastroLista(lista, no, nome);
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

// Menu Inicial
void MenuInicial(NODE **no, LISTA_DUPLAMENTE *lista) {

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
                menuLiz(no, lista);
                choice = - 1;
                break;
            case 2: 
                menuMoises(lista, no);
                choice = -1;
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

// Main
int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Erro. Uso: %s <arquivo.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    NODE *clientesFemininas;
    LISTA_DUPLAMENTE clientesMasculinos;

    inicializaAVL(&clientesFemininas);
    inicializaLista(&clientesMasculinos);
    
    loadProgram(argv[1], &clientesFemininas, &clientesMasculinos);

    MenuInicial(&clientesFemininas, &clientesMasculinos);

    geraSaida(&clientesFemininas, &clientesMasculinos);

    destruirAVL(&clientesFemininas);
    destruirLista(&clientesMasculinos);

    return EXIT_SUCCESS;
}