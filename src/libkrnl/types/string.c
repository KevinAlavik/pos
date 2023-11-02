#include "stdtypes.h"
#include "string.h"

void str_reverse(char *s) {
    size_t i = 0;
    size_t j = strlen(s) - 1;
    char tmp;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

void str_cat(char *s1, char *s2) {
    char *start = s1;
    while(*start != '\0') start++;
    while(*s2 != '\0') *start++ = *s2++;
    *start = '\0';
}

void str_append(char *s, char c) {
    size_t i = 0;
    while (s[i] != '\0') i++;
    s[i] = c;
    s[i+1] = '\0';
}

char *strcpy(char *dest, char *src) {
    size_t i;

    for (i = 0; src[i]; i++) {
        dest[i] = src[i];
    }

    dest[i] = 0;

    return dest;
}

char *strncpy(char *dest, char *src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i]; i++) {
        dest[i] = src[i];
    }
    for ( ; i < n; i++) {
        dest[i] = 0;
    }

    return dest;
}

int strcmp(char *s1, char *s2) {
    for (size_t i = 0; ; i++) {
        char c1 = s1[i], c2 = s2[i];
        if (c1 != c2) {
            return c1 - c2;
        }
        if (!c1) {
            return 0;
        }
    }
}

int strncmp(char *s1, char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        char c1 = s1[i], c2 = s2[i];
        if (c1 != c2) {
            return c1 - c2;
        }
        if (!c1) {
            return 0;
        }
    }

    return 0;
}

size_t strlen(char *str) {
    size_t len;

    for (len = 0; str[len]; len++);

    return len;
}

#ifdef HEAP_ACCESSABLE
#include "utils/heapdef.h"
char **split(char *str, char spliton) {
    int num_substrings = 1;
    for (int i = 0; str[i]; i++) {
        if (str[i] == spliton) {
            num_substrings++;
        }
    }
    char **substrings =
        (char **)malloc(((num_substrings + 1) * sizeof(char *)));
    int substr_index = 0;
    int start_index = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == spliton) {
            int substring_length = i - start_index;
            substrings[substr_index] = (char *)malloc((substring_length + 1));
            for (int j = 0; j < substring_length; j++) {
                substrings[substr_index][j] = str[start_index + j];
            }
            substrings[substr_index][substring_length] = '\0';
            start_index = i + 1;
            substr_index++;
        }
    }
    int substring_length = strlen(str) - start_index;
    substrings[substr_index] = (char *)malloc((substring_length + 1));
    for (int j = 0; j < substring_length; j++) {
        substrings[substr_index][j] = str[start_index + j];
    }
    substrings[substr_index][substring_length] = '\0';
    substr_index++;
    substrings[substr_index] = NULL;
    return substrings;
}
#endif
