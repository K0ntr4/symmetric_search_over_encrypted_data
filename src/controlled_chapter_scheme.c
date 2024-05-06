#include "../include/controlled_chapter_scheme.h"

// Perform controlled chapter encryption
void controlled_chapter_encryption(char ***search_keys, char ***plaintext,
                                   char ***ciphertext,
                                   char ***encryption_keys) {
  // Iterate over chapters and words
  for (size_t i = 0; plaintext[i] != NULL; i++) {
    for (size_t j = 0; plaintext[i][j] != NULL; j++) {
      // Calculate string length
      size_t string_length = strlen(plaintext[i][j]) + 1;

      // Allocate memory for search keys
      search_keys[i][j] = safe_secure_malloc(string_length);

      // Generate concatenated chapter and word string
      char *concatenated_chapter =
          concat_chapter_index_with_word(i, plaintext[i][j]);

      // Generate encryption key for the concatenated string
      generate_encryption_key(concatenated_chapter,
                              strlen(concatenated_chapter) + 1,
                              search_keys[i][j], string_length);

      // Free allocated memory
      free(concatenated_chapter);

      // Allocate memory for encryption keys
      encryption_keys[i][j] = safe_secure_malloc(string_length);

      // Get encryption value for the word
      get_encryption_value(encryption_keys[i][j], string_length,
                           search_keys[i][j], string_length);

      // Allocate memory for ciphertext
      ciphertext[i][j] = safe_malloc(string_length);
      ciphertext[i][j][string_length - 1] = '\0'; // Null-terminate

      // Encrypt the word
      encrypt_word(encryption_keys[i][j], plaintext[i][j], ciphertext[i][j],
                   string_length);

      // Print encrypted word
      printf("Encrypted Word: ");
      print_byte_array((unsigned char *)ciphertext[i][j], string_length);
    }
  }
}

// Perform controlled chapter decryption
void controlled_chapter_decryption(char ***encryption_keys, char ***ciphertext,
                                   size_t *ciphertext_lengths[],
                                   char ***plaintext) {
  // Iterate over chapters and words
  for (size_t i = 0; ciphertext[i] != NULL; i++) {
    for (size_t j = 0; ciphertext[i][j] != NULL; j++) {
      // Get string length
      size_t string_length = ciphertext_lengths[i][j];

      // Allocate memory for plaintext
      plaintext[i][j] = safe_malloc(string_length);
      plaintext[i][j][string_length - 1] = '\0'; // Null-terminate

      // Decrypt the word
      encrypt_word(encryption_keys[i][j], ciphertext[i][j], plaintext[i][j],
                   string_length);

      // Print decrypted word
      printf("Decrypted Word: %s\n", plaintext[i][j]);
    }
  }
}

// Implement controlled chapter scheme
void controlled_chapter_scheme() {
  // Define text and chapter indexes
  const char *text[] = {"Hello",     "World",    "what",   "wonderful",
                        "test",      "sentence", "tested", "Better",
                        "Encrypted", "than",     "zero",   "encryption",
                        "systems",   "obviously"};
  size_t chapter_indexes[] = {7};

  // Calculate number of words and chapters
  size_t num_words = sizeof(text) / sizeof(text[0]);
  size_t num_chapters = sizeof(chapter_indexes) / sizeof(chapter_indexes[0]);

  // Create dynamic arrays for plaintext, search keys, ciphertext, encryption
  // keys, and decrypted plaintext
  char ***plaintext = create_dynamic_string_array(
      text, num_words, chapter_indexes, num_chapters);
  char ***search_keys = create_empty_dynamic_string_array(
      text, sizeof(text) / sizeof(text[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***ciphertext = create_empty_dynamic_string_array(
      text, sizeof(text) / sizeof(text[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***encryption_keys = create_empty_dynamic_string_array(
      text, sizeof(text) / sizeof(text[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***decrypted_plaintext = create_empty_dynamic_string_array(
      text, sizeof(text) / sizeof(text[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  size_t **cipher_lengths = safe_malloc(sizeof(size_t *) * CHAPTER_COUNT);

  size_t total_words = sizeof(text) / sizeof(text[0]);
  // Calculate cipher lengths
  for (int i = 0; i <= num_chapters; i++) {
    size_t words = i < num_chapters ? chapter_indexes[i] : total_words;
    cipher_lengths[i] = safe_malloc(sizeof(size_t) * words);
    total_words = total_words - words;
  }

  // Populate cipher lengths from plaintext
  get_string_lengths_from_chapter_arr(plaintext, cipher_lengths);

  // Perform controlled chapter encryption
  controlled_chapter_encryption(search_keys, plaintext, ciphertext,
                                encryption_keys);

  // Search for a word
  int res = 0;
  if (plaintext && plaintext[1] && plaintext[1][1]) {
    res = basic_search((void *)search_keys[1][1], cipher_lengths[1][1],
                       ciphertext[1][1], plaintext[1][1], cipher_lengths[1][1]);
  }
  if (res) {
    printf("Did find %s\n",
           plaintext[1][1]); // Should be found if no issues
  }

  // Perform controlled chapter decryption
  controlled_chapter_decryption(encryption_keys, ciphertext, cipher_lengths,
                                decrypted_plaintext);

  // Free allocated memory
  free_dynamic_string_array(plaintext);
    free_dynamic_secure_string_array(search_keys);
    free_dynamic_secure_string_array(encryption_keys);
  free_dynamic_string_array(ciphertext);
  free_dynamic_string_array(decrypted_plaintext);

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    free(cipher_lengths[i]);
  }
  free(cipher_lengths);
}
