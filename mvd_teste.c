#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define buffer 24

enum instructions {ildc, ildv, iadd, isub, imult, idivi,
                   iinv, iand, ior, ineg, icme, icma, iceq,
                   icdif, icmeq, icmaq, ijmp, ijmpf, inum};

typedef struct token{
    char *lexema;
    int tipo;
}token;



int c;
FILE *file;

void commandPile(token P[], int *i, FILE *file){

    char line [buffer]; 
    char linebuffer[1000][buffer]; // line vector
    int linenumb = 0;
    int numbj = 0;// variable to save the jump number provisionally

    // reading and storing the lines on the line vector
    while (fgets(line, buffer, file) != NULL){
        strcpy(linebuffer[linenumb], line);
        linenumb++;
    }
    
    // debug prints
    // printf("%d\n", linenumb);
    // for(int x = 0; x < linenumb; x++){
    //     printf("%s", linebuffer[x]);
    // }

    // check for jump commands
    for(int x = 0; x <= linenumb; x ++){
        strcpy(line, linebuffer[x]);
        for (int y = 0; y <= 4; y ++){
            if (line[y] != NULL){
                numbj = line[y];
                line[y] = x;
                for (int z = 0; z <= linenumb; z ++){
                    strcpy(line, linebuffer[z]);
                    for(int w = 0; )
                }
            }
        }
    }

}

// pegar coisa do vetor de struct não do arquivo 
token getInstruction(token P[], int M[], int *s, int *i){
    token tk;
    tk.lexema = (char *) malloc(20 * sizeof(char));
    //int i = 0;

    if(strcmp(tk.lexema, "ildc") == 0){
        *s = *s + 1; 
        M[*s] = c;
    } else if(strcmp(tk.lexema, "ildv") == 0){
        *s = *s + 1;
        M[*s] = c;
    } else if(strcmp(tk.lexema, "iadd") == 0){
        M[*s - 1] = M[*s - 1] + M[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "isub") == 0){
        M[*s - 1] = M[*s - 1] - M[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "imult") == 0){
        M[*s - 1] = M[*s - 1] * M[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "idivi") == 0){
        M[*s - 1] = M[*s - 1] / M[*s];
        *s = *s - 1;
    } else if(strcmp(tk.lexema, "iinv") == 0){
        M[*s] = -(M[*s]);
    } else if(strcmp(tk.lexema, "iand") == 0){
        if(M[*s - 1] == 1 && M[*s] == 1){
			M[*s - 1] = 1;
		}
		else{
			M[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(tk.lexema, "ior") == 0){
        if(M[*s - 1] == 1 || M[*s] == 1){
			M[*s - 1] = 1;
		}
		else{
			M[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(tk.lexema, "ineg") == 0){
		M[*s] = 1 - M[*s];
    } else if(strcmp(tk.lexema, "icme") == 0){
        if(M[*s - 1] < M[*s]){
            M[*s - 1] = 1;
        }
        else {
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icma") == 0){
        if(M[*s - 1] > M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "iceq") == 0){
        if(M[*s - 1] == M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icdif") == 0){
        if(M[*s - 1] != M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }        
    } else if(strcmp(tk.lexema, "icmeq") == 0){
        if(M[*s - 1] <= M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(tk.lexema, "icmaq") == 0){
        if(M[*s - 1] >= M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } 
    // else if (strcmp(tk.lexema, "ijmp") == 0){
    //     *s = c; 
    // } else if (strcmp(tk.lexema, "ijmpf") == 0){
    //     if ((M[*s] = 0) ){
    //         *s = c;
    //     }
    //     else{
    //         *s =  + 1;
    //     }
    // } else if (strcmp(tk.lexema, "inum") == 0){
        
    // }


    c = fgetc(file);
    
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    int M[1000];
    int i = 0; // registrador do programa
    int s = 0; // registrador da pilha de valores
    char c;
    token P[1000];

    file = fopen(argv[1], "r");

    commandPile(P, &i, file);

    while(i != i){
        getInstruction(P, M, &s, &i);
        i = i + 1;
    }
    return 0;
}