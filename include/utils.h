#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>

void safe_memcpy(void *dest, size_t dest_size, const void *src,
                 size_t src_size);
void *safe_malloc(size_t size);
void *safe_secure_malloc(size_t size);
void *safe_secure_malloc_array(size_t count, size_t size);
void secure_free(void *ptr);
void safe_strcpy(char *dest, size_t dest_size, const char *src);
void print_byte_array(const unsigned char *arr, int size);
char *chapter_index_to_string_with_leading_zero(int chapter_index);
char *concat_chapter_index_with_word(int chapter_index, char *word);
void get_string_lengths_from_arr(const char *arr[], size_t arr_length,
                                 size_t lengths[]);
void get_string_lengths_from_chapter_arr(char ***dynamic_array,
                                         size_t *cipher_lengths[]);
char ***create_dynamic_string_array(const char *plaintext[], size_t num_words,
                                    const size_t chapter_indexes[],
                                    size_t num_chapters);
char ***create_empty_dynamic_string_array(const char *plaintext[],
                                          size_t num_words,
                                          const size_t chapter_indexes[],
                                          size_t num_chapters);
void free_dynamic_string_array(char ***dynamic_array);
void free_dynamic_secure_string_array(char ***dynamic_array);

#endif // UTILS_H
