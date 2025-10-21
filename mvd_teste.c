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

// Função para ler os tk marcar os tipo e arquivalos 
token commandPile(token commands[], int *pc){
    token tk;
    tk.lexema = (char *) malloc(20 * sizeof(char));
    int i = 0;

    while(c != ' ' && c != '\n' && c != EOF){
        tk.lexema[i] = c;
        i++;
        c = fgetc(file);
    }
    tk.lexema[i] = '\0';
    if(c == ' '){
        c = fgetc(file);
    }
    if(c == '\n'){
        c = fgetc(file);
    }
    if (strcmp(tk.lexema, "ildc") == 0){
        tk.tipo = ildc;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "ildv") == 0){
        tk.tipo = ildv;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "iadd") == 0){
        tk.tipo = iadd;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "isub") == 0){
        tk.tipo = isub;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "imult") == 0){
        tk.tipo = imult;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "idivi") == 0){
        tk.tipo = idivi;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "iinv") == 0){
        tk.tipo = iinv;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "iand") == 0){
        tk.tipo = iand;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "ior") == 0){
        tk.tipo = ior;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "ineg") == 0){
        tk.tipo = ineg;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "icme") == 0){
        tk.tipo = icme;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "icma") == 0){
        tk.tipo = icma;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "iceq") == 0){
        tk.tipo = iceq;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "icdif") == 0){
        tk.tipo = icdif;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "icmeq") == 0){
        tk.tipo = icmeq;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "icmaq") == 0){
        tk.tipo = icmaq;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "ijmp") == 0){
        tk.tipo = ijmp;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "ijmpf") == 0){
        tk.tipo = ijmpf;
        commands[*pc] = tk;
        *pc = *pc + 1;
    } else if (strcmp(tk.lexema, "inum") == 0){
        tk.tipo = inum;
        commands[*pc] = tk;
        *pc = *pc + 1;
    }

    c = fgetc(file);

    return tk;
}

token getInstruction(int execpile[], int *s){
    token tk;
    tk.lexema = (char *) malloc(20 * sizeof(char));
    int i = 0;

    while(c != ' ' && c != '\n' && c != EOF){
        tk.lexema[i] = c;
        i++;
        c = fgetc(file);
    }
    tk.lexema[i] = '\0';
    if(c == ' '){
        c = fgetc(file);
    }
    if(c == '\n'){
        c = fgetc(file);
    }
    if(strcmp(tk.lexema, "ildc") == 0){
        *s = *s + 1; 
        execpile[*s] = c;
    } else if(strcmp(tk.lexema, "ildv") == 0){
        *s = *s + 1;
        execpile[*s] = c;
    } else if(strcmp(tk.lexema, "iadd") == 0){
        execpile[*s - 1] = execpile[*s - 1] + execpile[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "isub") == 0){
        execpile[*s - 1] = execpile[*s - 1] - execpile[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "imult") == 0){
        execpile[*s - 1] = execpile[*s - 1] * execpile[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "idivi") == 0){
        execpile[*s - 1] = execpile[*s - 1] / execpile[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "iinv") == 0){
        execpile[*s] = -(execpile[*s]);
    } else if(strcmp(tk.lexema, "iand") == 0){
        if(execpile[*s - 1] == 1 && execpile[*s] == 1){
			execpile[*s - 1] = 1;
		}
		else{
			execpile[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(tk.lexema, "ior") == 0){
        if(execpile[*s - 1] == 1 || execpile[*s] == 1){
			execpile[*s - 1] = 1;
		}
		else{
			execpile[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(tk.lexema, "ineg") == 0){
		execpile[*s] = 1 - execpile[*s];
    } else if(strcmp(tk.lexema, "icme") == 0){
        if(execpile[*s - 1] < execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else {
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icma") == 0){
        if(execpile[*s - 1] > execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else{
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "iceq") == 0){
        if(execpile[*s - 1] == execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else{
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icdif") == 0){
        if(execpile[*s - 1] != execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else{
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }        
    } else if(strcmp(tk.lexema, "icmeq") == 0){
        if(execpile[*s - 1] <= execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else{
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icmaq") == 0){
        if(execpile[*s - 1] >= execpile[*s]){
            execpile[*s - 1] = 1;
        }
        else{
            execpile[*s - 1] = 0;
            *s = *s - 1;
        }
    } //else if (strcmp(tk.lexema, "ijmp") == 0){
       
    // } else if (strcmp(tk.lexema, "ijmpf") == 0){

    // } else if (strcmp(tk.lexema, "inum") == 0){
        
    // }


    c = fgetc(file);
    
    return tk;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    int execpile[1000], s = -1, pc = 0;
    token commands[1000];

    file = fopen(argv[1], "r");

    c = fgetc(file); 

    // Primeiro leio os tk marco o tipo e armazeno em uma pilha para 
    // depois durante a interpretação se tiver um jmp ele busca o tk para onde 
    // o programa saltou nessa pilha
    while (c != EOF){
        commandPile(commands, &pc);
    }

    while(c != EOF){
        getInstruction(execpile, &s);
    }
    return 0;
}