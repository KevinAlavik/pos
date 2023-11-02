#include <types/stdtypes.h>
#include <stdbool.h>

void str_reverse(char *s);
void str_cat(char *s1, char *s2);
void str_append(char *s, char c);

char *strcpy(char *dest, char *src);
char *strncpy(char *dest, char *src, size_t n);
int strcmp(char *s1, char *s2);
int strncmp(char *s1, char *s2, size_t n);
size_t strlen(char *str);

#ifdef HEAP_ACCESSABLE
char** split(char* str,char spliton);
#endif