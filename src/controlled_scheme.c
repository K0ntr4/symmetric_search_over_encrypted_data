#include "../include/controlled_scheme.h"

void controlled_encryption(char *search_keys[], const char *plaintext[],
                           size_t length, char *ciphertext[],
                           char *encryption_keys[]) {
  for (int i = 0; i < length; i++) {
    size_t string_length = strlen(plaintext[i]) + 1;
    search_keys[i] = safe_malloc(string_length);
    generate_encryption_key(plaintext[i], string_length, search_keys[i],
                            string_length);

    encryption_keys[i] = malloc(string_length);
    get_encryption_value(encryption_keys[i], string_length, search_keys[i],
                         string_length);
    ciphertext[i] = safe_malloc(string_length);
    ciphertext[i][string_length - 1] = '\0';

    encrypt_word(encryption_keys[i], plaintext[i], ciphertext[i],
                 string_length);
    printf("Encrypted Word: ");
    print_byte_array((unsigned char *)ciphertext[i], string_length);
  }
}

void controlled_decryption(char *encryption_keys[], char *ciphertext[],
                           size_t ciphertext_lengths[], size_t length,
                           char *plaintext[]) {
  for (int i = 0; i < length; i++) {
    size_t string_length = ciphertext_lengths[i];

    plaintext[i] = safe_malloc(string_length);
    plaintext[i][string_length - 1] = '\0';

    encrypt_word(encryption_keys[i], ciphertext[i], plaintext[i],
                 string_length);
    printf("Decrypted Word: %s\n", plaintext[i]);
  }
}

void controlled_scheme() {
  const char *plaintext[WORD_COUNT] = {"Hello\0", "World\0"};
  char *search_keys[WORD_COUNT];
  char *ciphertext[WORD_COUNT];
  char *encryption_keys[WORD_COUNT];
  char *decrypted_plaintext[WORD_COUNT];
  size_t cipher_lengths[WORD_COUNT];

  // get lengths for each word and encrypt it
  get_string_lengths_from_arr(plaintext, WORD_COUNT, cipher_lengths);
  controlled_encryption(search_keys, plaintext, WORD_COUNT, ciphertext,
                        encryption_keys);

  // search for plaintext[1] in ciphertext[1] with keys[1]
  int res = basic_search((void *)search_keys[1], cipher_lengths[1],
                         ciphertext[1], plaintext[1], cipher_lengths[1]);
  if (res) {
    printf("Did find %s\n", plaintext[1]); // should be found if no issues
  }

  // decrypt word
  controlled_decryption(encryption_keys, ciphertext, cipher_lengths, WORD_COUNT,
                        decrypted_plaintext);

  // free keys, ciphertext and decrypted plaintext variables
  for (int i = 0; i < WORD_COUNT; i++) {
    free(search_keys[i]);
    free(encryption_keys[i]);
    free(ciphertext[i]);
    free(decrypted_plaintext[i]);
  }
}