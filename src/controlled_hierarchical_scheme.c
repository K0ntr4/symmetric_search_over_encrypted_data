#include "../include/controlled_hierarchical_scheme.h"

// Generate hierarchical encryption key
void generate_hierarchical_encryption_key(void *generation_key,
                                          size_t generation_key_length,
                                          const char *plaintext,
                                          const size_t plaintext_length,
                                          char *key, const size_t key_length) {
  // TODO: Replace with another pseudorandom function
  pseudorandom_function(generation_key, generation_key_length, plaintext,
                        plaintext_length, key, key_length);
}

// Perform a controlled hierarchical search
int controlled_hierarchical_search(void *universal_key, size_t key_length,
                                   const char *cipher, const char *word,
                                   size_t cipher_length,
                                   const char *concatenated_chapter) {
  // Check if key length matches cipher length
  if (key_length != cipher_length) {
    return 0; // Key and cipher length mismatch, search fails
  }

  // Calculate the length of random bytes
  size_t random_bytes_length = cipher_length - KEY_LENGTH;

  // Allocate memory for encryption key, encryption value, compare value, and
  // hash
  char *encryption_key = safe_malloc(key_length);
  char *encryption_value = safe_malloc(cipher_length);
  char *compare_value = safe_malloc(cipher_length);
  char *hash = safe_malloc(KEY_LENGTH);
  int res = 1; // Initialize result to success

  // Generate encryption key based on the provided universal key and chapter
  generate_hierarchical_encryption_key(
      universal_key, key_length, concatenated_chapter,
      strlen(concatenated_chapter), encryption_key, key_length);

  // Perform XOR operation between cipher and word to get encryption value
  for (size_t i = 0; i < cipher_length; i++) {
    encryption_value[i] = cipher[i] ^ word[i];
  }

  // Copy a portion of encryption value for comparison
  safe_memcpy(compare_value, cipher_length, encryption_value,
              random_bytes_length);

  // Generate hash based on encryption key and compare value to check if
  // encryption value is of type {s, Fki(s)}
  pseudorandom_function(encryption_key, key_length, compare_value,
                        random_bytes_length, hash, KEY_LENGTH);

  // Copy hash to the rest of compare value for full comparison
  safe_memcpy(&compare_value[random_bytes_length],
              cipher_length - random_bytes_length, hash, KEY_LENGTH);

  // Compare encryption value with compare value
  for (size_t i = 0; i < cipher_length; i++) {
    if (encryption_value[i] != compare_value[i]) {
      res = 0; // If mismatch found, set result to failure
      break;   // No need to continue comparison
    }
  }

  // Free allocated memory
  free(encryption_key);
  free(encryption_value);
  free(compare_value);
  free(hash);

  return res; // Return the result of the search
}

