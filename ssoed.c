#include "ssoed.h"

#define KEY_LENGTH 3

void pseudorandom_generator(void *buffer, const size_t size) {
    randombytes_buf(buffer, size);
}

void pseudorandom_function(const void *key, size_t key_size, const char *plaintext, size_t plaintext_size, char *hash, size_t hash_size) {
    crypto_generichash((unsigned char *) hash, hash_size, (unsigned char *) plaintext, plaintext_size, key, key_size);
}

int encrypt_word(const char *key, const char *plain, char *cipher, size_t length) {
    for (size_t i = 0; i < length; i++) {
        cipher[i] = plain[i] ^ key[i];
    }
    return 0;
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

char *get_encryption_value(const char *text, size_t text_length, const char *key, size_t key_length) {
    size_t random_bytes_length = text_length - KEY_LENGTH;
    char *encryption_value = safe_malloc(text_length);
    void *randombytes = safe_malloc(random_bytes_length);
    char *hash = safe_malloc(KEY_LENGTH);

    pseudorandom_generator(randombytes, random_bytes_length);
    memcpy(encryption_value, randombytes, random_bytes_length);
    pseudorandom_function(key, key_length, randombytes, random_bytes_length, hash, KEY_LENGTH);
    memcpy(&encryption_value[random_bytes_length], hash, KEY_LENGTH);

    free(randombytes);
    free(hash);

    return encryption_value;
}

int basic_search(void *key, const char *cipher, const char *word, size_t length) {
    size_t random_bytes_length = length - KEY_LENGTH;
    char *encryption_value = safe_malloc(length);
    char *compare_value = safe_malloc(length);
    char *hash = safe_malloc(KEY_LENGTH);

    for (size_t i = 0; i < length; i++) {
        encryption_value[i] = cipher[i] ^ word[i];
    }
    memcpy(compare_value, encryption_value, random_bytes_length);
    pseudorandom_function(key, length, compare_value, random_bytes_length, hash, KEY_LENGTH);
    memcpy(&compare_value[random_bytes_length], hash, KEY_LENGTH);

    for (size_t i = 0; i < length; i++) {
        if (encryption_value[i] != compare_value[i]) {
            return 0;
        }
    }

    free(encryption_value);
    free(compare_value);
    free(hash);

    return 1;
}

void test_basic_scheme() {
    const char *text[2] = {"Hello", "World"};
    const char *keys[2] = {"World", "Hello"};

    for (int i = 0; i < 2; i++) {
        size_t string_length = strlen(text[i]);
        char *key = get_encryption_value(keys[i], string_length, keys[i], string_length);
        char *cipher = safe_malloc(string_length);
        char *plain = safe_malloc(string_length);

        strcpy(cipher, text[i]);
        strcpy(plain, text[i]);

        encrypt_word(key, text[i], cipher, string_length);
        printf("Encrypted Word: %s\n", cipher);

        if (basic_search((void *)keys[1], cipher, text[i], string_length) == 0) {
            printf("Didn't find %s with key %s in %s encrypted with key %s\n", text[i], keys[1], cipher, keys[i]);
        } else {
            printf("Did find %s with key %s in %s encrypted with key %s\n", text[i], keys[1], cipher, keys[i]);
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

    test_basic_scheme();
    return 0;
}
