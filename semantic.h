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

typedef struct expr {
    char *lexema[255];
    int type[255];
    int top;
} Expr;

Stack symbolsTable;

void insertTable(char *lexema, int type, char scope, void *address){
    symbolsTable.top++;
    symbolsTable.id[symbolsTable.top].lexema = lexema;
    symbolsTable.id[symbolsTable.top].type = type;
    symbolsTable.id[symbolsTable.top].scope = scope;
    symbolsTable.id[symbolsTable.top].address = address;
}

int checkPrecedence(int type, int v[], int n){
    int flag = 1;
    for (int i = 0; i < n; i++){
        flag = flag || (type != v[i]);
    }

    return flag;
}

/// Conversão para posfixa
void posfixConvertion(Expr expr){
    int precedenceArit[] = {smenos, smais, sdiv, smult, sabre_parenteses};
    Expr posfix, stack;
    for (int i = 0; i < expr.top; i++){
        // Se for variável ou número copia na saída
        if (expr.type[i] == snumero || expr.type[i] == sverdadeiro || expr.type[i] == sfalso || expr.type[i] == sidentificador){
            posfix.lexema[posfix.top] = expr.lexema[i];
            posfix.type[posfix.top] = expr.type[i];
            posfix.top++;
        // Se for abre-parênteses empilha
        } else if (expr.type[i] == sabre_parenteses) {
            stack.lexema[stack.top] = expr.lexema[i];
            stack.type[stack.top] = expr.type[i];
            stack.top++;
        // Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses (desempilhando-o)
        } else if (expr.type[i] == sfecha_parenteses){
            for(int j = stack.top; j > 0 || stack.type[j] != sabre_parenteses; j--){
                posfix.lexema[posfix.top] = stack.lexema[j];
                posfix.type[posfix.top] = stack.type[j];
                posfix.top++;
                stack.top--;
            }
            stack.top--;
        // Se for operador unário, desempilha todos os operadores até um operador de mult ou div ou mais ou menos ou abre-parenteses ou fim da pilha
        } else if (expr.type[i] == spositivo || expr.type[i] == snegativo){
            for(int j = stack.top; j > 0 || checkPrecedence(stack.type[j], precedenceArit, 5); j--){
                posfix.lexema[posfix.top] = stack.lexema[j];
                posfix.type[posfix.top] = stack.type[j];
                posfix.top++;
                stack.top--;
            }
            stack.lexema[stack.top] = expr.lexema[i];
            stack.type[stack.top] = expr.type[i];
            stack.top++;
        // Se for operador mult ou div, desempilha todos os operadores até um operador mais ou menos ou abre-parenteses ou fim da pilha
        } else if (expr.type[i] == smult || expr.type[i] == sdiv) {
            for(int j = stack.top; j > 0 || checkPrecedence(stack.type[j], &(precedenceArit[2]), 3); j--){
                posfix.lexema[posfix.top] = stack.lexema[j];
                posfix.type[posfix.top] = stack.type[j];
                posfix.top++;
                stack.top--;
            }
            stack.lexema[stack.top] = expr.lexema[i];
            stack.type[stack.top] = expr.type[i];
            stack.top++;
        // Se for operador mais ou menos, desempilha todos os operadores até um abre-parenteses ou fim da pilha
        } else if (expr.type[i] == smais || expr.type[i] == smenos) {
            for(int j = stack.top; j > 0 || stack.type[j] != sabre_parenteses; j--){
                posfix.lexema[posfix.top] = stack.lexema[j];
                posfix.type[posfix.top] = stack.type[j];
                posfix.top++;
                stack.top--;
            }
            stack.lexema[stack.top] = expr.lexema[i];
            stack.type[stack.top] = expr.type[i];
            stack.top++;
        // Se for operador relacional, desempilha todos os operadores até um abre-parenteses ou fim da pilha
        }// else if AQUI!!
    }
}

/// Search until the first mark
int searchDuplVarTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0 || symbolsTable.id[i].scope != 0; i--){
        if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
            return 1;
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

/// Search until the start of the stack
int searchDeclProcTable(char* lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0; i--){
        if (symbolsTable.id[i].type == tprocedimento){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return 1;
            }
        }
    }
    return 0;
}

/// Search until the start of the stack
int searchDeclVarFuncTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0; i--){
        if (symbolsTable.id[i].type == tfuncaointeiro || symbolsTable.id[i].type == tfuncaobooleana){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return 1;
            }
        }
    }
    return 0;
}

/// Search until the start of the stack
int searchTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top; i < 0; i--){
        if(symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano || symbolsTable.id[i].type == tfuncaointeiro || symbolsTable.id[i].type == tfuncaobooleana){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return i;
            }
        }
    }
    return 0;
}

void placeTypeTable(int type){
    for(int i = symbolsTable.top; i < 0 || symbolsTable.id[i].scope != 0; i--)
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
