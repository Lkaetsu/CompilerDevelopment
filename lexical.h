#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum simbolosReservados {sprograma, sinicio, sfim, sprocedimento, sfuncao,
                         sse, sentao, ssenao, senquanto, sfaca, satribuicao,
                         sescreva, sleia, svar, sinteiro, sbooleano,
                         sidentificador, snumero, sponto, sponto_virgula,
                         svirgula, sabre_parenteses, sfecha_parenteses,
                         smaior, smaiorig, sig, smenor, smenorig, sdif,
                         smais, smenos, smult, sdiv, se, sou, snao,
                         sdoispontos, sverdadeiro, sfalso, serro};

enum identifierTypes {tnomedeprograma, tvariavel, tinteiro, tbooleano,
                      tprocedimento, tfuncaointeiro, tfuncaobooleana};

typedef struct token {
    char *lexema;
    int simbolo;
    struct token *next;
}token;

typedef struct identifier {
    char *lexema;
    int type;
    char *scope;
    void *address;
} identifier;

typedef struct Stack {
    identifier id[1000];
    int top;
} Stack;

Stack symbolsTable;
int line = 1;
//token *tokenList = NULL;

void insertTable(char *lexema, int type, char *scope, void *address){
    symbolsTable.top++;
    symbolsTable.id[symbolsTable.top].lexema = lexema;
    symbolsTable.id[symbolsTable.top].type = type;
    symbolsTable.id[symbolsTable.top].scope = scope;
    symbolsTable.id[symbolsTable.top].address = address;
}

void placeTypeTable(int type){
    for(int i = symbolsTable.top; i > -1; i--)
    {
        if(symbolsTable.id[i].type == tvariavel){
            symbolsTable.id[i].type = type;
        }
    }
}


//void storeToken(token tk){
//    printf("[%s]: %d\n",tk.lexema, tk.simbolo);
//    token* newTk = (token*)malloc(sizeof(token));
//    if (!newTk) {
//        printf("Memory allocation failed\n");
//        exit(1);
//    }
//    newTk->lexema = tk.lexema;
//    newTk->simbolo = tk.simbolo;
//    newTk->next = tokenList;
//    tokenList = newTk;
//}


int isWhitespace(int c){
    // 8, 9, 10, 32 are whitespace
    if (c == (int)'\b' || c == (int)'\t' || c == (int)'\n' || c == (int)' '){
        if (c == (int)'\n')
        {
            line++;
        }
        return 1;
    }

    return 0;
}

token handleError(int *c, FILE *file){
    token tk;
    tk.lexema = malloc(sizeof(char)*255);
    char aux[255];
    int i = 0;

    do
    {
        aux[i] = (char) *c;
        *c = fgetc(file);
        i++;
    }
    while(!(isWhitespace(*c) || *c == ':' || *c == '+' ||
            *c == '-' || *c == '*' || *c == '!' || *c == '<' ||
            *c == '>' || *c == '=' || *c == ',' || *c == '(' ||
            *c == ')' || *c == '.' || *c == EOF)
    );
    aux[i] = '\0';
    strcpy(tk.lexema, aux);
    tk.simbolo = serro;
    printf("Error in line %d: Unrecognized identifier \'%s\'\n", line, tk.lexema);

    return tk;
}

token handleDigit(int *c, FILE *file){
    token tk;
    tk.lexema = malloc(sizeof(char)*255);
    char aux[255];
    int i = 0;

    do
    {
        aux[i] = (char) *c;
        *c = fgetc(file);
        i++;
    }while(isdigit(*c));
    aux[i] = '\0';
    strcpy(tk.lexema, aux);
    tk.simbolo = snumero;

    return tk;
}

token handleIdOrReserved(int *c, FILE *file){
    token tk;
    tk.lexema = malloc(sizeof(char)*255);
    char aux[255];
    int i = 0;

    do
    {
        aux[i] = (char) *c;
        *c = fgetc(file);
        i++;
    }while(isalnum(*c) || *c == '_');
    aux[i] = '\0';
    strcpy(tk.lexema, aux);
    tk.simbolo = sidentificador;

    if (aux[0] == 'p')
    {
        if (strcmp(aux, "programa") == 0)
            tk.simbolo = sprograma;
        else if (strcmp(aux, "procedimento") == 0)
            tk.simbolo = sprocedimento;
    }
    else if (aux[0] == 's')
    {
        if (strcmp(aux, "se") == 0)
            tk.simbolo = sse;
        else if (strcmp(aux, "senao") == 0)
            tk.simbolo = ssenao;
    }
    else if (aux[0] == 'e')
    {
        if (strcmp(aux, "entao") == 0)
            tk.simbolo = sentao;
        else if (strcmp(aux, "enquanto") == 0)
            tk.simbolo = senquanto;
        else if (strcmp(aux, "escreva") == 0)
            tk.simbolo = sescreva;
        else if (strcmp(aux, "e") == 0)
            tk.simbolo = se;
    }
    else if (aux[0] == 'f')
    {
        if (strcmp(aux, "faca") == 0)
            tk.simbolo = sfaca;
        else if (strcmp(aux, "fim") == 0)
            tk.simbolo = sfim;
        else if (strcmp(aux, "falso") == 0)
            tk.simbolo = sfalso;
        else if (strcmp(aux, "funcao") == 0)
            tk.simbolo = sfuncao;
    }
    else if (aux[0] == 'i')
    {
        if (strcmp(aux, "inicio") == 0)
            tk.simbolo = sinicio;
        else if (strcmp(aux, "inteiro") == 0)
            tk.simbolo = sinteiro;
    }
    else if (aux[0] == 'l')
    {
        if (strcmp(aux, "leia") == 0)
            tk.simbolo = sleia;
    }
    else if (aux[0] == 'v')
    {
        if (strcmp(aux, "var") == 0)
            tk.simbolo = svar;
        else if (strcmp(aux, "verdadeiro") == 0)
            tk.simbolo = sverdadeiro;
    }
    else if (aux[0] == 'b')
    {
        if (strcmp(aux, "booleano") == 0)
            tk.simbolo = sbooleano;
    }
    else if (aux[0] == 'd')
    {
        if (strcmp(aux, "div") == 0)
            tk.simbolo = sdiv;
    }
    else if (aux[0] == 'o')
    {
        if (strcmp(aux, "ou") == 0)
            tk.simbolo = sou;
    }
    else if (aux[0] == 'n')
    {
        if (strcmp(aux, "nao") == 0)
            tk.simbolo = snao;
    }

    return tk;
}

