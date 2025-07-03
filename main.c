#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Autor{
    char *nome;
    char *instituicao;
}Autor;

typedef struct { //vetor dinamico de Autor
    struct Autor *escritores;
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
} Livros; //vetor dinamico de livro

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
} Usuarios; //vetor dinamico de usuario


typedef struct {
    struct tm data_inicial;
    struct tm data_final;
    int identificador_livro;
    int identificador_usuario;
} Reserva;

int rand_int() {
    return (rand() % 100000);//gera um numero aleatorio para fazer o ID da pessoa
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
        inserir_usuarios(u, nm, end, tel); //função recursiva que repete o id, caso ele seja gerado repetido
    }
}

void inic_autores(Autores *a){
    a->escritores = NULL;
    a->capacity =0;
    a->size = 0;
}

int listar_autores(Autores *a){
    for (int i = 0; i < a->size; i++){
        printf("Autor %d: %s", i, a->escritores[i].nome);
        printf("Instituição: %s", a->escritores[i].instituicao);
    }
}

void inserir_autor(struct Autor *a, char *nm, char *inst){
    
    a->nome = malloc(strlen(nm) + 1);
    strcpy(a->nome, nm);
    
    a->instituicao = malloc(strlen(inst) + 1);
    strcpy(a->instituicao, inst);
    
}


/*void inserir_autores(Autores *a, char nm, char inst){ TEM Q OLHAR ISSO AQUI
    if (a->capacity == a->size) {
        a->escritores = realloc(a->escritores, sizeof(struct Autor) * (a->size + 1));
        a->capacity++;
    }

    inserir_autor(&a->escritores[a->size], nm, inst);
    a->size++;
}*/

void inic_livros(Livros *l) {
    l->livro = NULL;
    l->capacity =0;
    l->size = 0;
}

int listar_livros(Livros *l, int id){
    if (id == -1){
        for (int i = 0; i < l->size; i++){
            printf("Livro %i:\n", (i+1));
            printf("Identificador: %d\n", id);
            printf("Titulo: %s", l->livro[i].titulo);
            //printf("Autores")
            printf("Ano: %d", l->livro[i].ano);
            printf("Edição: %d", l->livro[i].edicao);
            printf("Editora: %s", l->livro[i].editora);

        }
    }

    else {
        for (int i = 0; i < l->size; i++) {
            if (l->livro[i].identificador == id) {
                //printf("Livro %i:\n", (i+1));
                //printf("Identificador: %d\n", id);
                //printf("Titulo: %s", l->livro[i].titulo);
                //printf("Autores")
                //printf("Ano: %d", l->livro[i].ano);
                //printf("Edição: %d", l->livro[i].edicao);
                //printf("Editora: %s", l->livro[i].editora);
                return i;
            }
        }
        return -1;
    }

}




int main(void) {
    srand(time(NULL));
    Usuarios usuarios;
    Autores autores;
    inic_usuarios(&usuarios);
    inic_autores(&autores);
    inserir_usuario(&usuarios, "irineu", "rua sim pq sim", 31987321);
    inserir_usuario(&usuarios, "irineu2", "rua sim pq nao", 46165165);
    listar_usuarios(&usuarios, -1);
    return 0;
}