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
} Livros; //vetor dinamico de livro

struct Usuario {
    int identificador;
    char *nome;
    char *endereco; //endereco completo
    int telefone;
};

typedef struct {
    struct Usuario *usuario;
    size_t size;
    size_t capacity;
} Usuarios; //vetor dinamico de usuario

struct Reserva{
    struct tm data_inicial;
    struct tm data_final;
    int identificador_livro;
    int identificador_usuario;
};

typedef struct {
    struct Reserva *reserva;
    size_t size;
    size_t capacity;
} Reservas;

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
    for (int i = 0; i < u->size; i++) {
        if (u->usuario[i].identificador == id) {
            printf("Usuario %d:\n", i+1);
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %d\n", u->usuario[i].telefone);
            printf("\n");
            return i;
        }
    }
    return -1; 
}

int buscar_usuarios (Usuarios *u, int id){
    for (int i = 0; i < u->size; i++) {
        if (u->usuario[i].identificador == id) {
            return i;
        }
    }
    return -1; //usuario nao encontrado
}

void inserir_usuario(Usuarios *u, char *nm, char *end, int tel) {
    int id = rand_int();
    if (buscar_usuarios(u, id) == -1) { //verificar se ha usuario com ID selecionado
        if (u->capacity == u->size) { //verificar se ha memoria ja alocada, caso nao haja, alocar
            u->usuario = realloc(u->usuario, sizeof(struct Usuario) * (u->size + 1));
            u->capacity = u->size + 1;
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
        inserir_usuario(u, nm, end, tel); //função recursiva que repete o id, caso ele seja gerado repetido
    }
}

void excluir_usuario(Usuarios *u, int id) {
    int j = buscar_usuarios(u, id);
    for (int i = j; i < u->size-1; i++) {
        u->usuario[i] = u->usuario[i+1];
    }
    u->size--;
    u->usuario = realloc(u->usuario, sizeof(struct Usuario) * (u->size));
    u->capacity = u->size;
}

void inic_autores(Autores *a){
    a->autor = NULL;
    a->capacity = 0;
    a->size = 0;
}

int listar_autores(Autores *a, char *nm){
    if (strcmp(nm, "0") == 0) {
        for (int i = 0; i < a->size; i++){
            printf("Autor %d: %s\n", i, a->autor[i].nome);
            printf("Instituição: %s\n", a->autor[i].instituicao);
        }
        return 0;
    }
    for (int i = 0; i < a->size; i++) {
        if (strcmp(a->autor[i].nome, nm) == 0) {
            printf("Autor %d: %s\n", i, a->autor[i].nome);
            printf("Instituição: %s\n", a->autor[i].instituicao);
            return i;
        }
    }
    return -1;
}

int buscar_autores (Autores *a, char *nm){
    for (int i = 0; i < a->size; i++) {
        if (strcmp(a->autor[i].nome, nm) == 0) {
            return i;
        }
    }
    return -1;
}

void inserir_autores(Autores *a, char *nm, char *inst) {
    if (buscar_autores(a, nm) == -1) {
        if (a->capacity == a->size) {
            a->autor = realloc(a->autor, sizeof(struct Autor) * (a->size + 1));
            a->capacity = a->size + 1;
        }
        a->autor[a->size].nome = malloc(strlen(nm) + 1);
        strcpy(a->autor[a->size].nome, nm);
        a->autor[a->size].instituicao = malloc(strlen(inst) + 1);
        strcpy(a->autor[a->size].instituicao, inst);
        a->size++;
    }
    else {
        printf("Erro: Autor ja existe!\n");
    }
}

void excluir_autores(Autores *a, char *nm) {
    int j = buscar_autores(a, nm);
        for (int i = j; i < a->size-1; i++) {
            a->autor[i] = a->autor[i+1];
        }
        a->size--;
        a->autor = realloc(a->autor, sizeof(struct Autor) * (a->size));
        a->capacity = a->size;
    }
    
void inic_livros(Livros *l) {
    l->livro = NULL;
    l->capacity = 0;
    l->size = 0;
}

int listar_livros(Livros *l, int id){
    if (id == -1){
        for (int i = 0; i < l->size; i++){
            printf("Livro %i:\n", (i+1));
            printf("Identificador: %d\n", l->livro[i].identificador);
            printf("Titulo: %s\n", l->livro[i].titulo);
            listar_autores(l->livro[i].autores, "0");
            printf("Ano: %d\n", l->livro[i].ano);
            printf("Edição: %d\n", l->livro[i].edicao);
            printf("Editora: %s\n", l->livro[i].editora);
        }
        return 0;
    }
    for (int i = 0; i < l->size; i++) {
        if (l->livro[i].identificador == id) {
            printf("Livro %i:\n", (i+1));
            printf("Identificador: %d\n", l->livro[i].identificador);
            printf("Titulo: %s\n", l->livro[i].titulo);
            listar_autores(l->livro[i].autores, "0");
            printf("Ano: %d\n", l->livro[i].ano);
            printf("Edição: %d\n", l->livro[i].edicao);
            printf("Editora: %s\n", l->livro[i].editora);
            return i;
        }
    }
    return -1;
}

int buscar_livros (Livros *l, int id){
    for (int i = 0; i < l->size; i++) {
        if (l->livro[i].identificador == id) {
            return i;
        }
    }
    return -1;
}

void inserir_livros(Livros *l, char *t, Autores *a, int ano, int ed, char *edit){
    int id = rand_int();
    if (buscar_livros(l,id) == -1){
        if (l->size >= l->capacity){
            l->livro = realloc(l->livro, sizeof(struct Livro) * (l->size + 1));
            l->capacity = l->size + 1;
        }
        l->livro[l->size].identificador = id;
        l->livro[l->size].ano = ano;
        l->livro[l->size].edicao = ed;
        l->livro[l->size].titulo = malloc(strlen(t) + 1);
        strcpy(l->livro[l->size].titulo, t);
        l->livro[l->size].editora = malloc(strlen(edit) + 1);
        strcpy(l->livro[l->size].editora, edit);
        l->livro[l->size].autores = malloc(sizeof(Autores));
        //l->livro[l->size].autores->size = 0;
        l->livro[l->size].autores->size = a->size;
        l->livro[l->size].autores->capacity = a->size;
        l->livro[l->size].autores->autor = malloc(sizeof(struct Autor) * a->size);
        for (int i = 0; i < l->livro[l->size].autores->size; i++) {
            //inserir_autores(l->livro[l->size].autores, a->autor[i].nome, a->autor[i].instituicao);
            l->livro[l->size].autores->autor[i].nome = malloc(strlen(a->autor[i].nome) + 1);
            strcpy(l->livro[l->size].autores->autor[i].nome, a->autor[i].nome);
            l->livro[l->size].autores->autor[i].instituicao = malloc(strlen(a->autor[i].instituicao) + 1);
            strcpy(l->livro[l->size].autores->autor[i].instituicao, a->autor[i].instituicao);
        }
        l->size++;
    }
    else {
        inserir_livros(l, t, a, ano, ed, edit);
    }
}

void excluir_livros(Livros *l, int id) {
    int j = buscar_livros(l, id);
    for (int i = j; i < l->size-1; i++) {
        l->livro[i] = l->livro[i+1];
    }
    l->size--;
    l->livro = realloc(l->livro, sizeof(struct Livro) * (l->size));
    l->capacity = l->size;
}

void inic_reserva(Reservas *r) {
    r->reserva = NULL;
    r->capacity = 0;
    r->size = 0;
}

void menu() {
    printf("Digite 1 para acessar Usuarios\n");
    printf("Digite 2 para acessar Livros\n");
    printf("Digite 3 para reservas\n");
    printf("Digite 4 para relatorios\n");
    printf("Digite 5 para sair\n");
    printf("Digite 6 para repetir o menu\n");

    
}

void switch_menu(){
    printf("\n");
    menu();
    int n = 0, t;
    while (n == 0) {
        scanf("%d", &t);
        getchar();
        switch (t){
        
        case 1:
            menu_usuarios();
            break;
        
        case 2:
            menu_livros();
            break;

        case 3:
            menu_reservas();
            break;
        
        case 4:
            menu_relatorios();
            break;
        
        case 5:
            n = 1;  
            break;
        
        default:
            break;
        }
        break;
    }
}

void menu_usuarios(){
    printf("Digite 1 para adicionar usuarios\n");
    printf("Digite 2 para procurar usuarios\n");
    printf("Digite 3 para listar todos usuarios\n");
    printf("Digite 4 para excluir usuarios\n");
    printf("Digite 5 para alterar usuarios\n");
    printf("Digite 6 para voltar\n");

    int n = 0, t;
    while (n == 0) {
        scanf("%d", &t);
        getchar();
        switch (t){
        
        case 1:
            //inserir_usuario();
            break;
        
        case 2:
            //listar_usuarios();
            break;

        case 3:
            //listar_usuarios();
            break;
        
        case 4:
            //excluir_usuario();
            break;
        
        case 5:
            //alterar 
            break;
        
        case 6:
            switch_menu();
            return; 
        
        default:
            printf("Numero invaldo, tente novamente\n");
            break;
        }
        break;
    }
}

void menu_livros(){
    printf("Digite 1 para adicionar livros\n");
    printf("Digite 2 para procurar livros\n");
    printf("Digite 3 para listar todos livros\n");
    printf("Digite 4 para excluir livros\n");
    printf("Digite 5 para alterar livros\n");
    printf("Digite 6 para voltar\n");

    int n = 0, t;
    while (n == 0) {
        scanf("%d", &t);
        getchar();
        switch (t){
        
        case 1:
            //inserir_livros();
            break;
        
        case 2:
            //listar_livros();
            break;

        case 3:
            //listar_livros();
            break;
        
        case 4:
            //excluir_livros();
            break;
        
        case 5:
            //alterar 
            break;
        
        case 6:
            switch_menu();
            return; 
        
        default:
            printf("Numero invaldo, tente novamente\n");
            break;
        }
        break;
    }
}

void menu_relatorios(){
    printf("Digite 1 para listar todos os livros\n");
    printf("Digite 2 para listar todos os usuarios\n");
    printf("Digite 3 para listar todos os usuarios com reservas\n");
    printf("Digite 4 para voltar\n");

    int n = 0, t;
    while (n == 0) {
        scanf("%d", &t);
        getchar();
        switch (t){
        
        case 1:
            //listar_livros();
            break;
        
        case 2:
            //listar_usuarios();
            break;

        case 3:
            //listar_reservas();
            break;
        
        case 4:
            switch_menu();
            return; 
        
        default:
            printf("Numero invaldo, tente novamente\n");
            break;
        }
        break;
    }
}

int main(void) {
    srand(time(NULL));
    printf("Bem vindo a Biblioteca Hawkings\n");
    switch_menu();
    
    // Usuarios usuarios;
    //inic_usuarios(&usuarios);
    //inserir_usuario(&usuarios, "irineu", "rua sim pq sim", 31987321);
    //inserir_usuario(&usuarios, "irineu2", "rua sim pq nao", 46165165);
    // listar_usuarios(&usuarios, -1);
    Autores a;
    inic_autores(&a);
    inserir_autores(&a, "enzo enzo enzo", "ceferno");
    inserir_autores(&a, "enzo enzo", "ceferno");
    Livros l;
    inic_livros(&l);
    inserir_livros(&l, "eu e meu trabalho", &a, 2025, 1, "hebert richards");
    inserir_livros(&l, "eu e vc", &a, 2023, 4, "hebert-richards");
    listar_livros(&l, -1);
    



    return 0;
}