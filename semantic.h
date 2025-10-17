#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.h"

enum identifierTypes {tnomedeprograma, tvariavel, tinteiro, tbooleano,
                      tprocedimento, tfuncaointeiro, tfuncaobooleana};

typedef struct identifier {
    char *lexema;
    int type;
    char scope;
    void *address;
} identifier;

typedef struct Stack {
    identifier id[1000];
    int top;
} Stack;

Stack symbolsTable;

void insertTable(char *lexema, int type, char scope, void *address){
    symbolsTable.top++;
    symbolsTable.id[symbolsTable.top].lexema = lexema;
    symbolsTable.id[symbolsTable.top].type = type;
    symbolsTable.id[symbolsTable.top].scope = scope;
    symbolsTable.id[symbolsTable.top].address = address;
}

/// Conversão para posfixa
Stack posfixConvertion(Stack expr){
    
    return expr;
}

/// Search until the first mark
int searchDuplVarTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0 || symbolsTable.id[i].scope != 0; i--){
        if (symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return 1;
            }
        }
    }
    return 0;
}

/// Search until the start of the stack
int searchDeclVarTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0; i--){
        if (symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return 1;
            }
        }
    }
    return 0;
}


int searchDeclProcTable(char* lexema);

int searchDeclVarFuncTable(char *lexema);

int searchTable(char *lexema);

void placeTypeTable(int type){
    for(int i = symbolsTable.top; i > -1; i--)
    {
        if(symbolsTable.id[i].type == tvariavel){
            if (type == sinteiro){
                symbolsTable.id[i].type = tinteiro;
            } else {
                symbolsTable.id[i].type = tbooleano;
            }
        }
    }
}

void unstackLevel(){
    int i;
    for(i = symbolsTable.top; i == 0 || symbolsTable.id[i].scope != 0; i--);
    symbolsTable.top = i;
    symbolsTable.id[i].scope = 0;
}

#endif
