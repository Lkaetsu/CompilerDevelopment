#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "lexical.h"

enum identifierTypes {tnomedeprograma=50, tvariavel, tinteiro, tbooleano,
                      tprocedimento, tfuncaointeiro, tfuncaobooleana};

typedef struct identifier {
    char *lexema;
    int type;
    int scope;
    char *address;
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
int label, availableAddr;

void setupSymbolsTable(){
    label = 1;
    availableAddr = 0;
    symbolsTable.top = 0;
}

/// Inserção na tabela de símbolos
void insertTable(char *lexema, int type, int scope, char *address){
    symbolsTable.id[symbolsTable.top].lexema = malloc(sizeof(char) * (strlen(lexema) + 1));
    symbolsTable.id[symbolsTable.top].address = malloc(sizeof(char) * (strlen(address) + 1));
    strcpy(symbolsTable.id[symbolsTable.top].lexema, lexema);
    symbolsTable.id[symbolsTable.top].type = type;
    symbolsTable.id[symbolsTable.top].scope = scope;
    strcpy(symbolsTable.id[symbolsTable.top].address, address);
    symbolsTable.top++;
}

int checkArrayForType(int type, int arr[], int n){
    for (int i = 0; i < n; i++){
        if (type == arr[i])
            return 1;
    }

    return 0;
}

void storeExpr(Expr *e, char *lexema, int type){
    if(e->top > 255){
        printf("Error in line %d: Expression compilation limit exceeded.", line);
        exit(-1);
    }

    e->lexema[e->top] = strdup(lexema);
    e->type[e->top] = type;
    e->top++;
}

void popExpr(Expr *e){
    free(e->lexema[e->top - 1]);
    e->top--;
}

/// Conversão para posfixa
Expr posfixConversion(Expr *expr){
    Expr posfix, auxStack;
    posfix.top = 0;
    auxStack.top = 0;
    int precedences[] = {smult, sdiv, smais, smenos, smaior, smaiorig, sig, smenorig, smenor, sdif, snao, se, sou};
    int prec_i, N = 13;

    for (int i = 0; i < expr->top; i++){
        if (expr->type[i] == snumero || expr->type[i] == sverdadeiro || expr->type[i] == sfalso || expr->type[i] == sidentificador){
            // Se for variável ou número copia na saída
            storeExpr(&posfix, expr->lexema[i], expr->type[i]);
        } else if (expr->type[i] == sabre_parenteses) {
            // Se for abre-parênteses empilha
            storeExpr(&auxStack, expr->lexema[i], expr->type[i]);
        } else if (expr->type[i] == sfecha_parenteses){
            // Se for fecha-parênteses, desempilha tudo copiando na saída até o primeiro abre-parênteses (desempilhando-o)
            for(int j = auxStack.top - 1; j > -1 && auxStack.type[j] != sabre_parenteses; j--){
                storeExpr(&posfix, auxStack.lexema[j], auxStack.type[j]);
                popExpr(&auxStack);
            }
            popExpr(&auxStack);
        } else if (checkArrayForType(expr->type[i], precedences, N)){
            // Se for operador, empilha após percorrer desempilhando e copiando na saída todos os operadores
            // com precedência maior ou igual ao que será empilhado, isso até encontrar o primeiro
            // operador com precedência menor, terminar a pilha ou encontrar o primeiro abre-parênteses
            switch (expr->type[i]) {
                case smais:
                case smenos:
                    if (expr->lexema[i][0] == 'u'){
                        prec_i = 0;
                    } else {
                        prec_i = 4;
                    }
                    break;
                case smult:
                case sdiv:
                    prec_i = 2;
                    break;
                case snao:
                    prec_i = 11;
                    break;
                case se:
                    prec_i = 12;
                    break;
                case sou:
                    prec_i = N;
                    break;
                default:
                    prec_i = 10;
                    break;
            }

            for(int j = auxStack.top - 1; j > -1;j--){
                if (auxStack.type[j] == sabre_parenteses || checkArrayForType(auxStack.type[j], &precedences[prec_i], N - prec_i)){
                    break;
                }
                storeExpr(&posfix, auxStack.lexema[j], auxStack.type[j]);
                popExpr(&auxStack);
            }
            storeExpr(&auxStack, expr->lexema[i], expr->type[i]);
        }
    }
    // Se terminar a expressão, desempilhar copiando na saída todos os operadores ainda existentes na pilha
    for(int i = auxStack.top - 1; i > -1; i--){
        storeExpr(&posfix, auxStack.lexema[i], auxStack.type[i]);
        popExpr(&auxStack);
    }
    while(expr->top > 0){
        popExpr(expr);
    }
    *expr = posfix;
}

/// Search until the first mark
int searchDuplVarTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top - 1; i > -1 && symbolsTable.id[i].scope == 0; i--){
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
    for(i = symbolsTable.top - 1; i > -1; i--){
        if (symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return i;
            }
        }
    }
    return -1;
}

/// Search until the start of the stack
int searchDeclProcTable(char* lexema)
{
    int i;
    for(i = symbolsTable.top - 1; i > -1; i--){
        // printf("%d: %s, %d, %d, %s\n", i, symbolsTable.id[i].lexema, symbolsTable.id[i].type, symbolsTable.id[i].scope, symbolsTable.id[i].address);
        if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
            return i;
        }
    }
    return -1;
}

/// Search until the start of the stack
int searchDeclVarFuncTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top - 1; i > -1; i--){
        // printf("%d: %s, %d, %d, %s\n", i, symbolsTable.id[i].lexema, symbolsTable.id[i].type, symbolsTable.id[i].scope, symbolsTable.id[i].address);
        if (symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano || symbolsTable.id[i].type == tfuncaointeiro || symbolsTable.id[i].type == tfuncaobooleana){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return i;
            }
        }
    }
    return -1;
}

/// Search until the start of the stack
int searchTable(char *lexema)
{
    int i;
    for(i = symbolsTable.top - 1; i > -1; i--){
        if(symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano || symbolsTable.id[i].type == tfuncaointeiro || symbolsTable.id[i].type == tfuncaobooleana){
            if(strcmp(symbolsTable.id[i].lexema, lexema) == 0){
                return i;
            }
        }
    }
    return -1;
}

/// Coloca os tipos corretos para as variáveis
void placeTypeTable(int type){
    for(int i = symbolsTable.top - 1; i > -1 && symbolsTable.id[i].scope == 0; i--)
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

#endif
