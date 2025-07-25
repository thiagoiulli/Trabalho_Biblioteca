#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef HAVE_GETLINE //código que faz a função geline funcionar em windows
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    *lineptr = NULL;
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stream) == NULL)
        return -1;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    len = strlen(buffer);
    *lineptr = realloc(*lineptr, len + 1);
    if (*lineptr == NULL)
        return -1;
    strcpy(*lineptr, buffer);
    *n = len + 1;
    return len;
}
#endif

struct Autor{
    char *nome;
    char *instituicao;
};//struct autor

typedef struct { //vetor dinamico de Autor
    struct Autor *autor;
    size_t size; //size = qnts pessoas tem no vetor
    size_t capacity; //capacity = qnts pessoas cabem no vetor
} Autores;

struct Livro{ //struct dos livros
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

struct Usuario { //struct dos usuarios
    int identificador;
    char *nome;
    char *endereco; //endereco completo
    long long telefone;
};

typedef struct {
    struct Usuario *usuario;
    size_t size;
    size_t capacity;
} Usuarios; //vetor dinamico de usuario

struct Reserva{ //struct das reservas
    struct tm data_inicial;
    struct tm data_final;
    int identificador_usuario;
    int identificador_livro;
};

typedef struct {
    struct Reserva *reserva;
    size_t size;
    size_t capacity;
} Reservas; //vetor dinamico da reserva

int rand_int() {
    return (rand() % 100000);//gera um numero aleatorio para fazer o ID da pessoa
}

void realloc_error() {
    printf("Realloc error!\n");
    exit(-404);
}

int tm_compare(const struct tm *a, const struct tm *b) { //compara datas
    if (a->tm_year != b->tm_year)
        return (a->tm_year > b->tm_year) ? 1 : -1; //retorna 1 se a é depois de b
    if (a->tm_mon != b->tm_mon)
        return (a->tm_mon > b->tm_mon) ? 1 : -1; // retorna -1 se b é depois de a
    if (a->tm_mday != b->tm_mday)
        return (a->tm_mday > b->tm_mday) ? 1 : -1; // compara primeiro ano, depois mes, e depois dia caso seja necessario
    return 0; // retorna 0 se a = b
}

void inic_usuarios(Usuarios *u) { //inicializa o vetor dinamico de usuarios, evitando Segmentation fault
    u->usuario = NULL;
    u->capacity = 0;
    u->size = 0;
}

void listar_usuarios(const Usuarios *u, const int id) {
    if (id == -1) { //se o identificador for passado como -1, a função lista todos os usuarios cadastrados
        for (int i = 0; i < u->size; i++) {
            printf("Usuario %d:\n", i+1);
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %lld\n", u->usuario[i].telefone);
            printf("\n");
        }
        return;
    }
    for (int i = 0; i < u->size; i++) { //se o id não for -1, ele procura entre todos os usuarios, para verificar se ele ex
        if (u->usuario[i].identificador == id) {
            printf("Identificador: %i\n", u->usuario[i].identificador);
            printf("Nome: %s\n", u->usuario[i].nome);
            printf("Endereco: %s\n", u->usuario[i].endereco);
            printf("Telefone: %lld\n", u->usuario[i].telefone);
            printf("\n");
            return;
        }
    }
    printf("Usuario %d: não encontrado\n", id);
}

int buscar_usuarios (const Usuarios *u, const int id){ //função que busca um usuario especifico
    for (int i = 0; i < u->size; i++) {
        if (u->usuario[i].identificador == id) {
            return i;
        }
    }
    return -1; //usuario nao encontrado
}

void inserir_usuario(Usuarios *u, const char *nm, const char *end, const long long tel) {
    const int id = rand_int();
    if (buscar_usuarios(u, id) == -1) { //verificar se ha usuario com ID selecionado
        if (u->capacity == u->size) { //verificar se ha memoria ja alocada, caso nao haja, alocar
            struct Usuario *tmp = realloc(u->usuario, sizeof(struct Usuario) * (u->size + 1));
            if (tmp == NULL) {
                realloc_error(); //finaliza se o realloc falhar
            }
            u->usuario = tmp;
            u->capacity = u->size + 1;
        }
        u->usuario[u->size].identificador = id; //copia o numero aleatorio para o id do usuario criado
        u->usuario[u->size].nome = malloc(strlen(nm) + 1);//aloca a memoria
        strcpy(u->usuario[u->size].nome, nm);//copia string de nome para o usuario criado 
        u->usuario[u->size].endereco = malloc(strlen(end) + 1);//aloca memoria
        strcpy(u->usuario[u->size].endereco, end);//copia string de endereço para o usuario criado
        u->usuario[u->size].telefone = tel;//copia o telefone para o usuario criado
        u->size++;//aumenta o tamanho do vetor, liberando espaço para mais um usuario
        printf("Usuario criado com id: %d\n", id);
    }
    else {
        inserir_usuario(u, nm, end, tel); //função recursiva que repete o id, caso ele seja gerado repetido
    }
}

void alterar_usuario(const Usuarios *u, const int id, const char *nm, const char *end, const long long tel) {
    int i = buscar_usuarios(u, id);
    if (i == -1) {
        printf("Sem usuario com id informado!\n");//verifica se usuario existe antes de alterar
        return;
    }
    if (strcmp(nm, "") != 0) {
        free(u->usuario[i].nome);//apaga o conteudo escrito anteriormente
        u->usuario[i].nome = malloc(strlen(nm) + 1);//aloca memoria novamente
        strcpy(u->usuario[i].nome, nm);//copia a string de nome para o usuario
    }
    if (strcmp(end, "") != 0) {
        free(u->usuario[i].endereco);//apaga o conteudo anterior
        u->usuario[i].endereco = malloc(strlen(end) + 1);//aloca memoria novamente
        strcpy(u->usuario[i].endereco, end);//copia a string de endereço para o usuario
    }
    if (tel != -1) {
        u->usuario[i].telefone = tel;//copia o novo telefone para o usuario
    }
}

void excluir_usuario(Usuarios *u, const int id) {
    int j = buscar_usuarios(u, id);
    
    if (j == -1) {
        printf("Usuário não encontrado!\n");//verifica se usuario existe antes de excluir
        return;
    }

    for (int i = j; i < u->size-1; i++) {
        u->usuario[i] = u->usuario[i+1];
    }
    u->size--;//reduz o parametro do tamanho do vetor
    struct Usuario *tmp = realloc(u->usuario, sizeof(struct Usuario) * (u->size)); //realoca o tamanho do vetor
    if (tmp == NULL) {
        realloc_error();
    }
    u->usuario = tmp;
    u->capacity = u->size;
}

void inic_autores(Autores *a){
    a->autor = NULL;
    a->capacity = 0;
    a->size = 0;
}

int listar_autores(const Autores *a, const char *nm){
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

int buscar_autores (const Autores *a, const char *nm){
    for (int i = 0; i < a->size; i++) {
        if (strcmp(a->autor[i].nome, nm) == 0) {
            return i;
        }
    }
    return -1;
}

void inserir_autores(Autores *a, const char *nm, const char *inst) {
    if (buscar_autores(a, nm) == -1) {
        if (a->capacity == a->size) {
            struct Autor *tmp = realloc(a->autor, sizeof(struct Autor) * (a->size + 1));
            if (tmp == NULL) {
                realloc_error();
            }
            a->autor = tmp;
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

void excluir_autores(Autores *a, const char *nm) {
    int j = buscar_autores(a, nm);
    
    if (j == -1) {
        printf("Autor não encontrado!\n");
        return;
    }
    
    for (int i = j; i < a->size-1; i++) {
        a->autor[i] = a->autor[i+1];
    }
    
    a->size--;
    struct Autor *tmp = realloc(a->autor, sizeof(struct Autor) * (a->size));
    if (tmp == NULL) {
        realloc_error();
    }
    a->autor = tmp;
    a->capacity = a->size;
}
    
void inic_livros(Livros *l) {
    l->livro = NULL;
    l->capacity = 0;
    l->size = 0;
}

int listar_livros(const Livros *l, const int id){
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

int buscar_livros (const Livros *l, const int id){
    for (int i = 0; i < l->size; i++) {
        if (l->livro[i].identificador == id) {
            return i;
        }
    }
    return -1;
}

void inserir_livros(Livros *l, char *t, Autores *a, const int ano, const int ed, char *edit){
    int id = rand_int();
    if (buscar_livros(l,id) == -1){
        if (l->size >= l->capacity){
            struct Livro *tmp = realloc(l->livro, sizeof(struct Livro) * (l->size + 1));
            if (tmp == NULL) {
                realloc_error();
            }
            l->livro = tmp;
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
        l->livro[l->size].autores->size = a->size;
        l->livro[l->size].autores->capacity = a->size;
        l->livro[l->size].autores->autor = malloc(sizeof(struct Autor) * a->size);
        for (int i = 0; i < l->livro[l->size].autores->size; i++) {
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

void alterar_livro(const Livros *l, const int id, const char *t, const char *edit, const int ano, const int ed) {
    const int i = buscar_livros(l, id);
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

void excluir_livros(Livros *l, const int id) {
    int j = buscar_livros(l, id);
    
    if (j == -1) {
        printf("Sem livro com id informado!\n");
        return;
    }

    for (int i = j; i < l->size-1; i++) {
        l->livro[i] = l->livro[i+1];
    }
    l->size--;
    struct Livro *tmp = realloc(l->livro, sizeof(struct Livro) * (l->size));
    if (tmp == NULL) {
        realloc_error();
    }
    l->livro = tmp;
    l->capacity = l->size;
}

void inic_reserva(Reservas *r) {
    r->reserva = NULL;
    r->capacity = 0;
    r->size = 0;
}

int buscar_reserva(const Reservas *r, const int idu, const int idl) {
    for (int i = 0; i < r->size; i++) {
        if (r->reserva[i].identificador_usuario == idu && r->reserva[i].identificador_livro == idl) {
            return i;
        }
    }
    return -1;
}

void listar_reservas(const Reservas *r, const int idu, const int idl) {
    if (idu == -1 || idl == -1) {
        for (int i = 0; i < r->size; i++) {
            printf("Data inicial da reserva: %d/%d/%d\n", r->reserva[i].data_inicial.tm_mday, r->reserva[i].data_inicial.tm_mon+1, r->reserva[i].data_inicial.tm_year);
            printf("Data final da reserva: %d/%d/%d\n", r->reserva[i].data_final.tm_mday, r->reserva[i].data_final.tm_mon+1, r->reserva[i].data_final.tm_year);
            printf("Id usuario: %d\n", r->reserva[i].identificador_usuario);
            printf("Id livro: %d\n", r->reserva[i].identificador_livro);
            printf("\n");
        }
        return;
    }
    for (int i = 0; i < r->size; i++) {
        if (r->reserva[i].identificador_usuario == idu && r->reserva[i].identificador_livro == idl) {
            printf("Data inicial da reserva: %d/%d/%d\n", r->reserva[i].data_inicial.tm_mday, r->reserva[i].data_inicial.tm_mon+1, r->reserva[i].data_inicial.tm_year);
            printf("Data final da reserva: %d/%d/%d\n", r->reserva[i].data_final.tm_mday, r->reserva[i].data_final.tm_mon+1, r->reserva[i].data_final.tm_year);
            printf("Id usuario: %d\n", r->reserva[i].identificador_usuario);
            printf("Id livro: %d\n", r->reserva[i].identificador_livro);
            printf("\n");
            return;
        }
    }
    printf("Reserva não encontrada para os IDs %d e %d!\n", idu, idl);
}

void inserir_reserva(Reservas *r, const struct tm di, const struct tm df, const int idu, const int idl) {
    int o = buscar_reserva(r, idu, idl);
    if (o == -1 && tm_compare(&df, &di) != -1) {
        if (r->capacity == r->size) {
            struct Reserva *tmp = realloc(r->reserva, sizeof(struct Reserva) * (r->size + 1));
            if (tmp == NULL) {
                realloc_error();
            }
            r->reserva = tmp;
            r->capacity = r->size + 1;
        }
        r->reserva[r->size].data_inicial = di;
        r->reserva[r->size].data_final = df;
        r->reserva[r->size].identificador_usuario = idu;
        r->reserva[r->size].identificador_livro = idl;
        r->size++;
    }
}

void alterar_reserva(const Reservas *r, const struct tm di, const struct tm df, const int idu, const int idl) {
    if (tm_compare(&df, &di) != -1) {
        int i = buscar_reserva(r, idu, idl);
        if (i == -1) {
            printf("Sem reserva com os IDs informados!\n");
            return;
        }
        if (di.tm_yday != -1) {
            r->reserva[i].data_inicial = di;
        }
        if (df.tm_yday != -1) {
            r->reserva[i].data_final = df;
        }
    }
    else {
        printf("Erro: data final maior que data inicial!\n");
    }
}

void excluir_reserva(Reservas *r, const int idu, const int idl) {
    int j = buscar_reserva(r, idu, idl);

    if (j == -1) {
        printf("Reserva não encontrada para este usuário e este livro!\n");
        return;
    }

    for (int i = j; i < r->size-1; i++) {
        r->reserva[i] = r->reserva[i+1];
    }
    r->size--;
    struct Reserva *tmp = realloc(r->reserva, sizeof(struct Reserva) * (r->size));
    if (tmp == NULL) {
        realloc_error();
    }
    r->reserva = tmp;
    r->capacity = r->size;
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
                long long tel, read;
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
                scanf("%lld", &tel);
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
                int nread;
                long long tell;
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
                    if (sscanf(telefone, "%lld", &tell) != 1) {
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

void menu_autores(const Livros *l){
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

void menu_relatorios(const Livros *l, const Usuarios *u, const Reservas *r){
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
                listar_reservas(r, -1, -1);
                break;
            case 4:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void menu_reservas(Reservas *r) {
    int n = 0, t;
    while (n == 0) {
        printf("Digite 1 para adicionar reserva\n");
        printf("Digite 2 para procurar reserva\n");
        printf("Digite 3 para listar todas as reservas\n");
        printf("Digite 4 para excluir reserva\n");
        printf("Digite 5 para alterar reserva\n");
        printf("Digite 6 para voltar\n");
        scanf("%d", &t);
        getchar();
        switch (t) {
            case 1:
                char *sdti1, *sdtf1;
                int idu1, idl1, read1;
                size_t size = 0;
                printf("Insira o id do usuário:\n");
                scanf("%d", &idu1);
                getchar();
                printf("Insira o id do livro:\n");
                scanf("%d", &idl1);
                getchar();
                struct tm di1 = {0}, df1 = {0};
                printf("Insira a data inicial da reserva(DD/MM/YYYY):\n");
                read1 = getline(&sdti1, &size, stdin);
                if (read1 != -1) {
                    if (sdti1[read1 - 1] == '\n') {
                        sdti1[read1 - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(sdti1);
                    free(sdtf1);
                    return;
                }
                if (sscanf(sdti1, "%d/%d/%d", &di1.tm_mday, &di1.tm_mon, &di1.tm_year) != 3) {
                    printf("Erro lendo stdin!\n");
                    free(sdti1);
                    free(sdtf1);
                    return;
                }
                di1.tm_mon--;
                size = 0;
                printf("Insira a data final da reserva(DD/MM/YYYY):\n");
                read1 = getline(&sdtf1, &size, stdin);
                if (read1 != -1) {
                    if (sdtf1[read1 - 1] == '\n') {
                        sdtf1[read1 - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(sdti1);
                    free(sdtf1);
                    return;
                }
                if (sscanf(sdtf1, "%d/%d/%d", &df1.tm_mday, &df1.tm_mon, &df1.tm_year) != 3) {
                    printf("Erro lendo stdin!\n");
                    free(sdti1);
                    free(sdtf1);
                    return;
                }
                df1.tm_mon--;
                inserir_reserva(r, di1, df1, idu1, idl1);
                free(sdti1);
                free(sdtf1);
                break;
            case 2:
                int idu2, idl2;
                printf("insira o id do usuario:\n");
                scanf("%d", &idu2);
                getchar();
                printf("insira o id do livro:\n");
                scanf("%d", &idl2);
                getchar();
                listar_reservas(r, idu2, idl2);
                break;
            case 3:
                listar_reservas(r, -1, -1);
                break;
            case 4:
                int idu4, idl4;
                printf("insira o id do usuario da reserva:\n");
                scanf("%d", &idu4);
                getchar();
                printf("insira o id do livro da reserva:\n");
                scanf("%d", &idl4);
                getchar();
                excluir_reserva(r, idu4, idl4);
                break;
            case 5:
                char *sdti5, *sdtf5;
                int idu5, idl5, read5;
                size_t len = 0;
                struct tm di5 = {0}, df5 = {0};
                printf("insira o id do usuario da reserva:\n");
                scanf("%d", &idu5);
                getchar();
                printf("insira o id do livro da reserva:\n");
                scanf("%d", &idl5);
                getchar();
                if (buscar_reserva(r, idu5, idl5) == -1) {
                    printf("Reserva com usuario e livro mencionado não encontrada!\n");
                    break;
                }
                printf("Insira a data inicial da reserva(DD/MM/YYYY) (enter para não alterar):\n");
                read5 = getline(&sdti5, &len, stdin);
                if (read5 != -1) {
                    if (sdti5[read5 - 1] == '\n') {
                        sdti5[read5 - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(sdti5);
                    free(sdtf5);
                    return;
                }
                if (strcmp(sdti5, "") != 0) {
                    if (sscanf(sdti5, "%d/%d/%d", &di5.tm_mday, &di5.tm_mon, &di5.tm_year) != 3) {
                        printf("Erro lendo stdin!\n");
                        free(sdti5);
                        free(sdtf5);
                        return;
                    }
                    di5.tm_mon--;
                }
                else {
                    di5.tm_yday = -1;
                }
                len = 0;
                printf("Insira a data final da reserva(DD/MM/YYYY) (enter para não alterar):\n");
                read5 = getline(&sdtf5, &len, stdin);
                if (read5 != -1) {
                    if (sdtf5[read5 - 1] == '\n') {
                        sdtf5[read5 - 1] = '\0';
                    }
                }
                else {
                    printf("Erro lendo stdin!\n");
                    free(sdti5);
                    free(sdtf5);
                    return;
                }
                if (strcmp(sdtf5, "") != 0) {
                    if (sscanf(sdtf5, "%d/%d/%d", &df5.tm_mday, &df5.tm_mon, &df5.tm_year) != 3) {
                        printf("Erro lendo stdin!\n");
                        free(sdti5);
                        free(sdtf5);
                        return;
                    }
                    df5.tm_mon--;
                }
                else {
                    df5.tm_yday = -1;
                }
                alterar_reserva(r, di5, df5, idu5, idl5);
                free(sdti5);
                free(sdtf5);
                break;
            case 6:
                return;
            default:
                printf("Numero invaldo, tente novamente\n");
                break;
        }
    }
}

void switch_menu(Usuarios *u, Livros *l, Reservas *r){
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
            menu_reservas(r);
            break;
        case 5:
            menu_relatorios(l, u, r);
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
    Livros livros;
    Reservas reservas;
    
    inic_reserva(&reservas);
    inic_usuarios(&usuarios);
    inic_livros(&livros);
    
    printf("Bem vindo a Biblioteca de Edson e Thiago\n\n");
    
    switch_menu(&usuarios, &livros, &reservas);
    
    return 0;
}