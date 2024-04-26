#include "../include/base_encryption.h"

void pseudorandom_generator(void *buffer, const size_t size) {
  randombytes_buf(buffer, size);
}

void pseudorandom_function(const void *key, size_t key_size,
                           const char *plaintext, size_t plaintext_size,
                           char *hash, size_t hash_size) {
  crypto_generichash((unsigned char *)hash, hash_size,
                     (unsigned char *)plaintext, plaintext_size, key, key_size);
}

void encrypt_word(const char *key, const char *plain, char *cipher,
                  size_t length) {
  for (size_t i = 0; i < length; i++) {
    cipher[i] = plain[i] ^ key[i];
  }
}

void get_encryption_value(char *encryption_value, size_t text_length,
                          const char *key, size_t key_length) {
  size_t random_bytes_length = text_length - KEY_LENGTH;
  void *randombytes = safe_malloc(random_bytes_length);
  char *hash = safe_malloc(KEY_LENGTH);

  pseudorandom_generator(randombytes, random_bytes_length);
  safe_memcpy(encryption_value, text_length, randombytes, random_bytes_length);
  pseudorandom_function(key, key_length, randombytes, random_bytes_length, hash,
                        KEY_LENGTH);
  safe_memcpy(&encryption_value[random_bytes_length],
              text_length - random_bytes_length, hash, KEY_LENGTH);

  free(randombytes);
  free(hash);
}

void generate_encryption_key(const char *plaintext,
                             const size_t plaintext_length, char *key,
                             const size_t key_length) {
  void *random_bytes = safe_malloc(plaintext_length);
  pseudorandom_generator(random_bytes, plaintext_length);
  pseudorandom_function(random_bytes, plaintext_length, plaintext,
                        plaintext_length, key,
                        key_length); // TODO use another pseudorandom function
  free(random_bytes);
}