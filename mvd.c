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
    } else if (strcmp(tk.lexema, "ijmp") == 0){
        *s = c;
    }

    c = fgetc(file);
    
    return tk;
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    int execpile[1000], s = -1;

    file = fopen(argv[1], "r");

    c = fgetc(file); 

    while(c != EOF){
        getInstruction(execpile, &s);
    }
    return 0;
}










int findJmp(const char *linha, int numbj, int linenumb){
    if (linha == NULL) return 0; // maybe change this verification later

    char numbuf[32];
    snprintf(numbuf, sizeof(numbuf), "%d", numbj);
    size_t nlen = strlen(numbuf);

    for (const char *p = linha; *p != '\0'; ++p){
        printf("analisando comando: %.4s\n", p);
        if (strncmp(p, "JMPF", 4) == 0) {
            //printf("achamo um jmpf aqui vamos trocar o numero dele\n");
            const char *q = p + 4;
            while (isspace((unsigned char)*q)) q++;
            if (strncmp(q, numbuf, nlen) == 0 && 
                (q[nlen] == '\0' || isspace((unsigned char)q[nlen]) || ispunct((unsigned char)q[nlen]))) {
                return (int)(q - linha); 
            }
        }
        if (strncmp(p, "JMP", 3) == 0) {
            //printf("achamo um jmpf aqui vamos trocar o numero dele\n");
            const char *q = p + 3;
            while (isspace((unsigned char)*q)) q++;
            if (strncmp(q, numbuf, nlen) == 0 &&
                (q[nlen] == '\0' || isspace((unsigned char)q[nlen]) || ispunct((unsigned char)q[nlen]))) {
                return (int)(q - linha);
            }
        }
    }
    return 0;
}