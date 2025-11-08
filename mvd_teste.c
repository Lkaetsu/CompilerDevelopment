#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#define buffer 24

enum instructions {ildc, ildv, iadd, isub, imult, idivi,
    iinv, iand, ior, ineg, icme, icma, iceq,
    icdif, icmeq, icmaq, ijmp, ijmpf, inum};
    
    typedef struct token{ //mudar o nome nao coloque token 
        char *lexema;
        int tipo;
    }token;
    
int c;
FILE *file;

void intToStr(int N, char *str) {
    int i = 0;
  
    // Save the copy of the number for sign
    int sign = N;

    // If the number is negative, make it positive
    if (N < 0)
        N = -N;

    // Extract digits from the number and add them to the
    // string
    while (N > 0) {
      
        // Convert integer digit to character and store
      	// it in the str
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 

    // If the number was negative, add a minus sign to the
    // string
    if (sign < 0) {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

// this one is use to find the jump
void findJmp(char linebuffer[][buffer], char numbj[], int linenumb, int x){

    char line[buffer]; // use to take a line from the linebuffer and look on it for the jump 
    char *found_match = NULL; // use to check for the jump label on the line
    int flagL = 0;// flag for check if the label had already been change or not
    int Lpos = 0; // position of the L on the line
    int numpos = 0; // position of the number on the line
    char number_to_string[12]; // hold the string version of x to be add on the line

    for (int i = 0; i < linenumb; i++){ // will run thruogh the entire document looking for JMPF or JMP in this order
        strcpy(line, linebuffer[i]);
        
        char *p = NULL; 
        int jmp_len = 0; 

        // looking for JMPF on the program
        p = strstr(line, "JMPF");
        if (p != NULL) {
            jmp_len = 4;
            p += jmp_len;// jumping to after the command

            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++;
            }

            // check for 'L' indicating a label and save its position
            if (*p == 'L') {
                flagL = 1;
                Lpos = (int)(p - line);
                p++;
            }

            // find the number and his position in the line
            found_match = strstr(line, numbj);
            if (found_match != NULL) {
                numpos = (int)(found_match - line);
            }

            // if it find the number on the line it will change it to x that contain the label line
            if (found_match != 0 && flagL == 1) { 
                int pos = (int)(p - line);
                line[Lpos] = '\0';
                intToStr(x,number_to_string);
                strcat(line,number_to_string);
                strcat(line,"\n");
                strcpy(linebuffer[i], line);
                flagL = 0;
            }  
        } 

        // looking for JMP on the program
        p = strstr(line, "JMP");
        if (p != NULL) {
            jmp_len = 3;
            p += jmp_len;// jumping to after the command

            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++; 
            }

            // check for 'L' indicating a label and save its position
            if (*p == 'L') {
                flagL = 1;
                Lpos = (int)(p - line);
                p++;
            }
        
            // find the number and his position in the line
            found_match = strstr(line, numbj);
            if (found_match != NULL) {
                numpos = (int)(found_match - line);
            }

            // if it find the number on the line it will change it to x that contain the label line
            if (found_match != 0 && flagL == 1) { 
                int pos = (int)(p - line);
                line[Lpos] = '\0';
                intToStr(x,number_to_string);
                strcat(line,number_to_string);
                strcat(line,"\n");
                strcpy(linebuffer[i], line);
                flagL = 0;
            } 
        } 

        // looking for CALL on the program
        p = strstr(line, "CALL");
        if (p != NULL) {
            jmp_len = 4;
            p += jmp_len;// jumping to after the command

            while (*p != '\0' && isspace((unsigned char)*p)) {
                p++; 
            }

            // check for 'L' indicating a label and save its position
            if (*p ==  'L') {
                flagL = 1;
                Lpos = (int)(p - line);
                p++;
            }
        
            // find the number and his position in the line
            found_match = strstr(line, numbj);
            if (found_match != NULL) {
                numpos = (int)(found_match - line);
            }

            // if it find the number on the line it will change it to x that contain the label line
            if (found_match != 0 && flagL == 1) {  
                int pos = (int)(p - line);
                line[Lpos] = '\0';
                intToStr(x,number_to_string);
                strcat(line,number_to_string);
                strcat(line,"\n");
                strcpy(linebuffer[i], line);
                flagL = 0;
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
                    printf("numero da linha passada pro findjump %d\n", x + 1);
                    findJmp(linebuffer, numbj, linenumb, x + 1); 
                    break;
                    
                } else {
                        
                    numbj[0] = line[y];
                    numbj[1] = '\0';


                    printf("numero do numbj %c\n", numbj[0]);
                    printf("numero da linha passada pro findjump %d\n", x + 1);
                    findJmp(linebuffer, numbj, linenumb, x + 1);
                    break;
                }
            }
        }
    }
    
    // debug prints
    printf("quantidade de linhas:%d\n", linenumb);
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