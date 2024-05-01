#ifndef SSOED_H
#define SSOED_H

#include "base_encryption.h"
#include "basic_scheme.h"
#include "controlled_chapter_scheme.h"
#include "controlled_hierarchical_scheme.h"
#include "controlled_scheme.h"
#include "utils.h"

void pseudorandom_generator(void *buffer, const size_t size);
void pseudorandom_function(const void *key, size_t key_size,
                           const char *plaintext, size_t plaintext_size,
                           char *hash, size_t hash_size);
void encrypt_word(const char *key, const char *plain, char *cipher,
                  size_t length);
void get_encryption_value(char *encryption_value, size_t text_length,
                          const char *key, size_t key_length);
void generate_encryption_key(const char *plaintext,
                             const size_t plaintext_length, char *key,
                             const size_t key_length);
int basic_search(void *key, size_t key_length, const char *cipher,
                 const char *word, size_t length);

#endif // SSOED_H
