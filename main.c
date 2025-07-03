#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Autor{
    char *nome;
    char *instituicao;
};

typedef struct { //vetor dinamico de Autor
    struct Autor *autor;
    size_t size; //size = qnts pessoas tem no vetor
    size_t capacity; //capacity = qnts pessoas cabem no vetor
} Autores;

struct Livro{
    int identificador;
    char *titulo;
    Autores *autores;
    int ano;
    int edicao;
    char *editora;
};

typedef struct {
    struct Livro *livro;
    size_t size;
    size_t capacity;
} Livros;

struct Usuario {
    int identificador;
    char *nome;
    char *endereco; //endereco completo
    int telefone;
} Usuario;

typedef struct {
    struct Usuario *usuario;
    size_t size;
    size_t capacity;
} Usuarios;


typedef struct {
    struct tm data_inicial;
    struct tm data_final;
    int identificador_livro;
    int identificador_usuario;
} Reserva;

int rand_int() {
    return (rand() % 100000);
}

void inic_usuarios(Usuarios *u) {
    u->usuario = NULL;
    u->capacity = 0;
    u->size = 0;
}

int listar_usuarios(Usuarios *u, int id) {
    if (id == -1) { //se o identificador for passado como -1, a função lista todos os usuarios cadastrados
        for (int i = 0; i < u->size; i++) {
            printf("Usuario %d:\n", i+1);
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %d\n", u->usuario[i].telefone);
            printf("\n");
        }
        return 0;
    }
    else{
        for (int i = 0; i < u->size; i++) {
            if (u->usuario[i].identificador == id) {
                // printf("Usuario %d:\n", i+1);
                // printf("Identificador: %i\n", u->usuario[i].identificador);
                // printf("Nome: %s\n", u->usuario[i].nome);
                // printf("Endereco: %s\n", u->usuario[i].endereco);
                // printf("Telefone: %d\n", u->usuario[i].telefone);
                // printf("\n");
                return i;
            }
        }
        return -1;
    }
}

void inserir_usuario(Usuarios *u, char *nm, char *end, int tel) {
    int id = rand_int();
    if (listar_usuarios(u, id) == -1) { //verificar se ha usuario com ID selecionado
        if (u->capacity == u->size) { //verificar se ha memoria ja alocada, caso nao haja, alocar
            u->usuario = realloc(u->usuario, sizeof(Usuario) * (u->size + 1));
            u->capacity++;
        }
        u->usuario[u->size].identificador = id;
        u->usuario[u->size].nome = malloc(strlen(nm) + 1);
        strcpy(u->usuario[u->size].nome, nm);
        u->usuario[u->size].endereco = malloc(strlen(end) + 1);
        strcpy(u->usuario[u->size].endereco, end);
        u->usuario[u->size].telefone = tel;
        u->size++;
    }
    else {
        fprintf(stderr,"Erro: Id duplicado, tente novamente");
    }
}



void inic_livros(Livros *l) {
    l->livro = NULL;
    l->capacity =0;
    l->size = 0;
}



int main(void) {
    srand(time(NULL));
    Usuarios usuarios;
    inic_usuarios(&usuarios);
    inserir_usuario(&usuarios, "irineu", "rua sim pq sim", 31987321);
    inserir_usuario(&usuarios, "irineu2", "rua sim pq nao", 46165165);
    listar_usuarios(&usuarios, -1);
    return 0;
}