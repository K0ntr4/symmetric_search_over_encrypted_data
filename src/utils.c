#include "../include/utils.h"

// Safely copy memory from source to destination, ensuring the destination size
// is not exceeded
void safe_memcpy(void *dest, size_t dest_size, const void *src,
                 size_t src_size) {
  if (src_size > dest_size) {
    fprintf(stderr, "Error: Source size exceeds destination size\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dest, src, src_size); // NOLINT
}

// Safely allocate memory, exiting with an error message if allocation fails
void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safe_secure_malloc(size_t size) {
    void *ptr = sodium_malloc(size);
    if (!ptr) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *safe_secure_malloc_array(size_t count, size_t size) {
    void *ptr = sodium_allocarray(count, size);
    if (!ptr) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void secure_free(void *ptr) {
    sodium_free(ptr);
}


// Safely copy a string from source to destination, ensuring the destination
// buffer is large enough
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

// Print a byte array in hexadecimal format
void print_byte_array(const unsigned char *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%02X ", arr[i]);
  }
  printf("\n");
}

// Get the lengths of strings in an array of strings
void get_string_lengths_from_arr(const char *arr[], size_t arr_length,
                                 size_t lengths[]) {
  for (int i = 0; i < arr_length; i++) {
    lengths[i] = strlen(arr[i]) + 1;
  }
}

// Convert a digit to its character representation
char digit_to_char(int digit) { return '0' + digit; }

// Calculate the length of an integer when converted to a string
int int_to_string_length(int num) {
  int temp = num;
  int length = num == 0 ? 1 : 0;

  while (temp != 0) {
    temp /= 10;
    length++;
  }
  return length;
}

// Convert an unsigned integer to a string
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

// Convert a chapter index to a string with a leading zero
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

// Concatenate a chapter index with a word
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

// Create a dynamically allocated array of strings from input text
char ***create_dynamic_string_array(const char *plaintext[], size_t num_words,
                                    const size_t chapter_indexes[],
                                    size_t num_chapters) {
  // Allocate memory for the array of chapter arrays plus one additional for
  // NULL terminator
  char ***dynamic_array = safe_malloc((num_chapters + 2) * sizeof(char **));

  size_t start_index = 0;
  for (size_t i = 0; i < num_chapters; i++) {
    size_t end_index = chapter_indexes[i];
    size_t chapter_length = end_index - start_index;

    // Allocate memory for the chapter array plus one additional for NULL
    // terminator
    dynamic_array[i] = safe_malloc((chapter_length + 1) * sizeof(char *));

    for (size_t j = 0; j < chapter_length; j++) {
      // Allocate memory for each word in the chapter
      dynamic_array[i][j] = safe_malloc(strlen(plaintext[start_index + j]) + 1);
      // Copy the word into the allocated memory
      safe_strcpy(dynamic_array[i][j], strlen(plaintext[start_index + j]) + 1,
                  plaintext[start_index + j]);
    }
    // Set the last element of the chapter array to NULL to mark its end
    dynamic_array[i][chapter_length] = NULL;
    start_index = end_index;
  }

  // Handle the last chapter (if any)
  size_t last_chapter_length = num_words - start_index;
  dynamic_array[num_chapters] =
      safe_malloc((last_chapter_length + 1) * sizeof(char *));

  for (size_t j = 0; j < last_chapter_length; j++) {
    // Allocate memory for each word in the last chapter
    dynamic_array[num_chapters][j] =
        safe_malloc(strlen(plaintext[start_index + j]) + 1);
    // Copy the word into the allocated memory
    safe_strcpy(dynamic_array[num_chapters][j],
                strlen(plaintext[start_index + j]) + 1,
                plaintext[start_index + j]);
  }

  // Set the last element of the last chapter array to NULL to mark its end
  dynamic_array[num_chapters][last_chapter_length] = NULL;
  // Set the last element of the dynamic array to NULL to mark its end
  dynamic_array[num_chapters + 1] = NULL;
  return dynamic_array;
}

// Create an empty dynamically allocated array of strings
char ***create_empty_dynamic_string_array(const char *plaintext[],
                                          size_t num_words,
                                          const size_t chapter_indexes[],
                                          size_t num_chapters) {
  // Allocate memory for the array of chapter arrays plus one additional for
  // NULL terminator
  char ***dynamic_array = safe_malloc((num_chapters + 2) * sizeof(char **));

  size_t start_index = 0;
  for (size_t i = 0; i < num_chapters; i++) {
    size_t end_index = chapter_indexes[i];
    size_t chapter_length = end_index - start_index;

    // Allocate memory for the chapter array plus one additional for NULL
    // terminator
    dynamic_array[i] = safe_malloc((chapter_length + 1) * sizeof(char *));
    // Set the last element of the chapter array to NULL to mark its end
    dynamic_array[i][chapter_length] = NULL;
    start_index = end_index;
  }

  // Handle the last chapter (if any)
  size_t last_chapter_length = num_words - start_index;
  dynamic_array[num_chapters] =
      safe_malloc((last_chapter_length + 1) * sizeof(char *));
  // Set the last element of the last chapter array to NULL to mark its end
  dynamic_array[num_chapters][last_chapter_length] = NULL;
  // Set the last element of the dynamic array to NULL to mark its end
  dynamic_array[num_chapters + 1] = NULL;
  return dynamic_array;
}

// Free memory allocated for a dynamically allocated array of strings
void free_dynamic_string_array(char ***dynamic_array) {
  for (size_t i = 0; dynamic_array[i] != NULL; i++) {
    for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
      free(dynamic_array[i][j]);
    }
    free(dynamic_array[i]);
  }
  free(dynamic_array);
}

void free_dynamic_secure_string_array(char ***dynamic_array) {
    for (size_t i = 0; dynamic_array[i] != NULL; i++) {
        for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
            secure_free(dynamic_array[i][j]);
        }
        free(dynamic_array[i]);
    }
    free(dynamic_array);
}

// Get the lengths of strings in a dynamically allocated array of strings
void get_string_lengths_from_chapter_arr(char ***dynamic_array,
                                         size_t *cipher_lengths[]) {
  for (size_t i = 0; dynamic_array[i] != NULL; i++) {
    for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
      cipher_lengths[i][j] = strlen(dynamic_array[i][j]) + 1;
    }
  }
}
