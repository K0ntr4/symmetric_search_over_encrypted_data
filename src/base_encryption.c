#include "../include/base_encryption.h"

// Generate pseudorandom bytes and store them in the buffer
void pseudorandom_generator(void *buffer, const size_t size) {
  randombytes_buf(buffer, size);
}

// Use a pseudorandom function to generate a hash based on the key and plaintext
void pseudorandom_function(const void *key, size_t key_size,
                           const char *plaintext, size_t plaintext_size,
                           char *hash, size_t hash_size) {
  // Use cryptographic hash function to generate the hash
  crypto_generichash((unsigned char *)hash, hash_size,
                     (unsigned char *)plaintext, plaintext_size, key, key_size);
}

// Encrypt a word using a simple bitwise XOR operation between plaintext and key
void encrypt_word(const char *key, const char *plain, char *cipher,
                  size_t length) {
  // Perform bitwise XOR operation between plaintext and key
  for (size_t i = 0; i < length; i++) {
    cipher[i] = plain[i] ^ key[i];
  }
}

// Generate an encryption value using key and plaintext
void get_encryption_value(char *encryption_value, size_t text_length,
                          const char *key, size_t key_length) {
  // Calculate the length of random bytes
  size_t random_bytes_length = text_length - KEY_LENGTH;

  // Allocate memory for random bytes and hash
  void *randombytes = safe_malloc(random_bytes_length);
  char *hash = safe_malloc(KEY_LENGTH);

  // Generate random bytes
  pseudorandom_generator(randombytes, random_bytes_length);

  // Copy random bytes to encryption value
  safe_memcpy(encryption_value, text_length, randombytes, random_bytes_length);

  // Use pseudorandom function with key to generate hash
  pseudorandom_function(key, key_length, randombytes, random_bytes_length, hash,
                        KEY_LENGTH);

  // Copy hash to encryption value
  safe_memcpy(&encryption_value[random_bytes_length],
              text_length - random_bytes_length, hash, KEY_LENGTH);

  // Free allocated memory
  free(randombytes);
  free(hash);
}

// Generate an encryption key based on plaintext using a pseudorandom function
void generate_encryption_key(const char *plaintext,
                             const size_t plaintext_length, char *key,
                             const size_t key_length) {
  // Allocate memory for random bytes
  void *random_bytes = safe_malloc(plaintext_length);

  // Generate random bytes
  pseudorandom_generator(random_bytes, plaintext_length);

  // Use pseudorandom function with plaintext and random bytes to generate key
  pseudorandom_function(random_bytes, plaintext_length, plaintext,
                        plaintext_length, key, key_length);

  // Free allocated memory
  free(random_bytes);
}
