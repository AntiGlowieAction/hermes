#include <stdint.h>
#include <stdbool.h>

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

enum henrtype {
    EN_TABLE,
    EN_VERSION,
    EN_BOOL,
    EN_STRING,
    EN_INTEGER
};


typedef struct {
    char *key;
    enum henrtype type;
    void *value;
} hentry;

typedef struct {
    hentry **entries;
    size_t size;
} htable;

typedef struct {
    char *str;
    int *list;
} hversion;

hentry* hget(htable *tab, const char *key);
unsigned int hgetint(htable *tab, const char *key);
char* hgetstr(htable *tab, const char *key);
htable* hgettab(htable *tab, const char *key);
int hgetbool(htable *tab, const char *key);
hversion* hgetver(htable *tab, const char *key);

void hset(htable *tab, const char *key, enum henrtype type, void *val);
void hsetint(htable *tab, const char *key, unsigned int val);
void hsetstr(htable *tab, const char *key, const char *val);
htable* hsettab(htable *tab, const char *key);
void hsetbool(htable *tab, const char *key, int val);
void hsetver(htable *tab, const char *key, const char *val);

htable* hcreatetab(size_t size);
