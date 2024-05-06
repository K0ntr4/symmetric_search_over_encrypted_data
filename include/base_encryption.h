#ifndef BASE_ENCRYPTION_H
#define BASE_ENCRYPTION_H

#define KEY_LENGTH 3
#define WORD_COUNT 2
#define CHAPTER_COUNT 2

#include "utils.h"

void pseudorandom_generator(void *buffer, const size_t size);
void pseudorandom_function(void *key, size_t key_size, const char *plaintext,
                           size_t plaintext_size, char *hash, size_t hash_size);
void encrypt_word(char *key, const char *plain, char *cipher, size_t length);
void get_encryption_value(char *encryption_value, size_t text_length, char *key,
                          size_t key_length);
void generate_encryption_key(const char *plaintext,
                             const size_t plaintext_length, char *key,
                             const size_t key_length);

#endif // BASE_ENCRYPTION_H
