#include <sodium.h>
#include <string.h>

#define BLOCKLENGTH 24UL

unsigned long long slen(const void *string) {
    unsigned long long i = 0;
    while (((const char *)string)[i]) {
        i++;
    }
    return i;
}

void phex(const void *string, unsigned long long length) {
    unsigned long long i;
    for (i = 0; i < length; i++) {
        printf("0x%02x ", ((const unsigned char *)string)[i]);
    }
    printf("\n");
}

void sxor(const void *string1, const void *string2, void *r, unsigned long long length) {
    unsigned long long i;
    for (i = 0; i < length; i++) {
        ((unsigned char *)r)[i] = ((const unsigned char *)string1)[i] ^ ((const unsigned char *)string2)[i];
    }
}

int encryptword(const unsigned char *s, const unsigned long long l, const unsigned char *key1, const unsigned char *n1, unsigned char *c) {
    // Encryption using sxor
    sxor(s, key1, c, BLOCKLENGTH);

    // Split encrypted word into two parts
    unsigned char first_part[l];
    unsigned char second_part[l];
    memcpy(first_part, c, l);

    // sxor the second part with (nonce xor first part)
    sxor(c, n1, second_part, l);

    // sxor the first part with the nonce
    sxor(first_part, n1, c, l);

    // Append the two parts to c
    memcpy(&c[l], second_part, l);

    return 0;
}

int decryptword(const unsigned char *c, const unsigned char *key1, const unsigned char *n1, unsigned char *s, unsigned long l) {
    // recover the first part of the word from c using sxor with the nonce
    unsigned char first_part[l];
    sxor(c, n1, first_part, l);
    // recover the second part of the word from c using sxor with (nonce xor first part)
    unsigned char key[l];
    sxor(s, n1, key, l);
    unsigned char whole_word[2 * l];
    memcpy(whole_word, first_part, l);
    sxor(&c[l], key, &whole_word[l], l);
    // decrypt the word using sxor
    sxor(whole_word, key1, s, l);
    return 0;
}

int encryptarray(const unsigned char a[][BLOCKLENGTH], const unsigned long l, const unsigned char *k1, const unsigned char *k2, const unsigned char *n1, const unsigned char *n2, unsigned char *c) {
    // Encryption of each word separately using sxor
    encryptword(a[0], BLOCKLENGTH, k1, n1, &c[0 * BLOCKLENGTH]);
    encryptword(a[1], BLOCKLENGTH, k2, n2, &c[1 * BLOCKLENGTH]);

    // Ensure null termination
    c[l * BLOCKLENGTH] = '\0';

    return 0;
}

int decryptarray(const unsigned char *c, const unsigned long nob, const unsigned char *k1, const unsigned char *k2, const unsigned char *n1, const unsigned char *n2, char a[][BLOCKLENGTH]) {
    // Decryption of each word separately using sxor
    decryptword(&c[0 * BLOCKLENGTH], k1, n1, (unsigned char *)a[0], BLOCKLENGTH);
    decryptword(&c[1 * BLOCKLENGTH], k2, n2, (unsigned char *)a[1], BLOCKLENGTH);
    return 0;
}


int hasword(const unsigned char *ew, const unsigned char *c, const unsigned long nob) {
    char *s = malloc(nob);
    sxor(ew, c, s, nob);
    return 0;
}

int main(void) {
    if (sodium_init() < 0) {
        exit(2);
    }

    unsigned char c[2 * BLOCKLENGTH];
    unsigned char nonce[crypto_stream_chacha20_NONCEBYTES];
    unsigned char key[crypto_stream_chacha20_KEYBYTES];
    unsigned char nonce2[crypto_stream_chacha20_NONCEBYTES];
    unsigned char key2[crypto_stream_chacha20_KEYBYTES];
    unsigned char c2[BLOCKLENGTH];

    const char a[2][BLOCKLENGTH] = {"hello", "world"};
    char b[2][BLOCKLENGTH];

    randombytes_buf(nonce, crypto_stream_chacha20_NONCEBYTES);
    randombytes_buf(key, crypto_stream_chacha20_KEYBYTES);
    randombytes_buf(nonce2, crypto_stream_chacha20_NONCEBYTES);
    randombytes_buf(key2, crypto_stream_chacha20_KEYBYTES);

    encryptarray((const unsigned char (*)[BLOCKLENGTH])a, 2UL, key, key2, nonce, nonce2, c);
    phex(c, 2 * BLOCKLENGTH);

    encryptword((const unsigned char *)"world", 5UL, key, nonce, c2);
    printf("%d\n", hasword(c2, c, 2UL));

    encryptword((const unsigned char *)"orange", 5UL, key, nonce, c2);
    printf("%d\n", hasword(c2, c, 2UL));

    decryptarray(c, 2UL, key, key2, nonce, nonce2, b);

    printf("%s\n", b[0]);
    printf("%s\n", b[1]);

    return 0;
}
