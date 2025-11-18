#ifndef CODEGEN_H
#define CODEGEN_H

#include "lexical.h"
#include "semantic.h"

FILE *output_file;

void codegenSetup(FILE *outputFile){
    output_file = outputFile;
}

void codeGen(char label[4], char instruction[8], char addr[4], char cont[4]){
    char codeLine[24];
    sprintf(codeLine, "%-3s %7s %3s %3s", label, instruction, addr, cont);
    fprintf(output_file, "%s\n",codeLine);
}

void codeGenExpr(Expr posfix, int resultType){
    Expr auxStack;
    posfixConversion(&posfix);
    identifier id;
    char addr[4];

    auxStack.top = 0;
    for (int i = 0; i < posfix.top; i++){
        switch (posfix.type[i]){
            case snumero:
                auxStack.type[auxStack.top] = tinteiro;
                codeGen("   ", "LDC    ", posfix.lexema[i], "   ");
                auxStack.top++;
                break;
            case sverdadeiro:
                strncpy(auxStack.lexema[auxStack.top], "1", 2);
                auxStack.type[auxStack.top] = tbooleano;
                codeGen("   ", "LDC    ", auxStack.lexema[auxStack.top], "   ");
                auxStack.top++;
                break;
            case sfalso:
                strncpy(auxStack.lexema[auxStack.top], "0", 2);
                auxStack.type[auxStack.top] = tbooleano;
                codeGen("   ", "LDC    ", auxStack.lexema[auxStack.top], "   ");
                auxStack.top++;
                break;
            case sidentificador:
                id = symbolsTable.id[searchTable(posfix.lexema[i])];
                if(id.type == tfuncaointeiro || id.type == tfuncaobooleana){
                    codeGen("   ", "CALL   ", id.address, "   ");
                    sprintf(addr, "%s", "0");
                    auxStack.type[auxStack.top] = tinteiro ? id.type == tfuncaointeiro : tbooleano;
                } else {
                    sprintf(addr, "%s", id.address);
                    auxStack.type[auxStack.top] = id.type;
                }
                codeGen("   ", "LDV    ", addr, "   ");
                auxStack.top++;
                break;
            case smais:
                if (posfix.lexema[i][0] == 'u'){
                    if (auxStack.type[auxStack.top - 1] == tbooleano){
                        printf("Error in line %d: Invalid operation '+' on a boolean.\n", line);
                        exit(-1);
                    }
                } else {
                    if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                        printf("Error in line %d: Invalid operation '+' on a boolean.\n", line);
                        exit(-1);
                    }
                    codeGen("   ", "ADD    ", "   ", "   ");
                    auxStack.top--;
                }
                break;
            case smenos:
                if (posfix.lexema[i][0] == 'u'){
                    if (auxStack.type[auxStack.top - 1] == tbooleano){
                        printf("Error in line %d: Invalid operand '-' on a boolean.\n", line);
                        exit(-1);
                    }
                    codeGen("    ", "INV    ", "    ", "    ");
                } else {
                    if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                        printf("Error in line %d: Invalid operation '-' on a boolean.\n", line);
                        exit(-1);
                    }
                    codeGen("   ", "SUB    ", "   ", "   ");
                    auxStack.top--;
                }
                break;
            case smult:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '*' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "MULT   ", "   ", "   ");
                auxStack.top--;
                break;
            case sdiv:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation 'div' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "DIVI   ", "   ", "   ");
                auxStack.top--;
                break;
            case smaior:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '>' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CMA    ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case smenor:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '<' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CME    ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case sig:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '=' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CEQ    ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case sdif:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '!=' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CDIF   ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case smaiorig:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '>=' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CMAQ   ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case smenorig:
                if ((auxStack.type[auxStack.top - 1] == tbooleano) || (auxStack.type[auxStack.top - 2] == tbooleano)){
                    printf("Error in line %d: Invalid operation '<=' on a boolean.\n", line);
                    exit(-1);
                }
                codeGen("   ", "CMEQ   ", "   ", "   ");
                auxStack.top--;
                auxStack.type[auxStack.top - 1] = tbooleano;
                break;
            case snao:
                if (auxStack.type[auxStack.top - 1] == tinteiro){
                    printf("Error in line %d: Invalid operand 'nao' on an integer.\n", line);
                    exit(-1);
                }
                codeGen("   ", "NEG    ", "   ", "   ");
                auxStack.top--;
                break;
            case se:
                if ((auxStack.type[auxStack.top - 1] == tinteiro) || (auxStack.type[auxStack.top - 2] == tinteiro)){
                    printf("Error in line %d: Invalid operation 'AND' on an integer.\n", line);
                    exit(-1);
                }
                codeGen("   ", "AND    ", "   ", "   ");
                auxStack.top--;
                break;
            case sou:
                if ((auxStack.type[auxStack.top - 1] == tinteiro) || (auxStack.type[auxStack.top - 2] == tinteiro)){
                    printf("Error in line %d: Invalid operation 'OR' on an integer.\n", line);
                    exit(-1);
                }
                codeGen("   ", "OR     ", "   ", "   ");
                auxStack.top--;
                break;
        }
        free(posfix.lexema[i]);
    }
    if (auxStack.type[auxStack.top - 1] == tinteiro && (resultType == tbooleano || resultType == tfuncaobooleana)){
        printf("Error in line %d: Invalid integer expression, it should result into a boolean.\n", line);
        exit(-1);
    } else if (auxStack.type[auxStack.top - 1] == tbooleano && (resultType == tinteiro || resultType == tfuncaointeiro)){
        printf("Error in line %d: Invalid boolean expression, it should result into an integer.\n", line);
        exit(-1);
    }
}

/// Unstack until scope and dalloc popped variables
void unstackLevel(){
    int i, aux = 0;
    char addr[4], cont[4];
    for(i = symbolsTable.top - 1; i > -1 && symbolsTable.id[i].scope == 0; i--){
        if (symbolsTable.id[i].type == tinteiro || symbolsTable.id[i].type == tbooleano){
            aux++;
        }
        free(symbolsTable.id[i].lexema);
        free(symbolsTable.id[i].address);
    }
    symbolsTable.top = i + 1;
    symbolsTable.id[i].scope = 0;

    if(aux > 0){
        sprintf(addr, "%d", availableAddr - aux);
        sprintf(cont, ",%d", aux);
        availableAddr -= aux;
        codeGen("   ", "DALLOC ", addr, cont);
    }
}

#endif
