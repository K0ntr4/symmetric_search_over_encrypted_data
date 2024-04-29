#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void safe_memcpy(void *dest, size_t dest_size, const void *src,
                        size_t src_size);
extern void *safe_malloc(size_t size);
extern void safe_strcpy(char *dest, size_t dest_size, const char *src);
extern void print_byte_array(const unsigned char *arr, int size);
extern size_t concatenated_string_array_size(const char *arr[],
                                             const size_t arr_length);
extern void concat_string_array(const char *arr[], const size_t arr_length,
                                char *output, size_t output_length);
extern void concat_string_array_with_word(const char *arr[],
                                          const size_t arr_length,
                                          const char *word, char *output,
                                          size_t output_length);
char *chapter_index_to_string_with_leading_zero(int chapter_index);
extern void get_string_lengths_from_arr(const char *arr[], size_t arr_length,
                                        size_t lengths[]);
char *uint_to_string(int num);

#endif // UTILS_H
