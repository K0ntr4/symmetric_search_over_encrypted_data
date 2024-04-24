#ifndef SSOED_H
#define SSOED_H

#include <sodium.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pseudorandom_generator(void *buffer, const size_t size);
void pseudorandom_function(const void *key, size_t key_size, const char *plaintext, size_t plaintext_size, char *hash, size_t hash_size);
int encrypt_word(const char *key, const char *plain, char *cipher, size_t length);
void *safe_malloc(size_t size);
char *get_encryption_value(const char *text, size_t text_length, const char *key, size_t key_length);
int basic_search(void *key, const char *cipher, const char *word, size_t length);
void test_basic_scheme();

#endif //SSOED_H
