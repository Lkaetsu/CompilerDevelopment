#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include "lexical.h"
#include "semantic.h"
#include "codegen.h"

token tk;

void syntactical();
void blockAnalysis();
void variableStageAnalysis();
void variableAnalysis();
void typeAnalysis();
void commandAnalysis();
void simpleCommandAnalysis();
void attribProcCallAnalysis();
void attribAnalysis();
void readAnalysis();
void writeAnalysis();
void whileAnalysis();
void ifAnalysis();
void subroutineAnalysis();
void procDeclAnalysis();
void funcDeclAnalysis();
void expressionAnalysis();
void simpleExpressionAnalysis();
void termAnalysis();
void factorAnalysis();
void procCallAnalysis();
void funcCallAnalysis();

void funcCallAnalysis(Expr *expr, char *address){
    char addr[4];
    sprintf(addr, "%s", address);
    // printf("<chamada de funcao>\n");
    storeExpr(expr, tk.lexema, tk.simbolo);
    codeGen("   ", "CALL   ", addr, "   ");
    free(tk.lexema);
    tk = lexical();
}

void procCallAnalysis(char *procName){
    int aux = searchDeclProcTable(procName);
    free(procName);
    // printf("<chamada de procedimento>\n");
    if (aux == -1){
        printf("Error in line %d: Procedure '%s' used, but not declared.\n", line, procName);
        exit(-1);
    }
    /// TODO: ask teacher if procedure calls should be able to call functions as well and if they should throw an error if it is just a variable
//    else if (symbolsTable.id[aux].type != tprocedimento){
//        printf("Error in line %d: Procedure '%s' used, but not declared.", line, procName);
//        exit(-1);
//    }
    codeGen("   ", "CALL   ", symbolsTable.id[aux].address, "   ");
}

void factorAnalysis(Expr *expr){
    // printf("<fator> -> ");
    int ind;
    if (tk.simbolo == sidentificador){      // Variable or function
        // printf("%s ", tk.lexema);
        ind = searchTable(tk.lexema);
        if (ind != -1){
            if (symbolsTable.id[ind].type == tfuncaointeiro ||
                symbolsTable.id[ind].type == tfuncaobooleana)
            {
                funcCallAnalysis(expr, symbolsTable.id[ind].address);
            } else {
                storeExpr(expr, tk.lexema, tk.simbolo);
                free(tk.lexema);
                tk = lexical();
            }
        } else {
            printf("Error in line %d: Identifier '%s' used, but not declared.", line, tk.lexema);
            exit(-1);
        }
    } else {
        if (tk.simbolo == snumero){         // Number
            // printf("%s ", tk.lexema);
            storeExpr(expr, tk.lexema, tk.simbolo);
            free(tk.lexema);
            tk = lexical();

        } else if (tk.simbolo == snao){
            // printf("%s ", tk.lexema);
            storeExpr(expr, tk.lexema, tk.simbolo);
            free(tk.lexema);
            tk = lexical();
            factorAnalysis(expr);
        } else if (tk.simbolo == sabre_parenteses){
            // printf("%s ", tk.lexema);
            storeExpr(expr, tk.lexema, tk.simbolo);
            free(tk.lexema);
            tk = lexical();
            expressionAnalysis(expr);

            if (tk.simbolo == sfecha_parenteses){
                // printf("%s ", tk.lexema);
                storeExpr(expr, tk.lexema, tk.simbolo);
                free(tk.lexema);
                tk = lexical();
            } else {
                printf("Error in line %d: Unclosed '(' in expression.", line);
                exit(-1);
            }
        } else if (tk.simbolo == sverdadeiro || tk.simbolo == sfalso){
            // printf("%s ", tk.lexema);
            storeExpr(expr, tk.lexema, tk.simbolo);
            free(tk.lexema);
            tk = lexical();
        } else {
            printf("Error in line %d: Unexpected token '%s' in place of an expression.", line, tk.lexema);
            exit(-1);
        }
    }
}

void termAnalysis(Expr *expr){
    // printf("<termo> -> ");
    factorAnalysis(expr);
    while(tk.simbolo == smult || tk.simbolo == sdiv ||
          tk.simbolo == se)
    {
        // printf("%s ", tk.lexema);
        storeExpr(expr, tk.lexema, tk.simbolo);
        free(tk.lexema);
        tk = lexical();
        factorAnalysis(expr);
    }
}

