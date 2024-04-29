#include "../include/utils.h"

void safe_memcpy(void *dest, size_t dest_size, const void *src,
                 size_t src_size) {
  if (src_size > dest_size) {
    fprintf(stderr, "Error: Source size exceeds destination size\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dest, src, src_size); // NOLINT
}

void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void safe_strcpy(char *dest, size_t dest_size, const char *src) {
  size_t src_length = 0;
  while (src[src_length] != '\0') {
    src_length++;
  }

  if (src_length > dest_size) {
    fprintf(stderr,
            "Error: Source string is too large for the destination buffer\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < src_length; i++) {
    dest[i] = src[i];
  }
  dest[src_length] = '\0';
}

void print_byte_array(const unsigned char *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%02X ", arr[i]);
  }
  printf("\n");
}

size_t concatenated_string_array_size(const char *arr[],
                                      const size_t arr_length) {
  size_t res = 0;
  for (int i = 0; i < arr_length; i++) {
    res = res + strlen(arr[i]);
  }
  return res + 1;
}

void concat_string_array(const char *arr[], const size_t arr_length,
                         char *output, size_t output_length) {
  int last_written = 0;

  for (int i = 0; i < arr_length; i++) {
    safe_strcpy(&output[last_written], output_length, arr[i]);
    last_written = last_written + strlen(arr[i]);
  }
}

void concat_string_array_with_word(const char *arr[], const size_t arr_length,
                                   const char *word, char *output,
                                   size_t output_length) {
  int last_written = 0;

  for (int i = 0; i < arr_length; i++) {
    safe_strcpy(&output[last_written], output_length, arr[i]);
    last_written = last_written + strlen(arr[i]);
  }
  safe_strcpy(&output[last_written], output_length, word);
}

void get_string_lengths_from_arr(const char *arr[], size_t arr_length,
                                 size_t lengths[]) {
  for (int i = 0; i < arr_length; i++) {
    lengths[i] = strlen(arr[i]) + 1;
  }
}

char digit_to_char(int digit) { return '0' + digit; }

int int_to_string_length(int num) {
  int temp = num;
  int length = num == 0 ? 1 : 0;

  while (temp != 0) {
    temp /= 10;
    length++;
  }
  return length;
}

char *uint_to_string(int num) {
  int length = int_to_string_length(num);
  int remainder;
  char *result = safe_malloc(length + 1);
  result[length] = '\0';

  int i = length - 1;
  do {
    remainder = num % 10;
    result[i--] = digit_to_char(remainder);
    num /= 10;
  } while (num != 0);

  return result;
}

char *chapter_index_to_string_with_leading_zero(int chapter_index) {
  char *index_string = uint_to_string(chapter_index);
  size_t length = strlen(index_string) + 2;
  char *result = safe_malloc(length);

  result[0] = '0';
  result[length - 1] = '\0';
  safe_strcpy(&result[1], length - 1, index_string);
  free(index_string);
  return result;
}