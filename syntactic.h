#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include "lexical.h"
#include "semantic.h"

token tk;
int label;

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

void funcCallAnalysis(){
    printf("<chamada de funcao>\n");
    tk = lexical();
    // Nothing to do on syntactic
}

void procCallAnalysis(){
    printf("<chamada de procedimento>\n");
    // Nothing to do on syntactic
}

void factorAnalysis(){
    printf("<fator> -> ");
    int ind;
    if (tk.simbolo == sidentificador){      // Variable or function
        printf("%s ", tk.lexema);
//        ind = searchTable(tk.lexema, level, ind);
//        if (ind){
//            if (symbolsTable.id[ind].type == tfuncaointeiro ||
//                symbolsTable.id[ind].type == tfuncaobooleana)
//            {
//                funcCallAnalysis();
//            } else {
                tk = lexical();
//            }
//        } else {
//            printf("Error in line %d: Identifier '%s' used, but not declared.", line, tk.lexema);
//            exit(-1);
//        }
    } else {
        if (tk.simbolo == snumero){         // Number
            printf("%s ", tk.lexema);
            tk = lexical();

        } else if (tk.simbolo == snao){
            printf("%s ", tk.lexema);
            tk = lexical();
            factorAnalysis();
        } else if (tk.simbolo == sabre_parenteses){
            printf("%s ", tk.lexema);
            tk = lexical();
            expressionAnalysis();

            if (tk.simbolo == sfecha_parenteses){
                printf("%s ", tk.lexema);
                tk = lexical();
            } else {
                printf("Error in line %d: Unclosed '(' in expression.");
                exit(-1);
            }
        } else if (tk.simbolo == sverdadeiro || tk.simbolo == sfalso){
            printf("%s ", tk.lexema);
            tk = lexical();
        } else {
            printf("Error in line %d: Unexpected token '%s' in place of an expression.", line, tk.lexema);
            exit(-1);
        }
    }
}

void termAnalysis(){
    printf("<termo> -> ");
    factorAnalysis();
    while(tk.simbolo == smult || tk.simbolo == sdiv ||
          tk.simbolo == se)
    {
        printf("%s ", tk.lexema);
        tk = lexical();
        factorAnalysis();
    }
}

void simpleExpressionAnalysis(){
    printf("<expressao simples> -> ");
    if (tk.simbolo == smais || tk.simbolo == smenos){
        printf("%s ", tk.lexema);
        tk = lexical();
    }
    termAnalysis();
    while(tk.simbolo == smais || tk.simbolo == smenos ||
          tk.simbolo == sou)
    {
        printf("%s ", tk.lexema);
        tk = lexical();
        termAnalysis();
    }
}

void expressionAnalysis(){
    printf("<expressao> -> ");
    simpleExpressionAnalysis();
    if (tk.simbolo == smaior || tk.simbolo == smaiorig ||
        tk.simbolo == smenor || tk.simbolo == smenorig ||
        tk.simbolo == sdif)
    {
        printf("%s ", tk.lexema);
        tk = lexical();
        simpleExpressionAnalysis();
    }
}

