#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum instructions {ildc, ildv, iadd, isub, imult, idivi,
                   iinv, iand, ior, ineg, icme, icma, iceq,
                   icdif, icmeq, icmaq, ijmp, ijmpf, inum};

typedef struct token{
    char *lexema;
    int tipo;
}token;



int c;
FILE *file;

void commandPile(int commands[], int *pc){
    commands[*pc] = c;
    *pc = *pc + 1;
}


int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    int execpile[1000], s = -1;
    int commands[1000], pc = 0;

    file = fopen(argv[1], "r");

    c = fgetc(file); 

    while (c != EOF){
        commandPile(commands, &pc);
    }

    // while(c != EOF){
    //     getInstruction(execpile, &s);
    // }
    return 0;
}