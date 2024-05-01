#include "../include/controlled_scheme.h"

// Perform controlled encryption for an array of plaintext words
void controlled_encryption(char *search_keys[], const char *plaintext[],
                           size_t length, char *ciphertext[],
                           char *encryption_keys[]) {
  for (int i = 0; i < length; i++) {
    // Calculate string length
    size_t string_length = strlen(plaintext[i]) + 1;

    // Allocate memory for search keys
    search_keys[i] = safe_malloc(string_length);

    // Generate encryption key for the plaintext word
    generate_encryption_key(plaintext[i], string_length, search_keys[i],
                            string_length);

    // Allocate memory for encryption keys
    encryption_keys[i] = malloc(string_length);

    // Get encryption value for the word
    get_encryption_value(encryption_keys[i], string_length, search_keys[i],
                         string_length);

    // Allocate memory for ciphertext
    ciphertext[i] = safe_malloc(string_length);
    ciphertext[i][string_length - 1] = '\0'; // Null-terminate

    // Encrypt the word
    encrypt_word(encryption_keys[i], plaintext[i], ciphertext[i],
                 string_length);

    // Print encrypted word
    printf("Encrypted Word: ");
    print_byte_array((unsigned char *)ciphertext[i], string_length);
  }
}

// Perform controlled decryption for an array of ciphertext words
void controlled_decryption(char *encryption_keys[], char *ciphertext[],
                           size_t ciphertext_lengths[], size_t length,
                           char *plaintext[]) {
  for (int i = 0; i < length; i++) {
    // Get string length
    size_t string_length = ciphertext_lengths[i];

    // Allocate memory for plaintext
    plaintext[i] = safe_malloc(string_length);
    plaintext[i][string_length - 1] = '\0'; // Null-terminate

    // Decrypt the word
    encrypt_word(encryption_keys[i], ciphertext[i], plaintext[i],
                 string_length);

    // Print decrypted word
    printf("Decrypted Word: %s\n", plaintext[i]);
  }
}

// Implement controlled scheme
void controlled_scheme() {
  // Define plaintext array
  const char *plaintext[WORD_COUNT] = {"Hello\0", "World\0"};

  // Declare arrays for search keys, ciphertext, encryption keys, and decrypted
  // plaintext
  char *search_keys[WORD_COUNT];
  char *ciphertext[WORD_COUNT];
  char *encryption_keys[WORD_COUNT];
  char *decrypted_plaintext[WORD_COUNT];

  // Declare array for ciphertext lengths
  size_t cipher_lengths[WORD_COUNT];

  // Get lengths for each word and encrypt it
  get_string_lengths_from_arr(plaintext, WORD_COUNT, cipher_lengths);
  controlled_encryption(search_keys, plaintext, WORD_COUNT, ciphertext,
                        encryption_keys);

  // Search for a word in ciphertext with keys
  int res = basic_search((void *)search_keys[1], cipher_lengths[1],
                         ciphertext[1], plaintext[1], cipher_lengths[1]);
  if (res) {
    printf("Did find %s\n", plaintext[1]); // Should be found if no issues
  }

  // Decrypt words
  controlled_decryption(encryption_keys, ciphertext, cipher_lengths, WORD_COUNT,
                        decrypted_plaintext);

  // Free allocated memory
  for (int i = 0; i < WORD_COUNT; i++) {
    free(search_keys[i]);
    free(encryption_keys[i]);
    free(ciphertext[i]);
    free(decrypted_plaintext[i]);
  }
}
