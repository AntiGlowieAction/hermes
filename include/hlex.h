#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define herror(info) \
    do { \
        printf(info); \
        exit(1); \
    } while(0)

#define halloc(type) \
    ({ type *p = (type*) calloc(sizeof(type), 1); \
     if (!p) herror("Memory allocation failed!!!\n"); \
     p; }) 

enum htoktype {
    TK_IDEN,
    TK_LBRACK,
    TK_RBRACK,
    TK_COLON,
    TK_EQUALS,
    TK_VERSION,
    TK_STRING,
    TK_INT,
    TK_TRUE,
    TK_FALSE
};

typedef struct htoken {
    enum htoktype type;
    int ival;
    char *sval;
    struct htoken *next;

}htoken;


typedef struct {
    char *str;
    int *list;
} hversion;


htoken* hlex(char *src);
char* hopen(const char *path);

