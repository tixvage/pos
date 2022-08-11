#ifndef STD_H_
#define STD_H_

#include <stdint.h>

void kmemcpy(char* source, char* dest, int nbytes);
void kmemset(uint8_t *dest, uint8_t val, uint32_t len);
void kint_to_ascii(int n, char str[]);
void kreverse(char s[]);
int kstrlen(char s[]);
void kstrappend(char s[], char n);

#endif
