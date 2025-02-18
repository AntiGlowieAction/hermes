#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../include/hermes.h"

char* hopen(const char *path){
    FILE *file = fopen(path, "r");
    
    if(!file){
        herror("No file found!!!");
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
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

    if (!strcmp(token->sval, "true")) {
        token->type = TK_TRUE;
    } else if (!strcmp(token->sval, "false")) {
        token->type = TK_FALSE;
    }

    *src = crs;
}

void hlexnum(char **src, htoken *token) {
    token->type = TK_INT;

    char *pos = *src;
    char *crs = pos;

    while(isdigit(*crs) || *crs == '.') {
        if(*crs == '.') {
            token->type = TK_VERSION;
        }
        crs++;
    }

    if (token->type == TK_INT) {
        token->ival = atoi(pos);
    } else {
        token->sval = strndup(pos, crs-pos);
    }

    *src = crs;
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
    
    while (*src) {
        if (isspace(*src)) {
            while(isspace(*src)) src++;
            continue;
        }

        htoken *token = halloc(htoken);

        if (!head) {
            head = token;
            tail = token;
        } else {
            tail->next = token;
            tail = token;
        }

        if (isalpha(*src)){
            hlexidn(&src, token);
            continue;
        }

        if (isdigit(*src)){
            hlexnum(&src, token);
            continue;
        }

        if (*src == '"'){
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
        src++;
    }

    return head;
}

void hprntoken(htoken *token) {
    if (!token) return;

    switch (token->type) {
        case TK_IDEN:
            printf("IDN %s\n", token->sval);
            break;
        case TK_LBRACK:
            printf("SYM [\n");
            break;
        case TK_RBRACK:
            printf("SYM ]\n");
            break;
        case TK_COLON:
            printf("SYM :\n");
            break;
        case TK_EQUALS:
            printf("SYM =\n");
            break;
        case TK_VERSION:
            printf("VER %s\n", token->sval);
            break;
        case TK_STRING:
            printf("STR %s\n", token->sval);
            break;
        case TK_INT:
            printf("INT %d\n", token->ival);
            break;
        case TK_TRUE:
            printf("TRUE\n");
            break;
        case TK_FALSE:
            printf("FALSE\n");
            break;
        default:
            printf("UNKNOWN TOKEN TYPE %d\n", token->type);
            break;
    }

    if (token->next) {
        hprntoken(token->next);
    }
}


int main(int argc, char **argv) {
    char *path = hopen("test.hm");
    htoken *tokens = hlex(path);
    hprntoken(tokens);
    return 0;
}
