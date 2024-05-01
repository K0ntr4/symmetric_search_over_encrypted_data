#ifndef CONTROLLED_HIERARCHICAL_SCHEME_H
#define CONTROLLED_HIERARCHICAL_SCHEME_H

#include "base_encryption.h"
#include "basic_scheme.h"
#include "utils.h"

void generate_hierarchical_encryption_key(void *generation_key,
                                          size_t generation_key_length,
                                          const char *plaintext,
                                          const size_t plaintext_length,
                                          char *key, const size_t key_length);
int controlled_hierarchical_search(void *universal_key, size_t key_length,
                                   const char *cipher, const char *word,
                                   size_t cipher_length,
                                   const char *concatenated_chapter);
void controlled_hierarchical_encryption(char ***search_keys,
                                        char ***universal_search_keys,
                                        char ***plaintext, char ***ciphertext,
                                        char ***encryption_keys,
                                        size_t **universal_key_lengths);
void controlled_hierarchical_decryption(char ***encryption_keys,
                                        char ***ciphertext,
                                        size_t *ciphertext_lengths[],
                                        char ***plaintext);
void controlled_hierarchical_scheme();

#endif // CONTROLLED_HIERARCHICAL_SCHEME_H
