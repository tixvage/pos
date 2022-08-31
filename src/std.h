#ifndef STD_H_
#define STD_H_

#include <stdint.h>

void kmemcpy(char* source, char* dest, int nbytes);
void kmemcpyfast(const void* src, void* dest, int nbytes);
void kmemset(void *dest, uint32_t val, uint32_t len);
void kint_to_ascii(int n, char str[]);
void kreverse(char s[]);
int kstrlen(char s[]);
void kstrappend(char s[], char n);

#endif
