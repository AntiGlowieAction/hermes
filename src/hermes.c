#include <stdio.h>
#include <stdlib.h>
#include "../include/hermes.h"
#include <string.h>
#include <ctype.h>
#define halloc(type) (type*)malloc(sizeof(type))
#define herror(info) do{ \
                        printf(info); \
                        exit(1); \
                        }while(0)


    

char* hopen(const char *path){
    FILE *file = fopen(path, "r");
    
    if(!file){
        herror("No file found!!!");
    }

    long len;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *source = (char*) malloc((len + 1)*sizeof(char));

    if(!source){
        herror("Could not get memory!!!");
    }
    
    if(fread(source, 1, len, file) != len) {
        herror("Could not read source!!!");
    }

    source[len] = 0;

    fclose(file);
    return source;
}

void hlexidn(char **src, htoken *token) {
    token->type = TK_IDEN;

    char *pos = *src;
    char *crs = *src;

    while(isalnum(*crs) || *crs == '_' || *crs == '-'){
        crs++;
    }

    token->sval = strndup(pos, crs - pos);
    printf("This is a debug statement %s\n", token->sval);

    if(!strcmp(token->sval, "true")) {
        token->type = TK_TRUE;
        token->ival = 1;
    }

    if(!strcmp(token->sval, "false")) {
        token->type = TK_FALSE;
        token->ival = 0;
    }

    *src = crs;
}

void hlexnum(char **src, htoken *token) {
    token->type = TK_INT;

    char *pos = *src;
    int len = 0;
    while(isdigit(**src) || **src == '.') {
        if(**src == '.') token->type = TK_VERSION;
        len++;
        (*src)++;
    }

    token->sval = strndup(pos, len);
    
    if(token->type == TK_INT) {
        token->ival = atoi(token->sval);
    }

}

void hlexstr(char **src, htoken *token) {
    token->type = TK_STRING;

    char *pos = *src + 1;
    char *crs = pos;

    while(*crs != '"') {
        if (*crs == '\n') {
            herror("Unexpected newline.");
        }
        crs++;
    }
    
    token->sval = strndup(pos, crs-pos);
    *src = crs + 1;
}


htoken* hlex(char *src) {
    htoken *head = NULL;
    htoken *tail = NULL;
    
    while(*src){

        if(isspace(*src)) {
            while(isspace(*src)) src++;
            continue;
        }

        htoken *token = halloc(htoken);

        if(!head) {
            head = token;
            tail = token;
        }

        if(isalpha(*src)){
            hlexidn(&src, token);
            continue;
        }

        if(isdigit(*src)){
            hlexnum(&src, token);
            continue;
        }

        if(*src == '"'){
            hlexstr(&src, token);
            continue;
        }
        
        switch (*src) {
            
            case '[':
                token->type = TK_LBRACK;
                break;
            case ']':
                token->type = TK_RBRACK;
                break;
            case ':':
                token->type = TK_COLON;
                break;
            case '=':
                token->type = TK_EQUALS;
                break;
            default:
                herror("Unexpected character!!!");
        }

        if(tail) {
            tail->next = token;
            tail = token;
        }

        src++;
    }
    return head;
}




int main(int argc, char **argv) {


    char *path = hopen("test.hm");
    htoken *lexstream = hlex(path);

    while(lexstream){
        printf("ival of token is %d\n", lexstream->ival);
        if(lexstream->sval) printf("sval of token is %s\n", lexstream->sval);
        printf("\n\n");
        
        
        lexstream = lexstream->next;
    }

    


    return 0;
}
