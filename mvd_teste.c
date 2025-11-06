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

// this function change the number on the begin of the line
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
        memmove(&line[start + newlen], &line[start +     oldlen], len - (start + oldlen) + 1);
    } else {
        /* need to move remainder to the right first, then copy */
        memmove(&line[start + newlen], &line[start + oldlen], len - (start + oldlen) + 1);
        memcpy(&line[start], numbuf, newlen);
    }
}

#include <string.h>

// Função ajustada com trava de segurança para buffer de 24 posições
char* addCaracter(char line[], int linePos, int charToAdd) {
    int len = (int)strlen(line);

    // TRAVA DE SEGURANÇA 1: Buffer Cheio
    // Se já temos 23 caracteres, não cabe mais nenhum (o 24º é o \0).
    if (len >= 23) {
        // Retorna a linha original sem alterações para evitar crash
        return line; 
    }

    // TRAVA DE SEGURANÇA 2: Posição Inválida
    // Não podemos inserir antes do início (linePos < 0) 
    // nem muito depois do fim atual (linePos > len).
    if (linePos < 0 || linePos > len) {
        return line;
    }

    // MOVIMENTAÇÃO (Shifting)
    // Começa do \0 e vai movendo tudo um passo para a direita
    // para abrir espaço na posição 'linePos'.
    for (int i = len; i >= linePos; i--) {
        line[i + 1] = line[i];
    }

    // INSERÇÃO
    line[linePos] = (char)charToAdd;

    printf("linha apos addcaracter: %s\n", line);

    // RETORNO
    // Devolve o ponteiro para a própria linha modificada
    return line;
}

// this one is use to find the jump
void findJmp(char linebuffer[][buffer], char numbj[], int linenumb, int x){

    char line[buffer]; // use to take a line from the linebuffer and look on it for the jump 
    char *found_match = NULL; // use to check for the jump label on the line
    int flagL = 0;// flag for check if the label had already been change or not
    int Lpos = 0; // position of the L on the line
    int numpos = 0; // position of the number on the line
    printf("jump label dentro do findjmp: %s\n",numbj);

    for (int i = 0; i < linenumb; i++){ // will run thruogh the entire document looking for JMPF or JMP in this order
        strcpy(line, linebuffer[i]);
        
        char *p = NULL; 
        int jmp_len = 0; 

        p = strstr(line, "JMPF");
        if (p != NULL) {
            jmp_len = 4;

            p += jmp_len;

            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++;
            }

            // check for 'L' indicating a label and save its position
            if (*p == 'L') {
                flagL = 1;
                printf("flagL setado para 1\n");
                Lpos = (int)(p - line);
                p++;
            }

            // find the position of the number in the line
            found_match = strstr(line, numbj);
            if (found_match != NULL) {
                numpos = (int)(found_match - line);
            }

            printf("found_match dentro do findjmp: %s\n", found_match);

            if (found_match != 0 && flagL == 1) {
                
                int pos = (int)(p - line);
                printf("posicao do L dentro do findjmp: %c\n", line[Lpos]);
                line[Lpos] = '\0';
                addCaracter(line, numpos, x);
                printf("a linha atualizada eh: %s\n", line);

            }
            
        } 

        p = strstr(line, "JMP");
        if (p != NULL) {
            jmp_len = 3;

            p += jmp_len;

            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++; 
            }

            if (isdigit((unsigned char)*p)) {
                

                int extracted_num = atoi(p);

                int label_num = atoi(numbj);
                if (extracted_num == label_num) {
                    int pos = (int)(p - line);
                    replace_number_at(linebuffer[i], buffer, pos, x);
                    
                }
            } 
            
        } 
    }
}



void commandPile(token P[], int *i, FILE *file){

    char line [buffer]; 
    char linebuffer[1000][buffer]; // line vector
    int linenumb = 0, x = 1;
    char numbj[3], L;// variable to save the jump number provisionally

    // reading and storing the lines on the line vector
    while (fgets(line, buffer, file) != NULL){
        strcpy(linebuffer[linenumb], line);
        linenumb++;
    }
    
    // check for jump commands
    for(; x < linenumb; x ++){
        strcpy(line, linebuffer[x]);

        for (int y = 0; y < 4; y++){

            if(isdigit((unsigned char)line[y])){
                    
                if (y + 1 < 4 && isdigit((unsigned char)line[y+1])) {
                        
                    numbj[0] = line[y];
                    numbj[1] = line[y+1];
                    numbj[2] = '\0';
                        
                    printf("numero do numbj %c%c\n", numbj[0], numbj[1]);

                    replace_number_at(line, buffer, y, x + 1); //yep a code without gambiarras is not a code
                    strcpy(linebuffer[x], line);
                    printf("numero da linha passada pro findjump %d\n", x + 1);
                    findJmp(linebuffer, numbj, linenumb, x + 1); // don't ask me why but it works 
                    
                } else {
                        
                    numbj[0] = line[y];
                    numbj[1] = '\0';


                    printf("numero do numbj %c\n", numbj);
                        
                    replace_number_at(line, buffer, y, x + 1); // it was happen here to 
                    strcpy(linebuffer[x], line);
                    printf("numero da linha passada pro findjump %d\n", x + 1);
                    findJmp(linebuffer, numbj, linenumb, x + 1); // same here
                }


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