#include "../include/ssoed.h"

#define KEY_LENGTH 3
#define WORD_COUNT 2

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

void safe_memcpy(void *dest, size_t dest_size, const void *src,
                 size_t src_size) {
  if (src_size > dest_size) {
    fprintf(stderr, "Error: Source size exceeds destination size\n");
    exit(EXIT_FAILURE);
  }
  memcpy(dest, src, src_size); // NOLINT
}

void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void safe_strcpy(char *dest, size_t dest_size, const char *src) {
  size_t src_length = 0;
  while (src[src_length] != '\0') {
    src_length++;
  }

  if (src_length > dest_size) {
    fprintf(stderr,
            "Error: Source string is too large for the destination buffer\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < src_length; i++) {
    dest[i] = src[i];
  }
  dest[src_length] = '\0';
}

char *get_encryption_value(size_t text_length, const char *key,
                           size_t key_length) {
  size_t random_bytes_length = text_length - KEY_LENGTH;
  char *encryption_value = safe_malloc(text_length);
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

  return encryption_value;
}

int basic_search(void *key, const char *cipher, const char *word,
                 size_t length) {
  size_t random_bytes_length = length - KEY_LENGTH;
  char *encryption_value = safe_malloc(length);
  char *compare_value = safe_malloc(length);
  char *hash = safe_malloc(KEY_LENGTH);
  int res = 1;

  for (size_t i = 0; i < length; i++) {
    encryption_value[i] = cipher[i] ^ word[i];
  }
  safe_memcpy(compare_value, length, encryption_value, random_bytes_length);
  pseudorandom_function(key, length, compare_value, random_bytes_length, hash,
                        KEY_LENGTH);
  safe_memcpy(&compare_value[random_bytes_length], length - random_bytes_length,
              hash, KEY_LENGTH);

  for (size_t i = 0; i < length; i++) {
    if (encryption_value[i] != compare_value[i]) {
      res = 0;
    }
  }

  free(encryption_value);
  free(compare_value);
  free(hash);
  return res;
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

void printByteArray(const unsigned char *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%02X ", arr[i]);
  }
  printf("\n");
}

void test_controlled_scheme() {
  const char *text[WORD_COUNT] = {"Hello\0", "World\0"};
  char *keys[WORD_COUNT];

  for (int i = 0; i < WORD_COUNT; i++) {
    size_t string_length = strlen(text[i]) + 1;
    char *encryption_key = safe_malloc(string_length);
    generate_encryption_key(text[i], string_length, encryption_key,
                            string_length);
    keys[i] = encryption_key;
    char *encryption_value =
        get_encryption_value(string_length, encryption_key, string_length);
    char *cipher = safe_malloc(string_length);
    char *plain = safe_malloc(string_length);

    cipher[string_length - 1] = '\0';
    plain[string_length - 1] = '\0';

    encrypt_word(encryption_value, text[i], cipher, string_length);
    printf("Encrypted Word: ");
    printByteArray((unsigned char *)cipher, string_length);

    int res = basic_search((void *)keys[0], cipher, text[i], string_length);

    if (res == 0) {
      printf("Did not find %s\n", text[i]);
    } else {
      printf("Did find %s\n", text[i]);
    }

    encrypt_word(encryption_value, cipher, plain, string_length);
    printf("Decrypted Word: %s\n", plain);

    free(encryption_value);
    free(cipher);
    free(plain);
  }

  for (int i = 0; i < WORD_COUNT; i++) {
    free(keys[i]);
  }
}

void test_basic_scheme() {
  const char *text[WORD_COUNT] = {"Hello\0", "World\0"};
  const char *keys[WORD_COUNT] = {"World\0", "Hello\0"};

  for (int i = 0; i < WORD_COUNT; i++) {
    size_t string_length = strlen(text[i]) + 1;
    char *key = get_encryption_value(string_length, keys[i], string_length);
    char *cipher = safe_malloc(string_length);
    char *plain = safe_malloc(string_length);

    cipher[string_length - 1] = '\0';
    plain[string_length - 1] = '\0';

    encrypt_word(key, text[i], cipher, string_length);
    printf("Encrypted Word: ");
    printByteArray((unsigned char *)cipher, string_length);

    int res = basic_search((void *)keys[1], cipher, text[i], string_length);

    if (res == 0) {
      printf("Did not find %s\n", text[i]);
    } else {
      printf("Did find %s\n", text[i]);
    }

    encrypt_word(key, cipher, plain, string_length);
    printf("Decrypted Word: %s\n", plain);

    free(key);
    free(cipher);
    free(plain);
  }
}

int main(void) {
  if (sodium_init() < 0) {
    perror("Sodium initialization failed");
    exit(EXIT_FAILURE);
  }

  // test_basic_scheme();
  // printf("\n");
  test_controlled_scheme();
  return 0;
}
