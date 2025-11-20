#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#ifdef _WIN32
#include <conio.h> // _kbhit, _getch para detectar ESC no Windows
#endif
#define buffer 24

const char *instructionsVector[] = {
    "LDC", "LDV", "ADD", "SUB", "MULT", "DIVI",
    "INV", "AND", "OR", "NEG", "CME", "CMA", "CEQ", "CDIF", "CMEQ",
    "CMAQ", "JMPF", "JMP", "NUM", "CALL", "STR", "RD", "PRN", "START",
    "ALLOC", "DALLOC", "HLT", "RETURN","NULL"
};
    
// struct for the P stack
typedef struct instru{
    char type[8];
    int m;
    int n;
}instru;
    
int c;
int linenumb = 0;
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

// this one is use to find the jump and change the label number to the line where it had to jump to
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



void commandPile(instru P[], int *i, FILE *file){

    char line [buffer]; 
    char linebuffer[1000][buffer]; // line vector
    int  x = 0;
    char *instruFound; //instruction found on the line from the linebuffer 
    char isntruFvector[8]; // instruction from the instructionvector
    char numbj[3], L;// variable to save the jump number provisionally
    int  jumpInstruction = 8; // use to jump afeter the intruction on the line 

    // reading and storing the lines on the line vector
    while (fgets(line, buffer, file) != NULL){
        strcpy(linebuffer[linenumb], line);
        linenumb++;
    }
    
    // check for labels on the four first spaces of the lins
    for(; x < linenumb; x ++){
        strcpy(line, linebuffer[x]);

        for (int y = 0; y < 4; y++){

            if(isdigit((unsigned char)line[y])){
                    
                if (y + 1 < 4 && isdigit((unsigned char)line[y+1])) {
                    
                    // saving the label number to use it on the findJmp function 
                    numbj[0] = line[y];
                    numbj[1] = line[y+1];
                    numbj[2] = '\0';
                    
                    // this FOR remove the label from the string
                    for (int j = 0; j < 4; j++){
                        line[j] = ' ';
                    }
                    strcpy(linebuffer[x], line);

                    findJmp(linebuffer, numbj, linenumb, x + 1); 
                    break;
                    
                } else {

                    // saving the label number to use it on the findJmp function 
                    numbj[0] = line[y];
                    numbj[1] = '\0';

                    // this FOR remove the label from the string
                    for (int j = 0; j < 4; j++){
                        line[j] = ' ';
                    }
                    strcpy(linebuffer[x], line);


                    findJmp(linebuffer, numbj, linenumb, x + 1);
                    break;
                }
            }
        }
    }

    for(int i = 0; i < linenumb; i++){
        printf("%s",linebuffer[i]);
    }
    printf("\n\n");
    
    // populating the P stack
    for(int x = 0; x < linenumb; x++){
        strcpy(line, linebuffer[x]);
        for (int i = 0; i <= 28; i++){
            strcpy(isntruFvector, instructionsVector[i]);
            instruFound = strstr(line, isntruFvector);
            if(instruFound != NULL){
                strcpy (P[x].type , isntruFvector);
                if (strstr(isntruFvector, "ALLOC")){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = line[j] - '0';
                            j++;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].n = line[j] - '0';
                                    break;
                                }
                                j++;
                            }
                            break;
                        }
                    }
                    break;
                }else if(strstr(isntruFvector, "JMPF" )){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = 0;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].m = P[x].m * 10 + (line[j] - '0');
                                }
                                j++;
                            }
                            break;
                        }
                    }
                    break;
                }else if(strstr(isntruFvector, "JMP" )){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = 0;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].m = P[x].m * 10 + (line[j] - '0');
                                }
                                j++;
                            }
                            break;
                        }
                    }
                    break;
                }else if(strstr(isntruFvector, "CALL" )){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = 0;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].m = P[x].m * 10 + (line[j] - '0');
                                }
                                j++;
                            }
                            break;
                        }
                    }
                   break;
                }else if(strstr(instruFound,"LDV")){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = line[j] - '0';
                            break;
                        }
                    }
                    break;
                }else if(strstr(instruFound,"LDC")){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = 0;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].m = P[x].m * 10 + (line[j] - '0');
                                }
                                j++;
                            }
                            break;
                        }
                    }
                    break;
                }else if(strstr(instruFound,"STR")){
                    for(int j = 0; j < strlen(line); j++){
                        if (isdigit((unsigned char)line[j])){
                            P[x].m = 0;
                            while (j < strlen(line)){
                                if (isdigit((unsigned char)line[j])){
                                    P[x].m = P[x].m * 10 + (line[j] - '0');
                                }
                                j++;
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }   
}

// taking the things from the vector and executing they
int executionFunction(instru P[],int i, int M[], int *s, int *auxScan, int *auxPrint){
    
    printf("Executing P[%d]: %s %d %d\n", i, P[i].type, P[i].m, P[i].n);
    printf("Stack top (s=%d): \n", *s);
    for (int j = 0; j <= *s; j++) {
        printf("[%d]:%d \n",j, M[j]);
    }
    printf("\n"); 

    printf("valor de entrada do I:%d\n",i);
    if(strcmp(P[i].type, "LDC") == 0){
        *s = *s + 1; 
        M[*s] = P[i].m;
    } else if(strcmp(P[i].type, "LDV") == 0){
        *s = *s + 1;
        M[*s] = M[P[i].m];
    } else if(strcmp(P[i].type, "ADD") == 0){
        M[*s - 1] = M[*s - 1] + M[*s];
        *s = *s - 1;
    } else if(strcmp(P[i].type, "SUB") == 0){
        M[*s - 1] = M[*s - 1] - M[*s];
        *s = *s - 1;
    } else if(strcmp(P[i].type, "MULT") == 0){
        M[*s - 1] = M[*s - 1] * M[*s];
        *s = *s - 1;
    } else if(strcmp(P[i].type, "DIVI") == 0){
        M[*s - 1] = M[*s - 1] / M[*s];
        *s = *s - 1;
    } else if(strcmp(P[i].type, "INV") == 0){
        M[*s] = -(M[*s]);
    } else if(strcmp(P[i].type, "AND") == 0){
        if(M[*s - 1] == 1 && M[*s] == 1){
            M[*s - 1] = 1;
		}
		else{
            M[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(P[i].type, "OR") == 0){
        if(M[*s - 1] == 1 || M[*s] == 1){
            M[*s - 1] = 1;
		}
		else{
            M[*s - 1] = 0;
			*s = *s - 1;
		}
    } else if(strcmp(P[i].type, "NEG") == 0){
        M[*s] = 1 - M[*s];
    } else if(strcmp(P[i].type, "CME") == 0){
        if(M[*s - 1] < M[*s]){
            M[*s - 1] = 1;
        }
        else {
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(P[i].type, "CMA") == 0){
        if(M[*s - 1] > M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(P[i].type, "CEQ") == 0){
        if(M[*s - 1] == M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(P[i].type, "CDIF") == 0){
        if(M[*s - 1] != M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }        
    } else if(strcmp(P[i].type, "CMEQ") == 0){
        if(M[*s - 1] <= M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if(strcmp(P[i].type, "CMAQ") == 0){
        if(M[*s - 1] >= M[*s]){
            M[*s - 1] = 1;
        }
        else{
            M[*s - 1] = 0;
            *s = *s - 1;
        }
    } else if (strcmp(P[i].type, "JMPF") == 0){
        if ((M[*s] = 0) ){
            *s = P[i].m;
        }
        else{
            *s =  + 1;
        }
    } else if (strcmp(P[i].type, "JMP") == 0){
        i = P[i].m; 
        printf("valor que esta sendo retornado do I:%d\n",i);
        return i - 1;
    } else if (strcmp(P[i].type, "DALLOC") == 0){
        for (int k = (P[i].n - 1); k > 0; k --){
            M[P[i].m + k] = M[*s];
            *s = *s - 1;
        }
    } else if (strcmp(P[i].type, "ALLOC") == 0){
        for (int k = 0; k <= (P[i].n - 1) ; k++){
            *s = *s + 1;
            M[*s] = M[P[i].m + k];
        }
    } else if (strcmp(P[i].type, "CALL") == 0){
        *s = *s + 1;
        M[*s] = i + 1;
        i = P[i].m;
        return i;
    } else if (strcmp(P[i].type, "START") == 0) {
        *s = *s + 1;
    } else if (strcmp(P[i].type, "RETURN") == 0){
        i = M[*s];
        *s = *s - 1;
    }else if (strcmp(P[i].type, "HLT") == 0){
        
    }else if (strcmp(P[i].type, "RD") == 0){
        *s = *s + 1;
        printf("\ndigite o valor de entrada:");
        scanf("%d", &M[*s]);
    }else if (strcmp(P[i].type, "PRN") == 0){
        printf("%d",M[*s]);
    }else if (strcmp(P[i].type, "STR") == 0){
        M[P[i].m] = M[*s]; 
        *s = *s - 1;
    }else if (strcmp(P[i].type, "NULL") == 0){
        printf("\nNULL\n");
    }
    else {
        printf("esse comando nao foi feito ainda ;p\n");
    }
    
    
    i++;
    printf("valor que esta sendo retornado do I:%d\n",i);
    return i;
    
}

int main(int argc, char *argv[])
{
    
    int M[1000]; // The region of the data stack M that will contain the values ​​manipulated by the instructions of the MVD.

    instru P[1000]; // The program region P that will contain the MVD instructions.
    int i = 0; // registrador do programa
    int s = 0; // registrador da pilha de valores
    int auxScan = 0;// auxiliar variable for RD instruction
    int auxPrint = 0;// auxiliar variable for PRN istruction
    
    int counter = 0;
    char c;

    // cleaning the terminal after begin
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // checking for correct number of arguments
    if(argc != 2){
        printf("Usage: ./mvd <filename>.o\n");
        return 1;
    }

    // checking if the file can be opened
    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }
    printf("Arquivo aberto: %s\n", argv[1]);

    // populating the P stack
    commandPile(P, &i, file);

    // ================= Debugging For, It will print the intire P stack =================
    for (int o = 0; o < linenumb; o++){
        printf("P[%d]: %s %d %d\n", o, P[o].type, P[o].m, P[o].n);
    }

    // ================= Selection of execution mode =================
    int mode = 0;
    char inbuf[16];
    printf("\nSelecione o modo de execucao:\n");
    printf("1 - Direto (ate %d)\n", linenumb);
    printf("2 - Passo a passo (pressione Enter a cada instrucao)\n> ");
    if (fgets(inbuf, sizeof(inbuf), stdin) != NULL) {
        mode = atoi(inbuf);
    }
    if (mode != 2) mode = 1; // default to direct mode if invalid input

    int aborted = 0;
    if (mode == 2) {
        for (; counter < linenumb; counter++) {
            i = executionFunction(P, i, M, &s, &auxScan, &auxPrint);
            printf("(i=%d) Pressione Enter para continuar (ESC para sair)...", i+1);
            fflush(stdout);

            int ch;
            while (1) {
                ch = getchar();
                if (ch == '\n' || ch == EOF) break;
                if (ch == 27) {
                    aborted = 1;
                    break;
                }
            }
            if (aborted) break;
        }
    } else {
        for (; counter < linenumb; counter++) {

#ifdef _WIN32
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 27) {
                    aborted = 1;
                    break;
                }
            }
#endif
            i = executionFunction(P, i, M, &s, &auxScan, &auxPrint);
        }
    }

    if (aborted) {
        printf("\nExecucao interrompida pelo usuario (ESC).\n");
    }
    // =============================================================================

    fclose(file);

    return 0;
}