// Perform controlled hierarchical encryption
void controlled_hierarchical_encryption(char ***search_keys,
                                        char ***universal_search_keys,
                                        char ***plaintext, char ***ciphertext,
                                        char ***encryption_keys,
                                        size_t **universal_key_lengths) {
  // Iterate over chapters and words
  for (size_t i = 0; plaintext[i] != NULL; i++) {
    for (size_t j = 0; plaintext[i][j] != NULL; j++) {
      // Calculate string length
      size_t string_length = strlen(plaintext[i][j]) + 1;

      // Allocate memory for search keys
      search_keys[i][j] = safe_malloc(string_length);

      // Generate concatenated chapter string
      char *concatenated_chapter =
          chapter_index_to_string_with_leading_zero(i + 1);

      // Allocate memory for universal search keys
      universal_search_keys[i][j] = safe_malloc(string_length);

      // Allocate memory for concatenated word
      char *concatenated_word = safe_malloc(string_length + 1);
      concatenated_word[0] = '1'; // Set the leading character
      safe_strcpy(&concatenated_word[1], string_length + 1, plaintext[i][j]);

      // Generate encryption key for the word
      generate_encryption_key(concatenated_word, string_length + 1,
                              universal_search_keys[i][j], string_length);

      // Generate hierarchical encryption key for the word
      generate_hierarchical_encryption_key(
          universal_search_keys[i][j], string_length, concatenated_chapter,
          strlen(concatenated_chapter), search_keys[i][j], string_length);

      // Free allocated memory
      free(concatenated_chapter);
      free(concatenated_word);

      // Store the universal key length
      universal_key_lengths[i][j] = string_length;

      // Allocate memory for encryption keys
      encryption_keys[i][j] = safe_malloc(string_length);

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

// Perform controlled hierarchical decryption
void controlled_hierarchical_decryption(char ***encryption_keys,
                                        char ***ciphertext,
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

// Implement controlled hierarchical scheme
void controlled_hierarchical_scheme() {
  // Define text and chapter indexes
  const char *text[] = {"Hello",     "World",    "what",   "wonderful",
                        "test",      "sentence", "tested", "Better",
                        "Encrypted", "than",     "zero",   "encryption",
                        "systems",   "obviously"};
  size_t chapter_indexes[] = {7};

  // Calculate number of words and chapters
  size_t num_words = sizeof(text) / sizeof(text[0]);
  size_t num_chapters = sizeof(chapter_indexes) / sizeof(chapter_indexes[0]);

  // Create dynamic arrays for plaintext, search keys, universal search keys,
  // ciphertext, encryption keys, decrypted plaintext, and cipher lengths
  char ***plaintext = create_dynamic_string_array(
      text, num_words, chapter_indexes, num_chapters);
  char ***search_keys = create_empty_dynamic_string_array(
      text, sizeof(text) / sizeof(text[0]), chapter_indexes,
      sizeof(chapter_indexes) / sizeof(chapter_indexes[0]));
  char ***universal_search_keys = create_empty_dynamic_string_array(
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
  size_t **universal_key_lengths =
      safe_malloc(sizeof(size_t *) * CHAPTER_COUNT);

  size_t total_words = sizeof(text) / sizeof(text[0]);
  // Calculate cipher lengths and universal key lengths
  for (int i = 0; i <= num_chapters; i++) {
    size_t words = i < num_chapters ? chapter_indexes[i] : total_words;
    cipher_lengths[i] = safe_malloc(sizeof(size_t) * words);
    universal_key_lengths[i] = safe_malloc(sizeof(size_t) * words);
    total_words = total_words - words;
  }

  // Populate cipher lengths from plaintext
  get_string_lengths_from_chapter_arr(plaintext, cipher_lengths);

  // Perform controlled hierarchical encryption
  controlled_hierarchical_encryption(search_keys, universal_search_keys,
                                     plaintext, ciphertext, encryption_keys,
                                     universal_key_lengths);

  // Search for a word using chapter key
  int res = 0;
  if (plaintext && plaintext[1] && plaintext[1][1]) {
    res = basic_search((void *)search_keys[1][1], universal_key_lengths[1][1],
                       ciphertext[1][1], plaintext[1][1], cipher_lengths[1][1]);
  }
  if (res) {
    printf("Did find %s using chapter key.\n",
           plaintext[1][1]); // Should be found if no issues
  }
  res = 0;

  // Search for a word using universal key
  if (plaintext && plaintext[1] && plaintext[1][1]) {
    char *concatenated_chapter =
        chapter_index_to_string_with_leading_zero(1 + 1);
    res = controlled_hierarchical_search(
        (void *)universal_search_keys[1][1], universal_key_lengths[1][1],
        ciphertext[1][1], plaintext[1][1], cipher_lengths[1][1],
        concatenated_chapter);
    free(concatenated_chapter);
  }

  if (res) {
    printf("Did find %s using universal key.\n",
           plaintext[1][1]); // Should be found if no issues
  }

  // Perform controlled hierarchical decryption
  controlled_hierarchical_decryption(encryption_keys, ciphertext,
                                     cipher_lengths, decrypted_plaintext);

  // Free allocated memory
  free_dynamic_string_array(plaintext);
  free_dynamic_string_array(search_keys);
  free_dynamic_string_array(universal_search_keys);
  free_dynamic_string_array(encryption_keys);
  free_dynamic_string_array(ciphertext);
  free_dynamic_string_array(decrypted_plaintext);

  for (int i = 0; i < CHAPTER_COUNT; i++) {
    free(cipher_lengths[i]);
    free(universal_key_lengths[i]);
  }
  free(cipher_lengths);
  free(universal_key_lengths);
}
