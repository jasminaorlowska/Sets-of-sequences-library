#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"

//valid - not empty, containing only chars 0 1 or 2
bool valid_string(const char *s) {
    if (s == NULL || *s == '\0') return false; 

    int i = 0; 
    while (*(s+i) != '\0') {
        char curr = *(s+i);
        if (curr != '0' && curr != '1' && curr != '2') return false;
        i++;
    }

    return true;
}

bool compare_strings(char * s1, const char *s2) {
    if (s2 == NULL || s1 == NULL) return false;

    int i = 0;

    while (*s1 != '\0' && *(s2+i) != '\0') {
        if (*s1 != *(s2+i)) return false;
        s1++; i++;
    }

    if (*s1 != *(s2+i)) return false;
    return true;
}

//different types than above
bool compare_strings2(char * s1, char *s2) {
    if (s2 == NULL || s1 == NULL) return false;

    int i = 0;
    while (*s1 != '\0' && *(s2+i) != '\0') {
        if (*s1 != *(s2+i)) return false;
        s1++; i++;
    }

    if (*s1 != *(s2+i)) return false;
    return true;
}

char * create_char(const char *c1) {
    char * c2 = malloc(strlen(c1) + 1);
    if (c2 == NULL) return NULL;
    strcpy(c2, c1);
    return c2;
}

char * concat_strings(char * s1, char * s2) {
    char * s3 = malloc(strlen(s1) + strlen(s2) + 1);
    if (s3 == NULL) return NULL;

    char *s3_ptr = s3;
    while (*s1 != '\0') {
        *s3_ptr = *s1; 
        s1++;
        s3_ptr++;
    }
    while (*s2 != '\0') {
        *s3_ptr = *s2; 
        s2++;
        s3_ptr++;
    }

    *s3_ptr = '\0';
    return s3;
}
