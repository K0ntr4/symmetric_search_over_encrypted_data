#ifndef CONTROLLED_SCHEME_H
#define CONTROLLED_SCHEME_H

#include "base_encryption.h"
#include "basic_scheme.h"
#include "utils.h"

void controlled_encryption(char *search_keys[], const char *plaintext[],
                           size_t length, char *ciphertext[],
                           char *encryption_keys[]);
void controlled_decryption(char *encryption_keys[], char *ciphertext[],
                           size_t ciphertext_lengths[], size_t length,
                           char *plaintext[]);
void controlled_scheme();

#endif // CONTROLLED_SCHEME_H
