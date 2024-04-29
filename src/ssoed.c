#include "../include/ssoed.h"

void generate_encryption_key_hierarchical(void *generation_key,
                                          size_t generation_key_length,
                                          const char *plaintext,
                                          const size_t plaintext_length,
                                          char *key, const size_t key_length) {
  pseudorandom_function(generation_key, generation_key_length, plaintext,
                        plaintext_length, key,
                        key_length); // TODO use another pseudorandom function
}

int basic_search_hierarchical(void *universal_key, size_t key_length,
                              const char *cipher, const char *word,
                              size_t cipher_length,
                              const char *concatenated_chapter) {
  if (key_length != cipher_length) {
    return 0;
  }
  size_t random_bytes_length = cipher_length - KEY_LENGTH;
  char *encryption_key = safe_malloc(key_length);
  char *encryption_value = safe_malloc(cipher_length);
  char *compare_value = safe_malloc(cipher_length);
  char *hash = safe_malloc(KEY_LENGTH);
  int res = 1;

  generate_encryption_key_hierarchical(
      universal_key, key_length, concatenated_chapter,
      strlen(concatenated_chapter), encryption_key, key_length);

  for (size_t i = 0; i < cipher_length; i++) {
    encryption_value[i] = cipher[i] ^ word[i];
  }
  safe_memcpy(compare_value, cipher_length, encryption_value,
              random_bytes_length);
  pseudorandom_function(encryption_key, key_length, compare_value,
                        random_bytes_length, hash, KEY_LENGTH);
  safe_memcpy(&compare_value[random_bytes_length],
              cipher_length - random_bytes_length, hash, KEY_LENGTH);

  for (size_t i = 0; i < cipher_length; i++) {
    if (encryption_value[i] != compare_value[i]) {
      res = 0;
    }
  }

  free(encryption_key);
  free(encryption_value);
  free(compare_value);
  free(hash);
  return res;
}

void test_controlled_scheme_with_chapters_hierarchical() {
  const char *chapters[CHAPTER_COUNT][CHAPTER_WORD_COUNT] = {
      {"obviously\0", "Hello\0", "World\0", "what\0", "wonderful\0", "test\0",
       "sentence\0"},
      {"Better\0", "Encrypted\0", "than\0", "zero\0", "encryption\0",
       "systems\0", "obviously\0"}};
  char *keys[CHAPTER_COUNT][CHAPTER_WORD_COUNT];
  char *generation_keys[CHAPTER_COUNT][CHAPTER_WORD_COUNT];
  size_t key_lengths[CHAPTER_COUNT][CHAPTER_WORD_COUNT];

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    for (int j = 0; j < CHAPTER_WORD_COUNT; j++) {
      size_t string_length = strlen(chapters[i][j]) + 1;
      keys[i][j] = safe_malloc(string_length);
      char *concatenated_chapter =
          chapter_index_to_string_with_leading_zero(i + 1);
      generation_keys[i][j] = safe_malloc(string_length);
      char *concatenated_word = safe_malloc(string_length + 1);
      concatenated_word[0] = '1';
      safe_strcpy(&concatenated_word[1], string_length + 1, chapters[i][j]);
      generate_encryption_key(concatenated_word, string_length + 1,
                              generation_keys[i][j], string_length);
      generate_encryption_key_hierarchical(
          generation_keys[i][j], string_length, concatenated_chapter,
          strlen(concatenated_chapter), keys[i][j], string_length);
      key_lengths[i][j] = string_length;
      char *encryption_value = malloc(string_length);
      get_encryption_value(encryption_value, string_length, keys[i][j],
                           string_length);
      char *cipher = safe_malloc(string_length);
      char *plain = safe_malloc(string_length);

      cipher[string_length - 1] = '\0';
      plain[string_length - 1] = '\0';

      encrypt_word(encryption_value, chapters[i][j], cipher, string_length);
      printf("Encrypted Word: ");
      print_byte_array((unsigned char *)cipher, string_length);

      int res = basic_search((void *)keys[0][0], key_lengths[0][0], cipher,
                             chapters[i][j], string_length);

      if (res == 0) {
        printf("Did not find %s using chapter key.\n", chapters[i][j]);
      } else {
        printf("Did find %s using chapter key.\n", chapters[i][j]);
      }

      res = basic_search_hierarchical((void *)generation_keys[0][0],
                                      key_lengths[0][0], cipher, chapters[i][j],
                                      string_length, concatenated_chapter);

      if (res == 0) {
        printf("Did not find %s using universal key.\n", chapters[i][j]);
      } else {
        printf("Did find %s using universal key.\n", chapters[i][j]);
      }

      encrypt_word(encryption_value, cipher, plain, string_length);
      printf("Decrypted Word: %s\n", plain);

      free(concatenated_word);
      free(concatenated_chapter);
      free(encryption_value);
      free(cipher);
      free(plain);
    }
  }

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    for (int j = 0; j < CHAPTER_WORD_COUNT; j++) {
      free(keys[i][j]);
      free(generation_keys[i][j]);
    }
  }
}

