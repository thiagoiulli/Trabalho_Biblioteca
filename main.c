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

void listar_usuarios(Usuarios *u, int id) {
    if (id == -1) { //se o identificador for passado como -1, a função lista todos os usuarios cadastrados
        for (int i = 0; i < u->size; i++) {
            printf("Usuario %d:\n", i+1);
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %d\n", u->usuario[i].telefone);
            printf("\n");
        }
        return;
    }
    for (int i = 0; i < u->size; i++) {
        if (u->usuario[i].identificador == id) {
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %d\n", u->usuario[i].telefone);
            printf("\n");
            return;
        }
    }
    printf("Usuario %d: não encontrado\n", id);
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
        printf("Usuario criado com id: %d\n", id);
    }
    else {
        inserir_usuario(u, nm, end, tel); //função recursiva que repete o id, caso ele seja gerado repetido
    }
}

void alterar_usuario(Usuarios *u, int id, char *nm, char *end, int tel) {
    int i = buscar_usuarios(u, id);
    if (i == -1) {
        printf("Sem usuario com id informado!\n");
        return;
    }
    if (strcmp(nm, "") != 0) {
        free(u->usuario[i].nome);
        u->usuario[i].nome = malloc(strlen(nm) + 1);
        strcpy(u->usuario[i].nome, nm);
    }
    if (strcmp(end, "") != 0) {
        free(u->usuario[i].endereco);
        u->usuario[i].endereco = malloc(strlen(end) + 1);
        strcpy(u->usuario[i].endereco, end);
    }
    if (tel != -1) {
        u->usuario[i].telefone = tel;
    }
}

void excluir_usuario(Usuarios *u, int id) {
    int j = buscar_usuarios(u, id);
    
    if (j == -1) {
        printf("Usuário não encontrado!\n");
        return;
    }

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
            printf("Autor %d: %s\n", i+1, a->autor[i].nome);
            printf("Instituição: %s\n", a->autor[i].instituicao);
        }
        printf("\n");
        return 0;
    }
    for (int i = 0; i < a->size; i++) {
        if (strcmp(a->autor[i].nome, nm) == 0) {
            printf("Autor %d: %s\n", i+1, a->autor[i].nome);
            printf("Instituição: %s\n", a->autor[i].instituicao);
            printf("\n");
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
    
    if (j == -1) {
        printf("Autor não encontrado!\n");
        return;
    }
    
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
            printf("\n");
        }
        return 0;
    }
    for (int i = 0; i < l->size; i++) {
        if (l->livro[i].identificador == id) {
            printf("Identificador: %d\n", l->livro[i].identificador);
            printf("Titulo: %s\n", l->livro[i].titulo);
            listar_autores(l->livro[i].autores, "0");
            printf("Ano: %d\n", l->livro[i].ano);
            printf("Edição: %d\n", l->livro[i].edicao);
            printf("Editora: %s\n", l->livro[i].editora);
            printf("\n");
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
        printf("Livro criado com id: %d\n", id);
    }
    else {
        inserir_livros(l, t, a, ano, ed, edit);
    }
}

void alterar_livro(Livros *l, int id, char *t, char *edit, int ano, int ed) {
    int i = buscar_livros(l, id);
    if (i == -1) {
        printf("Sem livro com id informado!\n");
        return;
    }
    if (strcmp(t, "") != 0) {
        free(l->livro[i].titulo);
        l->livro[i].titulo = malloc(strlen(t) + 1);
        strcpy(l->livro[i].titulo, t);
    }
    if (strcmp(edit, "") != 0) {
        free(l->livro[i].editora);
        l->livro[i].editora = malloc(strlen(edit) + 1);
        strcpy(l->livro[i].editora, edit);
    }
    if (ano != -1) {
        l->livro[i].ano = ano;
    }
    if (ed != -1) {
        l->livro[i].edicao = ed;
    }
}

