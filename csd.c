#include "lexical.h"
#include "syntactic.h"
#include "semantic.h"
#include "codegen.h"

int main(int argc, char *argv[])
{
    // No filename sent
    if (argc != 2)
    {
        printf("Usage: ./csd <filename>\n");
        return 1;
    }

    char filename[256];
    strncpy(filename, argv[1], 256);
    FILE *codeFile = fopen(filename, "r");
    strncat(filename, ".o", 3);
    FILE *outputFile = fopen(filename, "w");

    // Could not open file
    if (codeFile == NULL){
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

//    printf("Opening file: %s\n", argv[1]);

    token tk;
    lexicalSetup(codeFile);
    codegenSetup(outputFile);

///   Code for getting all tokens and testing lexical
//    while(c != EOF)
//    {
//        tk = lexical();
//        printf("[%s]: %d\n",tk.lexema, tk.simbolo);
//    }

    syntactical();

    fclose(outputFile);
    fclose(codeFile);

    return 0;
}
