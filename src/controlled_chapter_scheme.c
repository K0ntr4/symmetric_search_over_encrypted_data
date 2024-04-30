//
// Created by luca on 4/30/24.
//

#include "../include/controlled_chapter_scheme.h"

void testDynamicArr() {
  const char *plaintext[] = {"Hello",     "World",    "what",   "wonderful",
                             "test",      "sentence", "tested", "Better",
                             "Encrypted", "than",     "zero",   "encryption",
                             "systems",   "obviously"};

  size_t chapter_indexes[] = {7};

  char ***dynamic_array = create_dynamic_string_array(
      plaintext, sizeof(plaintext) / sizeof(plaintext[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));

  for (size_t i = 0; dynamic_array[i] != NULL; i++) {
    printf("Chapter %zu:\n", i + 1);
    for (size_t j = 0; dynamic_array[i][j] != NULL; j++) {
      printf("%s ", dynamic_array[i][j]);
    }
    printf("\n");
  }

  free_dynamic_string_array(dynamic_array);
}

void controlled_chapter_encryption(char ***search_keys, char ***plaintext,
                                   char ***ciphertext,
                                   char ***encryption_keys) {
  for (size_t i = 0; plaintext[i] != NULL; i++) {
    for (size_t j = 0; ciphertext[i][j] != NULL; j++) {
      size_t string_length = strlen(plaintext[i][j]) + 1;
      //      search_keys[i][j] = safe_malloc(string_length);
      char *concatenated_chapter =
          concat_chapter_index_with_word(i, plaintext[i][j]);
      generate_encryption_key(concatenated_chapter,
                              strlen(concatenated_chapter) + 1,
                              search_keys[i][j], string_length);

      free(concatenated_chapter);
      //      encryption_keys[i][j] = malloc(string_length);
      get_encryption_value(encryption_keys[i][j], string_length,
                           search_keys[i][j], string_length);
      //      ciphertext[i][j] = safe_malloc(string_length);
      ciphertext[i][j][string_length - 1] = '\0';

      encrypt_word(encryption_keys[i][j], plaintext[i][j], ciphertext[i][j],
                   string_length);
      printf("Encrypted Word: ");
      print_byte_array((unsigned char *)ciphertext[i][j], string_length);
    }
  }
}

void controlled_chapter_decryption(char ***encryption_keys, char ***ciphertext,
                                   size_t *ciphertext_lengths[],
                                   char ***plaintext) {
  for (size_t i = 0; ciphertext[i] != NULL; i++) {
    for (size_t j = 0; ciphertext[i][j] != NULL; j++) {
      size_t string_length = ciphertext_lengths[i][j];

      //      plaintext[i][j] = safe_malloc(string_length);
      plaintext[i][j][string_length - 1] = '\0';

      encrypt_word(encryption_keys[i][j], ciphertext[i][j], plaintext[i][j],
                   string_length);
      printf("Decrypted Word: %s\n", plaintext[i][j]);
    }
  }
}

void controlled_scheme_with_chapters() {
  const char *plaintext[] = {"Hello",     "World",    "what",   "wonderful",
                             "test",      "sentence", "tested", "Better",
                             "Encrypted", "than",     "zero",   "encryption",
                             "systems",   "obviously"};

  size_t chapter_indexes[] = {7};
  size_t num_words = sizeof(plaintext) / sizeof(plaintext[0]);
  size_t num_chapters = sizeof(chapter_indexes) / sizeof(chapter_indexes[0]);

  char ***dynamic_array = create_dynamic_string_array(
      plaintext, num_words, chapter_indexes, num_chapters);
  char ***search_keys = create_dynamic_string_array(
      plaintext, sizeof(plaintext) / sizeof(plaintext[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***ciphertext = create_dynamic_string_array(
      plaintext, sizeof(plaintext) / sizeof(plaintext[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***encryption_keys = create_dynamic_string_array(
      plaintext, sizeof(plaintext) / sizeof(plaintext[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***decrypted_plaintext = create_dynamic_string_array(
      plaintext, sizeof(plaintext) / sizeof(plaintext[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  size_t **cipher_lengths = malloc(sizeof(size_t *) * CHAPTER_COUNT);

  size_t total_words = sizeof(plaintext) / sizeof(plaintext[0]);
  for (int i = 0; i <= num_chapters; i++) {
    size_t words = i < num_chapters ? chapter_indexes[i] : total_words;
    cipher_lengths[i] = malloc(sizeof(size_t) * words);
    total_words = total_words - words;
  }

  get_string_lengths_from_chapter_arr(dynamic_array, cipher_lengths);
  controlled_chapter_encryption(search_keys, dynamic_array, ciphertext,
                                encryption_keys);

  // search for plaintext[1] from chapter[1] in ciphertext[1] from chapter[1]
  // with key[1][1]
  int res = 0;
  if (dynamic_array && dynamic_array[1] && dynamic_array[1][1]) {
    res = basic_search((void *)search_keys[1][1], cipher_lengths[1][1],
                       ciphertext[1][1], dynamic_array[1][1],
                       cipher_lengths[1][1]);
  }
  if (res) {
    printf("Did find %s\n",
           dynamic_array[1][1]); // should be found if no issues
  }

  // decrypt word
  controlled_chapter_decryption(encryption_keys, ciphertext, cipher_lengths,
                                decrypted_plaintext);

  free_dynamic_string_array(dynamic_array);
  free_dynamic_string_array(search_keys);
  free_dynamic_string_array(encryption_keys);
  free_dynamic_string_array(ciphertext);
  free_dynamic_string_array(decrypted_plaintext);

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    free(cipher_lengths[i]);
  }
  free(cipher_lengths);
}