void funcDeclAnalysis(){
    printf("<declaracao de funcao> -> funcao ");
    tk = lexical();
    char level = 'L';       // Marks the start of the function for searches
    if (tk.simbolo = sidentificador){
        printf("%s", tk.lexema);
//        if (!searchDeclProcTable(tk.lexema)){
//            insertTable(tk.lexema, tprocedimento, level, ""/* label*/);
            tk = lexical();
            if (tk.simbolo == sdoispontos) {
                tk = lexical();

                if (tk.simbolo == sinteiro || tk.simbolo == sbooleano){
                    if (tk.simbolo == sinteiro){
                        symbolsTable.id[symbolsTable.top].type = tfuncaointeiro;
                    } else {
                        symbolsTable.id[symbolsTable.top].type = tfuncaobooleana;
                    }
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
//        } else {
//            printf("Error in line %d: '%s' has already been previously declared.", line, tk.lexema);
//            exit(-1);
//        }
    } else {
        printf("Error in line %d: Unexpected token '%s' in place of function name.", line, tk.lexema);
        exit(-1);
    }
//    unstackLevel();
}

void procDeclAnalysis(){
    printf("<declaracao de procedimento> -> procedimento ");
    tk = lexical();
    char level = 'L';       // Marks the start of the procedure for searches
    if (tk.simbolo = sidentificador){
        printf("%s", tk.lexema);
//        if (!searchDeclProcTable(tk.lexema)){
//            insertTable(tk.lexema, tprocedimento, level, ""/* label*/);
//            codeGen(label, NULL, ´´, ´´);       // Call will get this label from the symbolsTable
//            label++;
            tk = lexical();
            if (tk.simbolo == sponto_virgula){
                printf(";\n");
                blockAnalysis();
            } else {
                printf("Error in line %d: Subroutine declarations must be ended by ';'.\n", line);
                exit(-1);
            }
//        } else {
//            printf("Error in line %d: '%s' has already been previously declared.", line, tk.lexema);
//            exit(-1);
//        }
    } else {
        printf("Error in line %d: Unexpected token '%s' in place of procedure name.", line, tk.lexema);
        exit(-1);
    }
//    unstackLevel();
}

void subroutineAnalysis(){
//    int auxrot, flag = 0;
    if (tk.simbolo == sprocedimento || tk.simbolo == sfuncao){
        printf("<etapa de declaracao de sub-rotinas> -> ");
//        auxrot = label;
//        codeGen(´´, JMP, label, ´´);      // Jump subroutines
//        label++;
//        flag = 1;
    }
    while (tk.simbolo == sprocedimento || tk.simbolo == sfuncao){
        if (tk.simbolo == sprocedimento){
            procDeclAnalysis();
        } else {
            funcDeclAnalysis();
        }
        if (tk.simbolo == sponto_virgula){
            tk = lexical();
        } else {
            printf("Error in line %d: Subroutine declarations must be ended by ';'.\n", line);
            exit(-1);
        }
    }
//    if (flag == 1) {
//        codeGen(auxrot, NULL, ´´, ´´);      // Beginning of main
//    }
}

void ifAnalysis(){
    printf("<comando condicional> -> se ");
    tk = lexical();
    expressionAnalysis();
    if (tk.simbolo == sentao){
        printf("entao \n");
        tk = lexical();
        simpleCommandAnalysis();
        if (tk.simbolo == ssenao){
            printf("senao \n");
            tk = lexical();
            simpleCommandAnalysis();
        }
    } else {
        printf("Error in line %d: There must be a 'entao' after 'se(<condition>)'. Example: se(verdadeiro) entao ...\n", line);
        exit(-1);
    }
}

void whileAnalysis(){
//    int auxrot1, auxrot2;
//    auxrot1 = label;
//    codeGen(label, NULL, ´´, ´´);
//    label++;
    printf("<comando enquanto> -> enquanto \n");
    tk = lexical();
    expressionAnalysis();
    if (tk.simbolo == sfaca){
        printf("faca \n");
//        auxrot2 = label;
//        codeGen(´´, JMPF, label, ´´);   // Jump if false
//        label++;
        tk = lexical();
        simpleCommandAnalysis();
//        codeGen(´´, JMP, auxrot1, ´´);  // Return to start of the loop
//        codeGen(auxrot2, NULL, ´´, ´´); // End of the while loop
    } else {
        printf("Error in line %d: There must be a 'faca' after 'enquanto(<condition>)'. Example: enquanto(verdadeiro) faca ...\n", line);
        exit(-1);
    }
}

void writeAnalysis(){
    printf("<comando escrita> -> escreva");
    tk = lexical();
    if (tk.simbolo == sabre_parenteses){
        printf("(");
        tk = lexical();
        if (tk.simbolo == sidentificador){
            printf("%s", tk.lexema);
//            if (searchDeclVarFuncTable(tk.lexema)){
                tk = lexical();
                if (tk.simbolo == sfecha_parenteses){
                    printf(")");
                    tk = lexical();
                } else {
                    printf("Error in line %d: Unexpected token '%s' before ')'. Example: escreva(x);\n", line, tk.lexema);
                    exit(-1);
                }
//            } else {
//                printf("Error in line %d: Variable '%s' used, but not being declared.\n", line, tk.lexema);
//                exit(-1);
//            }
        } else {
            printf("Error in line %d: Unexpected token '%s' inside write command, a variable name was expected.\n", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' instead of '('. Example: escreva(x);\n", line, tk.lexema);
        exit(-1);
    }
}

void readAnalysis(){
    printf("<comando leitura> -> leia");
    tk = lexical();
    if (tk.simbolo == sabre_parenteses){
        printf("(");
        tk = lexical();
        if (tk.simbolo == sidentificador){
            printf("%s", tk.lexema);
//            if (searchDeclVarTable(tk.lexema)){
                tk = lexical();
                if (tk.simbolo == sfecha_parenteses){
                    printf(")");
                    tk = lexical();
                } else {
                    printf("Error in line %d: Unexpected token '%s' before ')'. Example: leia(x);\n", line, tk.lexema);
                    exit(-1);
                }
//            } else {
//                printf("Error in line %d: Variable '%s' used, but not being declared.\n", line, tk.lexema);
//                exit(-1);
//            }
        } else {
            printf("Error in line %d: Unexpected token '%s' inside read command, a variable name was expected.\n", line, tk.lexema);
            exit(-1);
        }
    } else {
        printf("Error in line %d: Unexpected token '%s' instead of '('. Example: leia(x);\n", line, tk.lexema);
        exit(-1);
    }
}

void attribAnalysis(){
    tk = lexical();
    expressionAnalysis();
}

void attribProcCallAnalysis(){
    printf("<atribuicao_chprocedimento> -> ");
    char *deriv_aux = tk.lexema;
    tk = lexical();
    if (tk.simbolo == satribuicao){
        printf("<comando atribuicao> -> %s := ", deriv_aux);
        attribAnalysis();
    } else {
        printf("<chamada de procedimento> -> %s", deriv_aux);
        procCallAnalysis();
    }
}

void simpleCommandAnalysis(){
    printf("<comando> -> ");
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
    printf("<comandos> -> ");
    if (tk.simbolo == sinicio){
        printf("inicio\n");
        tk = lexical();
        simpleCommandAnalysis();
        while(tk.simbolo != sfim){
            if (tk.simbolo == sponto_virgula){
                printf(";\n");
                tk = lexical();
                if (tk.simbolo != sfim){
                    simpleCommandAnalysis();
                }
            } else {
                printf("Error in line %d: There must be a ';' in between commmands.\n", line);
                exit(-1);
            }
        }
        tk = lexical();
    } else {
        printf("Error in line %d: The code's commands must be started by 'inicio' and be ended with 'fim'.\n", line);
        exit(-1);
    }
    printf("fim\n");
}

void typeAnalysis(){
    printf("<tipo> -> ");
    if (tk.simbolo != sinteiro && tk.simbolo != sbooleano){
        printf("Error in line %d: '%s' is not a valid type.\n", line, tk.lexema);
        exit(-1);
    }
    else{
        printf("%s", tk.lexema);
        placeTypeTable(tk.simbolo);
    }
    tk = lexical();
}

void variableAnalysis(){
    printf("<declaracao de variaveis> -> ");
    while(tk.simbolo != sdoispontos){
        if (tk.simbolo == sidentificador){
            printf("%s", tk.lexema);
//            if (!searchDuplicVarTable(tk.lexema)){
//                insertTable(tk.lexema, tvariavel, "", "");
                tk = lexical();

                if (tk.simbolo == svirgula || tk.simbolo == sdoispontos){
                    if (tk.simbolo == svirgula){
                        printf(", ");
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
//            } else {
//                printf("Error in line %d: Variable %s is already declared in this scope.\n", line, tk.lexema);
//                exit(-1);
//            }
        } else {
            printf("Error in line %d: Variable %s is already declared in this scope.\n", line, tk.lexema);
            exit(-1);
        }
    }
    printf(": ");
    tk = lexical();
    typeAnalysis();
}

void variableStageAnalysis(){
    if (tk.simbolo == svar){
        printf("<etapa de declaracao de variaveis> -> ");
        printf("var ", tk.lexema);
        tk = lexical();

        if (tk.simbolo == sidentificador){
            while (tk.simbolo == sidentificador){
                variableAnalysis();
                if (tk.simbolo == sponto_virgula){
                    printf(";\n");
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
    printf("<bloco> -> ");
    tk = lexical();
    variableStageAnalysis();
    subroutineAnalysis();
    commandAnalysis();
}

void syntactical() {
    printf("<programa> -> ");
    label = 1;
    tk = lexical();

    if (tk.simbolo == sprograma){
        tk = lexical();

        if (tk.simbolo == sidentificador){
            printf("programa %s", tk.lexema);
//            insertTable(tk.lexema, tnomedeprograma, "", "");
            tk = lexical();

            if (tk.simbolo == sponto_virgula){
                printf(";\n");
                blockAnalysis();

                if (tk.simbolo == sponto){
                    printf(".\n");
                    lexical();
                    if (c == EOF || c == '{'){
                        printf("Syntax Success");
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