void excluir_livros(Livros *l, int id) {
    int j = buscar_livros(l, id);
    
    if (j == -1) {
        printf("Sem livro com id informado!\n");
        return;
    }

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

void menu_usuarios(Usuarios *u){
    int n = 0, t;
    while (n == 0) {
        printf("Digite 1 para adicionar usuarios\n");
        printf("Digite 2 para procurar usuarios\n");
        printf("Digite 3 para listar todos usuarios\n");
        printf("Digite 4 para excluir usuarios\n");
        printf("Digite 5 para alterar usuarios\n");
        printf("Digite 6 para voltar\n");
        scanf("%d", &t);
        getchar();
        switch (t){
            case 1:
                char *nm, *end;
                int tel, read;
                size_t size = 0;
                printf("Insira o nome do usuário:\n");
                read = getline(&nm, &size, stdin);
                if (read != -1) {
                    if (nm[read - 1] == '\n') {
                        nm[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nm);
                    free(end);
                    return;
                }
                size = 0;
                printf("Insira o endereço do usuário:\n");
                read = getline(&end, &size, stdin);
                if (read != -1) {
                    if (end[read - 1] == '\n') {
                        end[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nm);
                    free(end);
                    return;
                }
                printf("Insira o telefone do usuário:\n");
                scanf("%d", &tel);
                getchar();
                inserir_usuario(u, nm, end, tel);
                free(nm);
                free(end);
                break;
            case 2:
                int id;
                printf("insira o id do usuario:\n");
                scanf("%d", &id);
                getchar();
                listar_usuarios(u, id);
                break;
            case 3:
                listar_usuarios(u, -1);
                break;
            case 4:
                int id4;
                printf("insira o id do usuario para ser excluido:\n");
                scanf("%d", &id4);
                getchar();
                excluir_usuario(u, id4);
                break;
            case 5:
                int id5;
                char *nome, *endereco, *telefone;
                int nread, tell;
                size_t len = 0;
                printf("digite o id do usuário a ser alterado:\n");
                scanf("%d", &id5);
                getchar();
                if (buscar_usuarios(u, id5) == -1) {
                    printf("Usuario não encontrado!\n");
                    break;
                }
                printf("Digite o nome alterado (enter para não alterar):\n");
                nread = getline(&nome, &len, stdin);
                if (nread != -1) {
                    if (nome[nread - 1] == '\n') {
                        nome[nread - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nome);
                    free(endereco);
                    return;
                }
                len = 0;
                printf("Digite o endereço alterado (enter para não alterar):\n");
                nread = getline(&endereco, &len, stdin);
                if (nread != -1) {
                    if (endereco[nread - 1] == '\n') {
                        endereco[nread - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nome);
                    free(endereco);
                    return;
                }
                len = 0;
                printf("Digite o telefone alterado (enter para não alterar):\n");
                nread = getline(&telefone, &len, stdin);
                if (nread != -1) {
                    if (telefone[nread - 1] == '\n') {
                        telefone[nread - 1] = '\0';
                    }
                    if (sscanf(telefone, "%d", &tell) != 1) {
                        tell = -1;
                    }
                }
                alterar_usuario(u, id5, nome, endereco, tell);
                free(nome);
                free(endereco);
                break;
            case 6:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void menu_livros(Livros *l){
    int n = 0, t;
    while (n == 0) {
        printf("Digite 1 para adicionar livros\n");
        printf("Digite 2 para procurar livros\n");
        printf("Digite 3 para listar todos livros\n");
        printf("Digite 4 para excluir livros\n");
        printf("Digite 5 para alterar livros\n");
        printf("Digite 6 para voltar\n");
        scanf("%d", &t);
        getchar();
        switch (t){
            case 1:
                char *titulo, *editora;
                int edicao, ano, Nautores, read;
                size_t size = 0;
                printf("Insira o titulo do livro:\n");
                read = getline(&titulo, &size, stdin);
                if (read != -1) {
                    if (titulo[read - 1] == '\n') {
                        titulo[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(titulo);
                    free(editora);
                    return;
                }
                size = 0;
                printf("Insira a editora do livro:\n");
                read = getline(&editora, &size, stdin);
                if (read != -1) {
                    if (editora[read - 1] == '\n') {
                        editora[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(titulo);
                    free(editora);
                    return;
                }
                printf("Insira o ano do livro:\n");
                scanf("%d", &ano);
                getchar();
                printf("Insira a edição do livro:\n");
                scanf("%d", &edicao);
                getchar();
                printf("Digite quantos autores o livro tem:\n");
                scanf("%d", &Nautores);
                getchar();
                Autores autores;
                inic_autores(&autores);
                for (int i = 0; i < Nautores; i++) {
                    char *nm, *inst;
                    size = 0;
                    printf("Digite o nome do autor %d:\n", i+1);
                    read = getline(&nm, &size, stdin);
                    if (read != -1) {
                        if (nm[read - 1] == '\n') {
                            nm[read - 1] = '\0';
                        }
                    }
                    else {
                        printf("Erro lendo stdin!\n");
                        free(nm);
                        free(inst);
                        free(titulo);
                        free(editora);
                        return;
                    }
                    size = 0;
                    printf("Digite a instituição do autor %d:\n", i+1);
                    read = getline(&inst, &size, stdin);
                    if (read != -1) {
                        if (inst[read - 1] == '\n') {
                            inst[read - 1] = '\0';
                        }
                    }
                    else {
                        printf("Erro lendo stdin!\n");
                        free(nm);
                        free(inst);
                        free(titulo);
                        free(editora);
                        return;
                    }
                    inserir_autores(&autores, nm, inst);
                    free(nm);
                    free(inst);
                }
                inserir_livros(l, titulo, &autores, ano, edicao, editora);
                free(titulo);
                free(editora);
                break;
            case 2:
                int id2;
                printf("insira o id do livro:\n");
                scanf("%d", &id2);
                getchar();
                listar_livros(l, id2);
                break;
            case 3:
                listar_livros(l, -1);
                break;
            case 4:
                int id4;
                printf("insira o id do usuario para ser excluido:\n");
                scanf("%d", &id4);
                getchar();
                excluir_livros(l, id4);
                break;
            case 5:
                int id5;
                char *tit, *edit, *ano5, *edic5;
                int nread, an5, edic;
                size_t len = 0;
                printf("digite o id do livro a ser alterado:\n");
                scanf("%d", &id5);
                getchar();
                if (buscar_livros(l, id5) == -1) {
                    printf("Sem livro com id informado!\n");
                    break;
                }
                printf("Digite o titulo alterado (enter para não alterar):\n");
                nread = getline(&tit, &len, stdin);
                if (nread != -1) {
                    if (tit[nread - 1] == '\n') {
                        tit[nread - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(tit);
                    free(edit);
                    free(ano5);
                    free(edic5);
                    return;
                }
                len = 0;
                printf("Digite a editora alterada (enter para não alterar):\n");
                nread = getline(&edit, &len, stdin);
                if (nread != -1) {
                    if (edit[nread - 1] == '\n') {
                        edit[nread - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(tit);
                    free(edit);
                    free(ano5);
                    free(edic5);
                    return;
                }
                len = 0;
                printf("Digite o ano alterado (enter para não alterar):\n");
                nread = getline(&ano5, &len, stdin);
                if (nread != -1) {
                    if (ano5[nread - 1] == '\n') {
                        ano5[nread - 1] = '\0';
                    }
                    if (sscanf(ano5, "%d", &an5) != 1) {
                        an5 = -1;
                    }
                }
                len = 0;
                printf("Digite a edição alterado (enter para não alterar):\n");
                nread = getline(&edic5, &len, stdin);
                if (nread != -1) {
                    if (edic5[nread - 1] == '\n') {
                        edic5[nread - 1] = '\0';
                    }
                    if (sscanf(edic5, "%d", &edic) != 1) {
                        edic = -1;
                    }
                }
                alterar_livro(l, id5, tit, edit, an5, edic);
                free(tit);
                free(edit);
                free(ano5);
                free(edic5);
                break;
            case 6:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void menu_autores(Livros *l){
    int n = 0, t, id;
    printf("Digite o id do livro a ser usado:\n");
    scanf("%d", &id);
    getchar();
    if (buscar_livros(l, id) == -1) {
        printf("Sem livro com id informado!\n");
        return;
    }
    int i = buscar_livros(l, id);
    while (n == 0) {
        printf("Digite 1 para adicionar autores\n");
        printf("Digite 2 para listar todos autores\n");
        printf("Digite 3 para excluir autores\n");
        printf("Digite 4 para alterar o id do livro usado\n");
        printf("Digite 5 para voltar\n");
        scanf("%d", &t);
        getchar();
        switch (t){
            case 1:
                char *nm, *inst;
                int read;
                size_t size = 0;
                printf("Insira o nome do autor:\n");
                read = getline(&nm, &size, stdin);
                if (read != -1) {
                    if (nm[read - 1] == '\n') {
                        nm[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nm);
                    free(inst);
                    return;
                }
                size = 0;
                printf("Insira a instituição do autor:\n");
                read = getline(&inst, &size, stdin);
                if (read != -1) {
                    if (inst[read - 1] == '\n') {
                        inst[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nm);
                    free(inst);
                    return;
                }
                inserir_autores(l->livro[i].autores, nm, inst);
            case 2:
                listar_autores(l->livro[i].autores,"0");
                break;
            case 3:
                char *nm4;
                printf("insira o nome do autor para ser excluido:\n");
                read = getline(&nm4, &size, stdin);
                if (read != -1) {
                    if (nm4[read - 1] == '\n') {
                        nm4[read - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(nm4);
                    return;
                }
                excluir_autores(l->livro[i].autores, nm4);
                break;
            case 4:
                printf("Digite o id do novo livro a ser utilizado\n");
                scanf("%d", &id);
                getchar();
                if (buscar_livros(l, id) == -1) {
                    printf("Sem livro com id informado!\n");
                    return;
                }
                i = buscar_livros(l, id);
                break;
            case 5:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void menu_relatorios(Livros *l, Usuarios *u){
    int n = 0, t;
    while (n == 0) {
        printf("Digite 1 para listar todos os livros\n");
        printf("Digite 2 para listar todos os usuarios\n");
        printf("Digite 3 para listar todos os usuarios com reservas\n");
        printf("Digite 4 para voltar\n");
        scanf("%d", &t);
        getchar();
        switch (t){
            case 1:
                listar_livros(l, -1);
                break;
            case 2:
                listar_usuarios(u, -1);
                break;
            case 3:
                //listar_reservas();
                break;
            case 4:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void switch_menu(Usuarios *u, Livros *l){
    int n = 0, t;
    while (n == 0) {
        printf("Digite 1 para acessar Usuarios\n");
        printf("Digite 2 para acessar Livros\n");
        printf("Digite 3 para acessar Autores\n");
        printf("Digite 4 para reservas\n");
        printf("Digite 5 para relatorios\n");
        printf("Digite 6 para sair\n");
        scanf("%d", &t);
        getchar();
        switch (t){
        case 1:
            menu_usuarios(u);
            break;
        case 2:
            menu_livros(l);
            break;
        case 3:
            menu_autores(l);
            break;
        case 4:
            //menu_reserva();
            break;
        case 5:
            menu_relatorios(l, u);
            break;
        case 6:
            return;
        default:
            printf("numero invalido, tente novamente\n");
            break;
        }
    }
}

int main(void) {
    srand(time(NULL));
    Usuarios usuarios;
    inic_usuarios(&usuarios);
    Livros livros;
    inic_livros(&livros);
    printf("Bem vindo a Biblioteca Hawkings\n");
    switch_menu(&usuarios, &livros);
    //inserir_usuario(&usuarios, "irineu", "rua sim pq sim", 31987321);
    //inserir_usuario(&usuarios, "irineu2", "rua sim pq nao", 46165165);
    // listar_usuarios(&usuarios, -1);
    // Autores a;
    // inic_autores(&a);
    // inserir_autores(&a, "enzo enzo enzo", "ceferno");
    // inserir_autores(&a, "enzo enzo", "ceferno");
    // Livros l;
    // inic_livros(&l);
    // inserir_livros(&l, "eu e meu trabalho", &a, 2025, 1, "hebert richards");
    // inserir_livros(&l, "eu e vc", &a, 2023, 4, "hebert-richards");
    // listar_livros(&l, -1);
    return 0;
}