#ifndef BASIC_SCHEME_H
#define BASIC_SCHEME_H

#include "base_encryption.h"
#include "utils.h"

int basic_search(void *key, size_t key_length, const char *cipher,
                 const char *word, size_t cipher_length);
void basic_encryption(char *keys[], const char *plaintext[],
                      size_t length, char *ciphertext[],
                      char *encryption_keys[]);
void basic_decryption(char *encryption_keys[], char *ciphertext[],
                      size_t ciphertext_lengths[], size_t length,
                      char *plaintext[]);
void basic_scheme();

#endif // BASIC_SCHEME_H
