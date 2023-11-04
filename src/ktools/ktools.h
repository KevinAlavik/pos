void hlt(void) {
    while(1) {
        __asm__ volatile("hlt");
    }
}

void kstrcat(char* destination, const char* source) {
    // Find the end of the destination string
    while (*destination != '\0') {
        destination++;
    }

    // Append characters from source to destination
    while (*source != '\0') {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0'; // Terminate the destination string
}

int kstrcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' || *str2 != '\0') {
        if (*str1 != *str2) {
            return (*str1 - *str2);
        }
        str1++;
        str2++;
    }
    return 0;
}

void kmemcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void kmemset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

int kmemcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}
