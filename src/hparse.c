#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/hparse.h"

//Hasnihg function
uint32_t djb2(const char *str) {
    unsigned const char *ptr = (unsigned const char *)str;

    uint32_t hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = hash * 33 + c;
    }

    return hash;
}

//Getter functions
hentry* hget(htable *tab, const char *key) {
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

char* hgetstr(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_STRING) {
        herror("Syntax error!!!\n");
    }

    return (char*) entry->value;
}

htable* hgettab(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_TABLE) {
        herror("Syntax error!!!\n");
    }

    return (htable*) entry->value;
}

int hgetbool(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_BOOL) {
        herror("Syntax error!!!\n");
    }

    return !!entry->value;
}


hversion* hgetver(htable *tab, const char *key) {
    hentry *entry = hget(tab, key);

    if(entry->type != EN_VERSION) {
        herror("Systax error!!!\n");
    }

    return (hversion*) entry->value;
}

//Functions for creating htables and hentries
hentry* hcrtent(const char *key, enum henrtype type, void *val) {
    hentry *ent = halloc(hentry);
    ent->type = type;
    ent->value = val;
    ent->key = key;
    return ent;
}

htable* hcreatetab(size_t size) {
    htable *tab = halloc(htable);
    tab->size = size;
    tab->entries = calloc(size, sizeof(hentry*));

    if(!tab->entries) {
        herror("Could not allocate memory!!!\n");
    }
    
    return tab;
}

//Setter functions
void hset(htable *tab, const char *key, enum henrtype type, void *val) {
    uint32_t digest = djb2(key) % tab->size;
    hentry *entry = tab->entries[digest];
    hentry *temp = entry;

    if(!entry) {
        tab->entries[digest] = hcrtent(key, type, val);
        return;
    }

    while(entry->next){
        if(!strcmp(key, entry->key)) {
            free(entry->value);
            entry->type = type;
            entry->value = val;
            return;
        }
        entry = entry->next;
    }
    //This functions seems to not cover the situation where there is a list at the digest index
    //but none of the enetries in the list have the same key as is the one in the function call
    //The bellow is my attempt to fix this issue the code can safely be deleted if I am tripping
    //Also this function probably does not work with htables
    entry = hcrtent(key, type, val);
    tab->entries[digest] = entry;
    entry->next = temp;
}


void hsetint(htable *tab, const char *key, unsigned int val) {
    unsigned int *temp = halloc(unsigned int);
    *temp = val;
    hset(tab, key, EN_INTEGER, (void*) temp);
}

void hsetstr(htable *tab, const char *key, const char *val) {
    //I am copying the string here assuming that a stack allocated string will be passed to the function
    const char *temp = val;
    int len = 0;
    while(val++) len++;
    temp = strndup(val, len);
    hset(tab, key, EN_STRING, (void*) temp);
}

htable* hsettab(htable *tab, const char *key) {
    hset(tab, key, EN_TABLE, NULL);
}

void hsetbool(htable *tab, const char *key, int val) {
    int *temp = halloc(int);
    *temp = val;
    hset(tab, key, EN_BOOL, (void*) temp);
}

void hsetver(htable *tab, const char *key, const char *val) {
    const char *temp = val;
    int len = 0;
    while(val++) len++;
    temp = strndup(val, len);
    hset(tab, key, EN_VERSION, (void*) temp);
}




int main() {
    char *path = hopen("test.hm");
    htoken *tokens = hlex(path);
    hprntoken(tokens);
    return 0;
}
