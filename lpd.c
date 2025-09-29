#include "lexical.h"
#include "syntax.h"
#include "semantic.h"

int main(int argc, char *argv[])
{
    // No filename sent
    if (argc != 2)
    {
        printf("Usage: ./lpd <filename>\n");
        return 1;
    }

    FILE *codeFile = fopen(argv[1], "r");

    // Could not open file
    if (codeFile == NULL){
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }
    printf("Opening file: %s\n", argv[1]);
    token tk;
    lexicalSetup(codeFile);

    while(c != EOF)
    {
        tk = lexical();
        printf("[%s]: %d\n",tk.lexema, tk.simbolo);
    }
    fclose(file);
    return 0;
}
