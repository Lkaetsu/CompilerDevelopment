#include <stdio.h>
#include <stdlib.h>

enum instructions {ildc, ildv, iadd, isub, imult, idivi,
                   iinv, iand, ior, ineg, icme, icma, iceq,
                   icdif, icmeq, icmaq, ijmp, ijmpf, inum};

typedef struct token{
    char *lexema;
    int tipo;
}token;

int c;

token getToken(){

}

void getInstruction(){
    
    c = fgetc();
}

void getCommand(){
    getInstruction()
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    c = fgetc(); 

    while(c != EOF){
        getCommand();
    }
    return 0;
}
