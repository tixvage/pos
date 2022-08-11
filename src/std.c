#include "std.h"

void kmemcpy(char* source, char* dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) *(dest + i) = *(source + i);
}

void kmemset(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void kint_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    kreverse(str);
}

void kreverse(char s[]) {
    int c, i, j;
    for (i = 0, j = kstrlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int kstrlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void kstrappend(char s[], char n) {
    int len = kstrlen(s);
    s[len] = n;
    s[len+1] = '\0';
}
