#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum simbolosReservados = {sprograma, sinicio, sfim, sprocedimento, sfuncao, sse, sentao, ssenao, senquanto, sfaca, satribuicao, sescreva, sleia, svar, sinteiro, sbooleano, sidentificador, snumero, sponto, sponto_virgula, svirgula, sabre_parenteses, sfecha_parenteses, smaior, smaiorig, sig, smenor, smenorig, sdif, smais, smenos, smult, sdiv, se, sou, snao, sdoispontos, sverdadeiro, sfalso};

typedef struct token {
    char lexema[];
    char simbolo[];
    struct token *next;
}token;

token *tk;
tk = malloc(sizeof(token));

void storeToken(char t[], int s){
    strcpy(tk->lexema, t);
    tk->simbolo = s;
    tk->NEXT = malloc(sizeof(token));
    tk = tk->NEXT;
    tk->next = NULL;
}

int isWhitespace(int c){
    // 8, 9, 10, 32 are whitespace
    if (c == 8 || c == 9 || c == 10 || c == 32){
        return 1;
    }

    return 0;
}

void handleDigit(int c){

}

void handleIdOrReserved(int c){

}

int handleAttr(file *file){
    int c;
    c = fgetc(file);
    if (*c == (int)'=') {
        tk->lexema = ":=";
        tk->simbolo = satribuicao;
        fgetc(file);
    } else {
        tk->lexema = ":";
        tk->simbolo = sdoispontos;
    }

    return c;
}

void handleOpAri(int c){

}

void handleOpRel(int c){

}

void handlePunct(int c){

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
    FILE *file = fopen(argv[1]);

    // Could not open file
    if (file == NULL){
        printf("Could not open %s.\n", );
        return 1;
    }

    c = fgetc(file);

    while(c != EOF)
    {
        while((c == '{' || isWhitespace(c)) && c != EOF)
        {

        }
    }

    for(token ptk = tk; ptk == NULL; ptk = ptk->next)
    {
        free(ptk);
    }
    fclose(file);

    return 0;
}

