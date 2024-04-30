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

char *concat_chapter_index_with_word(int chapter_index, char *word) {
  char *index_string = uint_to_string(chapter_index);
  size_t index_length = strlen(index_string);
  size_t length = index_length + strlen(word) + 1;
  char *result = safe_malloc(length);

  result[length - 1] = '\0';
  safe_strcpy(result, length - 1, index_string);
  safe_strcpy(&result[index_length - 1], length - index_length - 1, word);
  free(index_string);
  return result;
}

char ***create_dynamic_string_array(const char *plaintext[], size_t num_words,
                                    const size_t chapter_indexes[],
                                    size_t num_chapters) {
  char ***dynamic_array = safe_malloc((num_chapters + 2) * sizeof(char **));

  size_t start_index = 0;
  for (size_t i = 0; i < num_chapters; i++) {
    size_t end_index = chapter_indexes[i];
    size_t chapter_length = end_index - start_index;

    dynamic_array[i] = safe_malloc((chapter_length + 1) * sizeof(char *));

    for (size_t j = 0; j < chapter_length; j++) {
      dynamic_array[i][j] = safe_malloc(strlen(plaintext[start_index + j]) + 1);
      safe_strcpy(dynamic_array[i][j], strlen(plaintext[start_index + j]) + 1,
                  plaintext[start_index + j]);
    }
    dynamic_array[i][chapter_length] = NULL;
    start_index = end_index;
  }

  size_t last_chapter_length = num_words - start_index;
  dynamic_array[num_chapters] =
      safe_malloc((last_chapter_length + 1) * sizeof(char *));

  for (size_t j = 0; j < last_chapter_length; j++) {
    dynamic_array[num_chapters][j] =
        safe_malloc(strlen(plaintext[start_index + j]) + 1);
    safe_strcpy(dynamic_array[num_chapters][j],
                strlen(plaintext[start_index + j]) + 1,
                plaintext[start_index + j]);
  }

  dynamic_array[num_chapters][last_chapter_length] = NULL;
  dynamic_array[num_chapters + 1] = NULL;
  return dynamic_array;
}

void free_dynamic_string_array(char ***dynamic_array) {
  for (size_t i = 0; dynamic_array[i] != NULL; i++) {
    for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
      free(dynamic_array[i][j]);
    }
    free(dynamic_array[i]);
  }
  free(dynamic_array);
}

void get_string_lengths_from_chapter_arr(char ***dynamic_array,
                                         size_t *cipher_lengths[]) {
  for (size_t i = 0; dynamic_array[i] != NULL; i++) {
    for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
      cipher_lengths[i][j] = strlen(dynamic_array[i][j]) + 1;
    }
  }
}