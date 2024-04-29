#include "../include/basic_scheme.h"

int basic_search(void *key, size_t key_length, const char *cipher,
                 const char *word, size_t cipher_length) {
  if (key_length != cipher_length) {
    return 0;
  }
  size_t random_bytes_length = cipher_length - KEY_LENGTH;
  char *encryption_value = safe_malloc(cipher_length);
  char *compare_value = safe_malloc(cipher_length);
  char *hash = safe_malloc(KEY_LENGTH);
  int res = 1;

  for (size_t i = 0; i < cipher_length; i++) {
    encryption_value[i] = cipher[i] ^ word[i];
  }
  safe_memcpy(compare_value, cipher_length, encryption_value,
              random_bytes_length);
  pseudorandom_function(key, key_length, compare_value, random_bytes_length,
                        hash, KEY_LENGTH);
  safe_memcpy(&compare_value[random_bytes_length],
              cipher_length - random_bytes_length, hash, KEY_LENGTH);

  for (size_t i = 0; i < cipher_length; i++) {
    if (encryption_value[i] != compare_value[i]) {
      res = 0;
    }
  }

  free(encryption_value);
  free(compare_value);
  free(hash);
  return res;
}

void basic_encryption(const char *keys[], const char *plaintext[],
                      size_t length, char *ciphertext[],
                      char *encryption_keys[]) {
  for (int i = 0; i < length; i++) {
    size_t string_length = strlen(plaintext[i]) + 1;

    encryption_keys[i] = safe_malloc(string_length);
    ciphertext[i] = safe_malloc(string_length);
    ciphertext[i][string_length - 1] = '\0';

    get_encryption_value(encryption_keys[i], string_length, keys[i],
                         string_length);
    encrypt_word(encryption_keys[i], plaintext[i], ciphertext[i],
                 string_length);
    printf("Encrypted Word: ");
    print_byte_array((unsigned char *)ciphertext[i], string_length);
  }
}

void basic_decryption(char *encryption_keys[], char *ciphertext[],
                      size_t ciphertext_lengths[], size_t length,
                      char *plaintext[]) {
  for (int i = 0; i < length; i++) {
    size_t string_length = ciphertext_lengths[i];

    plaintext[i] = safe_malloc(string_length);
    plaintext[i][string_length - 1] = '\0';

    print_byte_array((unsigned char *)encryption_keys[i], string_length);
    print_byte_array((unsigned char *)ciphertext[i], string_length);
    encrypt_word(encryption_keys[i], ciphertext[i], plaintext[i],
                 string_length);
    printf("Decrypted Word: %s\n", plaintext[i]);
  }
}

void basic_scheme() {
  // init variables
  const char *plaintext[WORD_COUNT] = {"Hello\0", "World\0"};
  const char *keys[WORD_COUNT] = {"World\0", "Hello\0"};
  char *ciphertext[WORD_COUNT];
  char *encryption_keys[WORD_COUNT];
  char *decrypted_plaintext[WORD_COUNT];
  size_t cipher_lengths[WORD_COUNT];

  // get lengths for each word and encrypt it
  get_string_lengths_from_arr(plaintext, WORD_COUNT, cipher_lengths);
  basic_encryption(keys, plaintext, WORD_COUNT, ciphertext, encryption_keys);

  // search for plaintext[1] in ciphertext[1] with keys[1]
  int res = basic_search((void *)keys[1], cipher_lengths[1], ciphertext[1],
                         plaintext[1], cipher_lengths[1]);
  if (res) {
    printf("Did find %s\n", plaintext[1]); // should be found if no issues
  }

  // decrypt word
  basic_decryption(encryption_keys, ciphertext, cipher_lengths, WORD_COUNT,
                   decrypted_plaintext);

  // free keys, ciphertext and decrypted plaintext variables
  for (int i = 0; i < WORD_COUNT; i++) {
    free(encryption_keys[i]);
    free(ciphertext[i]);
    free(decrypted_plaintext[i]);
  }
}