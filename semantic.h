#ifndef SEMANTIC_H
#define SEMANTIC_H

enum identifierTypes {tnomedeprograma, tvariavel, tinteiro, tbooleano,
                      tprocedimento, tfuncaointeiro, tfuncaobooleana};

typedef struct identifier {
    char *lexema;
    int type;
    char scope;
    void *address;
} identifier;

typedef struct Stack {
    identifier id[1000];
    int top;
} Stack;

Stack symbolsTable;

void insertTable(char *lexema, int type, char scope, void *address){
    symbolsTable.top++;
    symbolsTable.id[symbolsTable.top].lexema = lexema;
    symbolsTable.id[symbolsTable.top].type = type;
    symbolsTable.id[symbolsTable.top].scope = scope;
    symbolsTable.id[symbolsTable.top].address = address;
}

/// Conversão para posfixa
Stack posfixConvertion(Stack expr){
    
    return expr;
}

/// Search until the first mark
int searchDuplVarTable(char *lexema)
{

}

/// Search until the start of the stack
int searchDeclVarTable(char *lexema); 

int searchDeclProcTable(char* lexema);

int searchDeclVarFuncTable(char *lexema);

int searchTable(char *lexema);

void placeTypeTable(int type){
    for(int i = symbolsTable.top; i > -1; i--)
    {
        if(symbolsTable.id[i].type == tvariavel){
            symbolsTable.id[i].type = type;
        }
    }
}

void unstackLevel(){
    for(int i = symbolsTable.top; symbolsTable.id[symbolsTable.top].scope != 0; i--){

    }
}

#endif