void test_controlled_scheme_with_chapters() {
  const char *chapters[CHAPTER_COUNT][CHAPTER_WORD_COUNT] = {
      {"Hello\0", "World\0", "what\0", "wonderful\0", "test\0", "sentence\0",
       "tested\0"},
      {"Better\0", "Encrypted\0", "than\0", "zero\0", "encryption\0",
       "systems\0", "obviously\0"}};
  char *keys[CHAPTER_COUNT][CHAPTER_WORD_COUNT];
  size_t key_lengths[CHAPTER_COUNT][CHAPTER_WORD_COUNT];

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    for (int j = 0; j < CHAPTER_WORD_COUNT; j++) {
      size_t string_length = strlen(chapters[i][j]) + 1;
      keys[i][j] = safe_malloc(string_length);
      size_t concatenated_length =
          concatenated_string_array_size(chapters[i], CHAPTER_WORD_COUNT) +
          string_length - 1;
      char *concatenated_chapter = malloc(concatenated_length);
      concat_string_array_with_word(chapters[i], CHAPTER_WORD_COUNT,
                                    chapters[i][j], concatenated_chapter,
                                    concatenated_length);
      generate_encryption_key(concatenated_chapter, concatenated_length,
                              keys[i][j], string_length);
      key_lengths[i][j] = string_length;
      char *encryption_value = malloc(string_length);
      get_encryption_value(encryption_value, string_length, keys[i][j],
                           string_length);
      char *cipher = safe_malloc(string_length);
      char *plain = safe_malloc(string_length);

      cipher[string_length - 1] = '\0';
      plain[string_length - 1] = '\0';

      encrypt_word(encryption_value, chapters[i][j], cipher, string_length);
      printf("Encrypted Word: ");
      print_byte_array((unsigned char *)cipher, string_length);

      int res = basic_search((void *)keys[0][0], key_lengths[0][0], cipher,
                             chapters[i][j], string_length);

      if (res == 0) {
        printf("Did not find %s\n", chapters[i][j]);
      } else {
        printf("Did find %s\n", chapters[i][j]);
      }

      encrypt_word(encryption_value, cipher, plain, string_length);
      printf("Decrypted Word: %s\n", plain);

      free(concatenated_chapter);
      free(encryption_value);
      free(cipher);
      free(plain);
    }
  }

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    for (int j = 0; j < CHAPTER_WORD_COUNT; j++) {
      free(keys[i][j]);
    }
  }
}

int main(void) {
  if (sodium_init() < 0) {
    perror("Sodium initialization failed");
    exit(EXIT_FAILURE);
  }

  // basic_scheme();
  // printf("\n");
  controlled_scheme();
  // test_controlled_scheme_with_chapters_hierarchical();
  return 0;
}