token handleAttr(int *c, FILE *file){
    token tk;
    *c = fgetc(file);
    if (*c == (int)'=') {
        tk.lexema = ":=";
        tk.simbolo = satribuicao;
        *c = fgetc(file);
    } else {
        tk.lexema = ":";
        tk.simbolo = sdoispontos;
    }

    return tk;
}

token handleOpAri(int *c, FILE *file){
    token tk;

    if (*c == (int)'+'){
        tk.lexema = "+";
        tk.simbolo = smais;
    }
    else if (*c == (int)'-')
    {
        tk.lexema = "-";
        tk.simbolo = smenos;
    }
    else
    {
        tk.lexema = "*";
        tk.simbolo = smult;
    }
    *c = fgetc(file);

    return tk;
}

token handleOpRel(int *c, FILE *file){
    token tk;

    if (*c == (int)'!'){
        *c = fgetc(file);

        if (*c == (int)'=')
        {
            tk.lexema = "!=";
            tk.simbolo = sdif;
            *c = fgetc(file);
        }
        else
        {
            token newTk;
            newTk = handleError(c, file);
        }
    }
    else if (*c == (int)'<')
    {
        *c = fgetc(file);

        if (*c == (int)'=')
        {
            tk.lexema = "<=";
            tk.simbolo = smenorig;
            *c = fgetc(file);
        }
        else
        {
            tk.lexema = "<";
            tk.simbolo = smenor;
        }
    }
    else if (*c == (int)'>')
    {
        *c = fgetc(file);

        if (*c == (int)'=')
        {
            tk.lexema = ">=";
            tk.simbolo = smaiorig;
            *c = fgetc(file);
        }
        else
        {
            tk.lexema = ">";
            tk.simbolo = smaior;
        }
    }
    else
    {
        tk.lexema = "=";
        tk.simbolo = sig;
        *c = fgetc(file);
    }

    return tk;
}

token handlePunct(int *c, FILE *file){
    token tk;

    if (*c == (int)';')
    {
        tk.lexema = ";";
        tk.simbolo = sponto_virgula;
    }
    else if (*c == (int)',')
    {
        tk.lexema = ",";
        tk.simbolo = svirgula;
    }
    else if (*c == (int)'(')
    {
        tk.lexema = "(";
        tk.simbolo = sabre_parenteses;
    }
    else if (*c == (int)')')
    {
        tk.lexema = ")";
        tk.simbolo = sfecha_parenteses;
    }
    else
    {
        tk.lexema = ".";
        tk.simbolo = sponto;
    }

    *c = fgetc(file);

    return tk;
}

token getToken(int *c, FILE *file){
    token tk;
    if (isdigit(*c))
    {
        tk = handleDigit(c, file);
    }
    else if (isalpha(*c))
    {
        tk = handleIdOrReserved(c, file);
    }
    else if (*c == (int)':')
    {
        tk = handleAttr(c, file);
    }
    else if (*c == (int)'+' || *c == (int)'-' || *c == (int)'*')
    {
        tk = handleOpAri(c, file);
    }
    else if (*c == (int)'!' || *c == (int)'<' || *c == (int)'>' || *c == (int)'=')
    {
        tk = handleOpRel(c, file);
    }
    else if (*c == (int)';' || *c == (int)',' || *c == (int)'(' || *c == (int)')' || *c == (int)'.')
    {
        tk = handlePunct(c, file);
    }
    else
    {
        tk = handleError(c, file);
    }


    return tk;
}

token lexical(FILE *file, int *c){
    token tk;
    while((*c == (int)'{' || isWhitespace(*c)) && *c != EOF)
    {
        if(*c == (int)'{'){
            while(*c != (int)'}' && *c != EOF)
            {
                if(*c == (int)'\n'){
                    line++;
                }
                *c = fgetc(file);
            }
            if (*c == EOF){
                printf("Error in line %d: Unterminated comment.", line);
            }
            *c = fgetc(file);
        }
        while(isWhitespace(*c) && *c != EOF){
            *c = fgetc(file);
        }
    }
    if (*c != EOF){
        tk = getToken(c, file);
        return tk;
    }
}
