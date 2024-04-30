#ifndef CONTROLLED__CHAPTER_SCHEME_H
#define CONTROLLED__CHAPTER_SCHEME_H

#include "base_encryption.h"
#include "basic_scheme.h"
#include "utils.h"

void testDynamicArr();
void controlled_chapter_encryption(char ***search_keys, char ***plaintext,
                                   char ***ciphertext,
                                   char ***encryption_keys);
void controlled_chapter_decryption(char ***encryption_keys, char ***ciphertext,
                                   size_t *ciphertext_lengths[],
                                   char ***plaintext);
void controlled_scheme_with_chapters();

#endif