void simpleExpressionAnalysis(Expr *expr){
    // printf("<expressao simples> -> ");
    if (tk.simbolo == smais || tk.simbolo == smenos){
        // printf("%s ", tk.lexema);
        storeExpr(expr, tk.lexema, tk.simbolo);
        free(tk.lexema);
        tk = lexical();
    }
    termAnalysis(expr);
    while(tk.simbolo == smais || tk.simbolo == smenos ||
          tk.simbolo == sou)
    {
        // printf("%s ", tk.lexema);
        storeExpr(expr, tk.lexema, tk.simbolo);
        free(tk.lexema);
        tk = lexical();
        termAnalysis(expr);
    }
}

void expressionAnalysis(Expr *expr){
    // printf("<expressao> -> ");
    simpleExpressionAnalysis(expr);
    if (tk.simbolo == smaior || tk.simbolo == smaiorig ||
        tk.simbolo == smenor || tk.simbolo == smenorig ||
        tk.simbolo == sdif)
    {
        // printf("%s ", tk.lexema);
        storeExpr(expr, tk.lexema, tk.simbolo);
        free(tk.lexema);
        tk = lexical();
        simpleExpressionAnalysis(expr);
    }
}

void funcDeclAnalysis(){
    //printf("<declaracao de funcao> -> funcao ");
    free(tk.lexema);
    tk = lexical();
    int scopeFlag = 1;       // Marks the start of the function for searches
    char buffer[4];
    if (tk.simbolo = sidentificador){
        //printf("%s", tk.lexema);
        int aux = searchDeclProcTable(tk.lexema);
        if (aux == -1){
            sprintf(buffer, "L%d", label);
            insertTable(tk.lexema, tprocedimento, scopeFlag, buffer);
            codeGen(buffer, "NULL   ", "   ", "   ");
            label++;
            free(tk.lexema);
            tk = lexical();
            if (tk.simbolo == sdoispontos) {
                free(tk.lexema);
                tk = lexical();

                if (tk.simbolo == sinteiro || tk.simbolo == sbooleano){
                    if (tk.simbolo == sinteiro){
                        symbolsTable.id[symbolsTable.top - 1].type = tfuncaointeiro;
                    } else {
                        symbolsTable.id[symbolsTable.top - 1].type = tfuncaobooleana;
                    }
                    free(tk.lexema);
                    tk = lexical();
                    if (tk.simbolo == sponto_virgula){
                        blockAnalysis();
                    }
                } else {
                    printf("Error in line %d: '%s' is not a valid type.\n", line, tk.lexema);
                    exit(-1);
                }
            } else {
                printf("Error in line %d: Unexpected token '%s' in place of ':'.", line, tk.lexema);
                exit(-1);
            }
        } else {
            printf("Error in line %d: '%s' has already been previously declared.", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' in place of function name.", line, tk.lexema);
        exit(-1);
    }
    unstackLevel();
    codeGen("   ", "RETURN", "   ", "   ");
}

void procDeclAnalysis(){
    // printf("<declaracao de procedimento> -> procedimento ");
    free(tk.lexema);
    tk = lexical();
    int scopeFlag = 1;       // Marks the start of the procedure for searches
    char buffer[4];
    if (tk.simbolo = sidentificador){
        // printf("%s", tk.lexema);
        int aux = searchDeclProcTable(tk.lexema);
        if (aux == -1){
            sprintf(buffer, "L%d", label);
            insertTable(tk.lexema, tprocedimento, scopeFlag, buffer);
            codeGen(buffer, "NULL   ", "   ", "   ");       // Call will get this label from the symbolsTable
            label++;
            free(tk.lexema);
            tk = lexical();
            if (tk.simbolo == sponto_virgula){
                // printf(";\n");
                blockAnalysis();
            } else {
                printf("Error in line %d: Subroutine declarations must be ended by ';'.\n", line);
                exit(-1);
            }
        } else {
            printf("Error in line %d: '%s' has already been previously declared.", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' in place of procedure name.", line, tk.lexema);
        exit(-1);
    }
    unstackLevel();
    codeGen("   ", "RETURN", "   ", "   ");
}

void subroutineAnalysis(){
    int auxrot, flag = 0;
    char buffer[4];
    if (tk.simbolo == sprocedimento || tk.simbolo == sfuncao){
        // printf("<etapa de declaracao de sub-rotinas> -> ");
        auxrot = label;
        sprintf(buffer, "L%d", label);
        codeGen("   ", "JMP    ", buffer, "   ");      // Jump subroutines
        label++;
        flag = 1;
    }
    while (tk.simbolo == sprocedimento || tk.simbolo == sfuncao){
        if (tk.simbolo == sprocedimento){
            procDeclAnalysis();
        } else {
            funcDeclAnalysis(auxrot);
        }
        if (tk.simbolo == sponto_virgula){
            free(tk.lexema);
            tk = lexical();
        } else {
            printf("Error in line %d: Subroutine declarations must be ended by ';'.\n", line);
            exit(-1);
        }
    }
    if (flag == 1) {
        sprintf(buffer, "L%d", auxrot);
        codeGen(buffer, "NULL   ", "   ", "   ");      // Beginning of main
    }
}

void ifAnalysis(){
    // printf("<comando condicional> -> se ");
    char auxrot1[4], auxrot2[4];
    sprintf(auxrot1, "L%d", label);
    free(tk.lexema);
    tk = lexical();
    Expr expression;
    expression.top = 0;
    expressionAnalysis(&expression);
    codeGenExpr(expression, tbooleano);
    codeGen("   ", "JMPF   ", auxrot1, "   ");       // Jump if condition is false
    label++;
    if (tk.simbolo == sentao){
        // printf("entao \n");
        free(tk.lexema);
        tk = lexical();
        // printf("%s", tk.lexema);
        simpleCommandAnalysis();
        if (tk.simbolo == ssenao){
            sprintf(auxrot2, "L%d", label);
            codeGen("   ", "JMP    ", auxrot2, "   ");
            codeGen(auxrot1, "NULL   ", "   ", "   ");
            // printf("senao \n");
            free(tk.lexema);
            tk = lexical();
            simpleCommandAnalysis();
            codeGen(auxrot2, "NULL   ", "   ", "   ");
        } else {
            codeGen(auxrot1, "NULL   ", "   ", "   ");
        }
    } else {
        printf("Error in line %d: There must be a 'entao' after 'se(<condition>)'. Example: se(verdadeiro) entao ...\n", line);
        exit(-1);
    }
}

void whileAnalysis(){
    char buffer[4];
    int auxrot1, auxrot2;
    auxrot1 = label;
    sprintf(buffer, "L%d", auxrot1);
    codeGen(buffer, "NULL   ", "   ", "   ");
    label++;
    // printf("<comando enquanto> -> enquanto \n");
    free(tk.lexema);
    tk = lexical();
    Expr expression;
    expression.top = 0;
    expressionAnalysis(&expression);
    codeGenExpr(expression, tbooleano);
    if (tk.simbolo == sfaca){
        // printf("faca \n");
        auxrot2 = label;
        sprintf(buffer, "L%d", auxrot2);
        codeGen("   ", "JMPF   ", buffer, "   ");   // Jump if false
        label++;
        free(tk.lexema);
        tk = lexical();
        simpleCommandAnalysis();
        sprintf(buffer, "L%d", auxrot1);
        codeGen("   ", "JMP    ", buffer, "   ");  // Return to start of the loop
        sprintf(buffer, "L%d", auxrot2);
        codeGen(buffer, "NULL   ", "   ", "   "); // End of the while loop
    } else {
        printf("Error in line %d: There must be a 'faca' after 'enquanto(<condition>)'. Example: enquanto(verdadeiro) faca ...\n", line);
        exit(-1);
    }
}

void writeAnalysis(){
    // printf("<comando escrita> -> escreva");
    int aux;
    char addr[4];
    free(tk.lexema);
    tk = lexical();
    if (tk.simbolo == sabre_parenteses){
        // printf("(");
        free(tk.lexema);
        tk = lexical();
        if (tk.simbolo == sidentificador){
            // printf("%s", tk.lexema);
            aux = searchDeclVarTable(tk.lexema);
            if (aux != -1){
                if(symbolsTable.id[aux].type == tbooleano){
                    printf("Error in line %d: Only integer variables can be printed.\n", line);
                    exit(-1);
                }

                sprintf(addr, "%s", symbolsTable.id[aux].address);
                codeGen("   ", "LDV    ", addr, "   ");
                free(tk.lexema);
                tk = lexical();
                if (tk.simbolo == sfecha_parenteses){
                    // printf(")");
                    free(tk.lexema);
                    tk = lexical();
                } else {
                    printf("Error in line %d: Unexpected token '%s' before ')'. Example: escreva(x);\n", line, tk.lexema);
                    exit(-1);
                }
            } else {
                printf("Error in line %d: Variable '%s' used, but not being declared.\n", line, tk.lexema);
                exit(-1);
            }
        } else {
            printf("Error in line %d: Unexpected token '%s' inside write command, a variable name was expected.\n", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' instead of '('. Example: escreva(x);\n", line, tk.lexema);
        exit(-1);
    }
    codeGen("   ", "PRN    ", "   ", "   ");
}

void readAnalysis(){
    // printf("<comando leitura> -> leia");
    int aux;
    char addr[4];
    free(tk.lexema);
    tk = lexical();
    if (tk.simbolo == sabre_parenteses){
        // printf("(");
        free(tk.lexema);
        tk = lexical();
        if (tk.simbolo == sidentificador){
            // printf("%s", tk.lexema);
            aux = searchDeclVarTable(tk.lexema);
            if (aux != -1){
                if(symbolsTable.id[aux].type == tbooleano){
                    printf("Error in line %d: Only integer variables can be read.\n", line);
                    exit(-1);
                }

                sprintf(addr, "%s", symbolsTable.id[aux].address);
                free(tk.lexema);
                tk = lexical();
                if (tk.simbolo == sfecha_parenteses){
                    // printf(")");
                    free(tk.lexema);
                    tk = lexical();
                } else {
                    printf("Error in line %d: Unexpected token '%s' before ')'. Example: leia(x);\n", line, tk.lexema);
                    exit(-1);
                }
            } else {
                printf("Error in line %d: Variable '%s' used, but not being declared.\n", line, tk.lexema);
                exit(-1);
            }
        } else {
            printf("Error in line %d: Unexpected token '%s' inside read command, a variable name was expected.\n", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' instead of '('. Example: leia(x);\n", line, tk.lexema);
        exit(-1);
    }
    codeGen("   ", "RD     ", "   ", "   ");
    codeGen("   ", "STR    ", addr, "   ");
}

void attribAnalysis(char *varName){
    char addr[4];
    int aux = searchDeclVarFuncTable(varName);
    if (aux == -1){
        printf("Error in line %d: Attempting to store a value into undeclared variable '%s'.\n", line, varName);
        free(varName);        exit(-1);
    }

    free(varName);
    identifier id = symbolsTable.id[aux];
    free(tk.lexema);
    tk = lexical();
    Expr expression;
    expression.top = 0;
    expressionAnalysis(&expression);
    codeGenExpr(expression, id.type);
    if (id.type == tfuncaobooleana || id.type == tfuncaointeiro){
        if (symbolsTable.id[aux].scope){
            sprintf(addr, "%s", "0");
        } else {
            printf("Error in line %d: Attempting to store a value into a function outside its scope.\n", line);
            exit(-1);
        }
    } else {
        sprintf(addr, "%s", symbolsTable.id[aux].address);
    }
    codeGen("   ", "STR    ", addr, "   ");
}

void attribProcCallAnalysis(){
    // printf("<atribuicao_chprocedimento> -> ");
    char *auxName;
    auxName = strdup(tk.lexema);
    free(tk.lexema);
    tk = lexical();
    if (tk.simbolo == satribuicao){
        // printf("<comando atribuicao> -> %s := ", deriv_aux);
        attribAnalysis(auxName);
    } else {
        // printf("<chamada de procedimento> -> %s", deriv_aux);
        procCallAnalysis(auxName);
    }
}

void simpleCommandAnalysis(){
    // printf("<comando> -> ");
    switch (tk.simbolo){
        case sidentificador:
            attribProcCallAnalysis();
            break;
        case sse:
            ifAnalysis();
            break;
        case senquanto:
            whileAnalysis();
            break;
        case sleia:
            readAnalysis();
            break;
        case sescreva:
            writeAnalysis();
            break;
        default:
            commandAnalysis();
    }
}

void commandAnalysis(){
//    printf("<comandos> -> ");
    if (tk.simbolo == sinicio){
//        printf("inicio\n");
        free(tk.lexema);
        tk = lexical();
        simpleCommandAnalysis();
        while(tk.simbolo != sfim){
            if (tk.simbolo == sponto_virgula){
//                printf(";\n");
                free(tk.lexema);
                tk = lexical();
                if (tk.simbolo != sfim){
                    simpleCommandAnalysis();
                }
            } else {
                printf("Error in line %d: There must be a ';' in between commmands.\n", line);
                exit(-1);
            }
        }
        free(tk.lexema);
        tk = lexical();
    } else {
        printf("Error in line %d: The code's commands must be started by 'inicio' and be ended with 'fim'.\n", line);
        exit(-1);
    }
//    printf("fim\n");
}

void typeAnalysis(){
    // printf("<tipo> -> ");
    if (tk.simbolo != sinteiro && tk.simbolo != sbooleano){
        printf("Error in line %d: '%s' is not a valid type.\n", line, tk.lexema);
        exit(-1);
    }
    else{
        // printf("%s", tk.lexema);
        placeTypeTable(tk.simbolo);
    }
    free(tk.lexema);
    tk = lexical();
}

void variableAnalysis(){
    int cont = 0, aux = availableAddr;
    char buffer[4], buffer2[4];
    // printf("<declaracao de variaveis> -> ");
    while(tk.simbolo != sdoispontos){
        if (tk.simbolo == sidentificador){
            cont++;
            // printf("%s", tk.lexema);
            if (!searchDuplVarTable(tk.lexema)){
                sprintf(buffer, "%d", availableAddr);
                insertTable(tk.lexema, tvariavel, 0, buffer);
                availableAddr++;
                free(tk.lexema);
                tk = lexical();

                if (tk.simbolo == svirgula || tk.simbolo == sdoispontos){
                    if (tk.simbolo == svirgula){
                        // printf(", ");
                        free(tk.lexema);
                        tk = lexical();

                        if (tk.simbolo == sdoispontos){
                            printf("Error in line %d: Variable declaration can't have ':' after ',' a variable name is expected. Example: var x, y: inteiro;\n", line);
                            exit(-1);
                        }
                    }
                } else {
                    printf("Error in line %d: Unexpected token '%s' in variable declaration. Examples: var x, y:inteiro; var z : booleano\n", line, tk.lexema);
                    exit(-1);
                }
            } else {
                printf("Error in line %d: Variable %s is already declared in this scope.\n", line, tk.lexema);
                exit(-1);
            }
        } else {
            printf("Error in line %d: Variable %s is already declared in this scope.\n", line, tk.lexema);
            exit(-1);
        }
    }
    sprintf(buffer, "%d", aux);
    sprintf(buffer2, ",%d", cont);
    codeGen("   ", "ALLOC  ", buffer, buffer2);
    // printf(": ");
    free(tk.lexema);
    tk = lexical();
    typeAnalysis();
}

void variableStageAnalysis(){
    if (tk.simbolo == svar){
        // printf("<etapa de declaracao de variaveis> -> ");
        // printf("var ", tk.lexema);
        free(tk.lexema);
        tk = lexical();

        if (tk.simbolo == sidentificador){
            while (tk.simbolo == sidentificador){
                variableAnalysis();
                if (tk.simbolo == sponto_virgula){
                    // printf(";\n");
                    free(tk.lexema);
                    tk = lexical();
                } else {
                    printf("Error in line %d: Variable declaration should end with ';'. Example: var x:inteiro;\n", line);
                    exit(-1);
                }
            }
        } else {
            printf("Error in line %d: Unexpected token '%s' in place of identifier in variable declaration.\n", line, tk.lexema);
            exit(-1);
        }
    }
}

void blockAnalysis() {
//    printf("<bloco> -> ");
    free(tk.lexema);
    tk = lexical();
    variableStageAnalysis();
    subroutineAnalysis();
    commandAnalysis();
}

void syntactical() {
//    printf("<programa> -> ");
    setupSymbolsTable();
    tk = lexical();

    if (tk.simbolo == sprograma){
        free(tk.lexema);
        tk = lexical();

        if (tk.simbolo == sidentificador){
//            printf("programa %s", tk.lexema);
            insertTable(tk.lexema, tnomedeprograma, 0, "");
            codeGen("   ", "START  ", "   ", "   ");
            free(tk.lexema);
            tk = lexical();

            if (tk.simbolo == sponto_virgula){
//                printf(";\n");
                blockAnalysis();

                if (tk.simbolo == sponto){
                    // printf(".\n");
                    free(tk.lexema);
                    lexical();
                    if (c == EOF || c == '{'){
                        unstackLevel();
                        codeGen("   ", "HLT    ", "   ", "   ");
                    } else {
                        printf("Error in line %d: Code outside of the program scope.\n", line);
                        exit(-1);
                    }
                } else {
                    printf("Error in line %d: '.' not found at the end of the program.\n", line);
                    exit(-1);
                }
            } else {
                printf("Error in line %d: Program declaration lacking ';'.\n", line);
                exit(-1);
            }
        } else {
            printf("Error in line %d: Program declaration needs to take in a name. Example: programa name;\n", line);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Code needs to start with a program declaration. Example: programa name;\n", line);
        exit(-1);
    }
}

#endif
