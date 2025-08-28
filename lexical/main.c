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
                         sdoispontos, sverdadeiro, sfalso};

typedef struct token {
    char *lexema;
    int simbolo;
    struct token *next;
}token;

token *tokenList;

void storeToken(token tk, token *ptr){
    ptr = malloc(sizeof(token));
    strcpy(ptr->lexema, tk.lexema);
    ptr->simbolo = tk.simbolo;
    ptr->next = NULL;
}

int isWhitespace(int c){
    // 8, 9, 10, 32 are whitespace
    if (c == (int)'\b' || c == (int)'\t' || c == (int)'\n' || c == (int)' '){
        return 1;
    }

    return 0;
}

token handleDigit(int *c, FILE *file){
    token tk;
    return tk;
}

token handleIdOrReserved(int *c, FILE *file){
    token tk;
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

    if (c == (int)'+'){
        tk.lexema = "+";
        tk.simbolo = smais;
    }
    else if (c == (int)'-')
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

    if (c == (int)'!'){
        *c = fgetc(file);

        if (c == (int)'=')
        {
            tk.lexema = "!=";
            tk.simbolo = sdif;
            *c = fgetc(file);
        }
        else
        {
            printf("error");
        }
    }
    else if (c == (int)'<')
    {
        *c = fgetc(file);

        if (c == (int)'=')
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
    else if (c == (int)'>')
    {
        *c = fgetc(file);

        if (c == (int)'=')
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
    return tk;
}

token handleError(int *c, FILE *file){
    token tk;
    printf("error");
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

int main(int argc, char *argv[])
{
    // No filename sent
    if (argc != 2)
    {
        printf("Usage: lpd <filename>");
        return 1;
    }

    int c;
    FILE *file = fopen(argv[1], "r");

    // Could not open file
    if (file == NULL){
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }
    token *ptr = tokenList;
    token newToken;

    c = fgetc(file);

    while(c != EOF)
    {
        while((c == (int)'{' || isWhitespace(c)) && c != EOF)
        {
            if(c == (int)'{'){
                while(c != (int)'}' && c != EOF)
                {
                    c = fgetc(file);
                }
                c = fgetc(file);
            }
            while(isWhitespace(c) && c != EOF){
                c = fgetc(file);
            }
        }
        if (c != EOF){
            newToken = getToken(&c, file);
            storeToken(newToken, ptr);
        }
    }

    for(token *ptk = tokenList; ptk == NULL; ptk = ptk->next)
    {
        free(ptk);
    }
    fclose(file);

    return 0;
}

