#include "../include/basic_scheme.h"

// Perform basic search to find a word in ciphertext
int basic_search(void *key, size_t key_length, const char *cipher,
                 const char *word, size_t cipher_length) {
  // Check if key length matches cipher length
  if (key_length != cipher_length) {
    return 0;
  }

  // Calculate length of random bytes
  size_t random_bytes_length = cipher_length - KEY_LENGTH;

  // Allocate memory for encryption value, compare value, and hash
  char *encryption_value = safe_malloc(cipher_length);
  char *compare_value = safe_malloc(cipher_length);
  char *hash = safe_malloc(KEY_LENGTH);

  // Initialize result
  int res = 1;

  // Generate encryption value for the word
  for (size_t i = 0; i < cipher_length; i++) {
    encryption_value[i] = cipher[i] ^ word[i];
  }

  // Copy part of encryption value for comparison
  safe_memcpy(compare_value, cipher_length, encryption_value,
              random_bytes_length);

  // Use pseudorandom function with key to generate hash to check if
  // encryption value is of type {s, Fki(s)}
  pseudorandom_function(key, key_length, compare_value, random_bytes_length,
                        hash, KEY_LENGTH);

  // Copy hash to compare value
  safe_memcpy(&compare_value[random_bytes_length],
              cipher_length - random_bytes_length, hash, KEY_LENGTH);

  // Compare encryption value and compare value
  for (size_t i = 0; i < cipher_length; i++) {
    if (encryption_value[i] != compare_value[i]) {
      res = 0;
    }
  }

  // Free allocated memory
  free(encryption_value);
  free(compare_value);
  free(hash);

  return res;
}

// Perform basic encryption for an array of plaintext words
void basic_encryption(const char *keys[], const char *plaintext[],
                      size_t length, char *ciphertext[],
                      char *encryption_keys[]) {
  for (int i = 0; i < length; i++) {
    // Calculate string length
    size_t string_length = strlen(plaintext[i]) + 1;

    // Allocate memory for encryption keys and ciphertext
    encryption_keys[i] = safe_malloc(string_length);
    ciphertext[i] = safe_malloc(string_length);
    ciphertext[i][string_length - 1] = '\0'; // Null-terminate

    // Get encryption value for the key
    get_encryption_value(encryption_keys[i], string_length, keys[i],
                         string_length);

    // Encrypt the word
    encrypt_word(encryption_keys[i], plaintext[i], ciphertext[i],
                 string_length);

    // Print encrypted word
    printf("Encrypted Word: ");
    print_byte_array((unsigned char *)ciphertext[i], string_length);
  }
}

// Perform basic decryption for an array of ciphertext words
void basic_decryption(char *encryption_keys[], char *ciphertext[],
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

// Implement basic scheme
void basic_scheme() {
  // Initialize variables
  const char *plaintext[WORD_COUNT] = {"Hello\0", "World\0"};
  const char *keys[WORD_COUNT] = {"World\0", "Hello\0"};
  char *ciphertext[WORD_COUNT];
  char *encryption_keys[WORD_COUNT];
  char *decrypted_plaintext[WORD_COUNT];
  size_t cipher_lengths[WORD_COUNT];

  // Get lengths for each word and encrypt it
  get_string_lengths_from_arr(plaintext, WORD_COUNT, cipher_lengths);
  basic_encryption(keys, plaintext, WORD_COUNT, ciphertext, encryption_keys);

  // Search for a word in ciphertext with keys
  int res = basic_search((void *)keys[1], cipher_lengths[1], ciphertext[1],
                         plaintext[1], cipher_lengths[1]);
  if (res) {
    printf("Did find %s\n", plaintext[1]); // Should be found if no issues
  }

  // Decrypt words
  basic_decryption(encryption_keys, ciphertext, cipher_lengths, WORD_COUNT,
                   decrypted_plaintext);

  // Free allocated memory
  for (int i = 0; i < WORD_COUNT; i++) {
    free(encryption_keys[i]);
    free(ciphertext[i]);
    free(decrypted_plaintext[i]);
  }
}
