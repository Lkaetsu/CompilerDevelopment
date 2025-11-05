#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
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


/* Replace the number (sequence of digits) that starts at position pos in
   'line' with the decimal text of 'value'. line_size is the size of the
   buffer 'line' points to (to avoid overflows). This function modifies
   'line' in-place. If the new text doesn't fit, the function returns
   without changing the line. */
void replace_number_at(char *line, size_t line_size, int pos, int value) {
    if (line == NULL || pos < 0) return;
    size_t len = strlen(line);
    if ((size_t)pos >= len) return;

    size_t start = (size_t)pos;
    /* find end of digit sequence starting at start */
    size_t k = start;
    while (k < len && isdigit((unsigned char)line[k])) k++;
    size_t oldlen = k - start;

    char numbuf[32];
    int n = snprintf(numbuf, sizeof(numbuf), "%d", value);
    if (n < 0) return; /* snprintf error */
    size_t newlen = (size_t)n;

    /* Check space */
    if (len + (newlen > oldlen ? newlen - oldlen : 0) + 1 > line_size) {
        /* not enough space to perform replacement safely */
        return;
    }

    if (newlen <= oldlen) {
        /* overwrite and shift left the remainder */
        memcpy(&line[start], numbuf, newlen);
        memmove(&line[start + newlen], &line[start + oldlen], len - (start + oldlen) + 1);
    } else {
        /* need to move remainder to the right first, then copy */
        memmove(&line[start + newlen], &line[start + oldlen], len - (start + oldlen) + 1);
        memcpy(&line[start], numbuf, newlen);
    }
}


int findJmp(const char *linha, int numbj ){
    if (linha == NULL) return 0; // maybe change this verification later

    char numbuf[32];
    snprintf(numbuf, sizeof(numbuf), "%d", numbj);
    size_t nlen = strlen(numbuf);

    for (const char *p = linha; *p != '\0'; ++p){ //the problem is somewhere over here
        if (strncmp(p, "JMPF", 4) == 0) {
            //printf("achamo um jmpf aqui vamos trocar o numero dele\n");
            const char *q = p + 4;
            while (isspace((unsigned char)*q)) q++;
            if (strncmp(q, numbuf, nlen) == 0 && // or over here
                (q[nlen] == '\0' || isspace((unsigned char)q[nlen]) || ispunct((unsigned char)q[nlen]))) {
                return (int)(q - linha); // or here
            }
        }
        if (strncmp(p, "JMP", 3) == 0) {
            //printf("achamo um jmpf aqui vamos trocar o numero dele\n");
            const char *q = p + 3;
            while (isspace((unsigned char)*q)) q++;
            if (strncmp(q, numbuf, nlen) == 0 &&
                (q[nlen] == '\0' || isspace((unsigned char)q[nlen]) || ispunct((unsigned char)q[nlen]))) {
                return (int)(q - linha);// maybe here
            }
        }
    }
    return 0;
}


void commandPile(token P[], int *i, FILE *file){

    char line [buffer]; 
    char linebuffer[1000][buffer]; // line vector
    int linenumb = 0;
    int numbj;// variable to save the jump number provisionally

    // reading and storing the lines on the line vector
    while (fgets(line, buffer, file) != NULL){
        strcpy(linebuffer[linenumb], line);
        linenumb++;
    }
    
    
    
    // check for jump commands
    for(int x = 0; x < linenumb; x ++){
        strcpy(line, linebuffer[x]);
        printf("analisando linha %d: %s", x, line);
        for (int y = 0; y < 4; y++){
            if(isdigit((unsigned char)line[y])){
                printf("o numero do line[y] eh: %c\n", line[y]);
                numbj = line[y] - '0';
                printf("numero do numbj %d\n", numbj);
                replace_number_at(line, buffer, y, x );
                strcpy(linebuffer[x], line);

            }
        }
    }
    
    // debug prints
    printf("%d\n", linenumb);
    for(int x = 0; x < linenumb; x++){
        printf("%s", linebuffer[x]);
    }
    
}

// taking the things from the vector and executing they
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
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }
    printf("Arquivo aberto: %s\n", argv[1]);

    commandPile(P, &i, file);

    while(i != i){
        getInstruction(P, M, &s, &i);
        i = i + 1;
    }
    return 0;
}