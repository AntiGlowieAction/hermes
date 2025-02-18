#include <stdint.h>
#include<stdlib.h>
#include <string.h>

#include "../include/hparse.h"



uint32_t djb2(unsigned char *str) {
    uint32_t hash = 5381;
    int c;

    while (c = *str++) {
        hash = hash * 33 + c;
    }

    return hash;
}

hentry* hget(htable *tab, const char *key){
    uint32_t digest = djb2(key);
    hentry *entry = tab->entries[digest % tab->size];
    
    while(entry) {
        if(!strcmp(entry->key, key)) {
            return entry;
        }
        entry = entry->next;
    }

    return entry;
}

unsigned int hgetint(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_INTEGER) {
        herror("Syntax error!!!\n");
    }
    return *((unsigned int*) entry->value);
}

char* hgetstr(htable *tab, const char *key);
htable* hgettab(htable *tab, const char *key);

int hgetbool(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_BOOL) {
        herror("Syntax error!!!\n");
    }

    return !!entry->value;
}

hentry* hcrtent(const char *key, enum henrtype type, void *val) {
    hentry *ent = halloc(hentry);
    ent->type = type;
    ent->value = val;
    ent->key = key;
    return ent;
}

hversion* hgetver(htable *tab, const char *key);

void hset(htable *tab, const char *key, enum henrtype type, void *val) {
    uint32_t digest = djb2(key) % tab->size;
    hentry *entry = tab->entries[digest];

    if(!entry) {
        tab->entries[digest] = hcrtent(key, type, val);
        return;
    }

    while(entry->next){
        if(!strcmp(key, entry->key) {
            free(entry->value);
            entry->type = type;
            entry->value = val;
        }
        entry = entry->next;
    }
}


void hsetint(htable *tab, const char *key, unsigned int val);
void hsetstr(htable *tab, const char *key, const char *val);
htable* hsettab(htable *tab, const char *key);
void hsetbool(htable *tab, const char *key, int val);
void hsetver(htable *tab, const char *key, const char *val);

htable* hcreatetab(size_t size) {
    htable *tab = halloc(htable);
    tab->size = size;
    tab->entries = calloc(size, sizeof(hentry*));

    if(!tab->entries) {
        herror("Could not allocate memory!!!\n");
    }
    
    return tab;
}


int main(int argc, char **argv) {
    char *path = hopen("test.hm");
    htoken *tokens = hlex(path);
    return 0;
}
