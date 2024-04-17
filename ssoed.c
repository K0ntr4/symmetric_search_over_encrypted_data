#include <sodium.h>
#include <string.h>

#define KEY_SIZE 32
#define BLOCK_SIZE 16
#define NONCE_SIZE 24

 void split_text_into_blocks(const unsigned char *text, size_t text_length, unsigned char *blocks[], size_t *num_blocks) {
    *num_blocks = (text_length + BLOCK_SIZE - 1) / BLOCK_SIZE;

     for (size_t i = 0; i < *num_blocks; i++) {
         blocks[i] = (unsigned char *)malloc(BLOCK_SIZE);
         if (blocks[i] == NULL) {
             printf("Error: Memory allocation failed\n");
             exit(1);
         }
     }

    for (size_t i = 0; i < *num_blocks; i++) {
        size_t bytes_to_copy = (i + 1) * BLOCK_SIZE <= text_length ? BLOCK_SIZE : text_length - i * BLOCK_SIZE;
        memcpy(blocks[i], text + i * BLOCK_SIZE, bytes_to_copy);
    }
}

void free_blocks(unsigned char *blocks[], size_t num_blocks) {
    for (size_t i = 0; i < num_blocks; i++) {
        free(blocks[i]);
    }
}

void encrypt_block(const unsigned char *key, const unsigned char *nonce, const unsigned char *plaintext, unsigned char *ciphertext) {
    crypto_secretbox_easy(ciphertext, plaintext, BLOCK_SIZE, nonce, key);
}

int decrypt_block(const unsigned char *key, const unsigned char *nonce, const unsigned char *ciphertext, unsigned char *plaintext) {
    return crypto_secretbox_open_easy(plaintext, ciphertext, BLOCK_SIZE + crypto_secretbox_MACBYTES, nonce, key);
}

int buffers_equal(const unsigned char *buffer1, const unsigned char *buffer2, size_t size) {
    return memcmp(buffer1, buffer2, size) == 0;
}

void pseudorandom_function(const unsigned char *key, const unsigned char *message, unsigned char *output) {
    crypto_generichash(output, crypto_generichash_BYTES, message, BLOCK_SIZE, key, KEY_SIZE);
}

void generate_pseudorandom(unsigned char *buffer, size_t buffer_size) {
    randombytes_buf(buffer, buffer_size);
}

void test_pseudorandom_function() {
    unsigned char key[KEY_SIZE];
    unsigned char message[BLOCK_SIZE] = {"Hello World\0"};
    unsigned char output[crypto_generichash_BYTES];
    unsigned char output2[crypto_generichash_BYTES];

    randombytes_buf(key, KEY_SIZE);

    pseudorandom_function(key, message, output);
    pseudorandom_function(key, message, output2);

    if (buffers_equal(output, output2, crypto_generichash_BYTES)) {
        printf("Output matches for input %s\n", message);
    } else {
        printf("Output does not match for input %s\n", message);
    }

    printf("Pseudorandom Output:\n");
    for (int i = 0; i < crypto_generichash_BYTES; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");
}

void test_split_text_into_blocks() {
    const char *text = "This is a longer text that needs to be split into blocks of the specified size.";
    size_t text_length = strlen(text);
    size_t num_blocks = (text_length + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned char *blocks[num_blocks];

    split_text_into_blocks((const unsigned char *)text, text_length, blocks, &num_blocks);

    printf("Text Blocks:\n");
    for (size_t i = 0; i < num_blocks; i++) {
        printf("Block %zu: ", i + 1);
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
            printf("%02x", blocks[i][j]);
        }
        printf("\n");
    }
    free_blocks(blocks, num_blocks);
}

void test_encrypt_decrypt_block() {
    unsigned char key[KEY_SIZE];
    unsigned char nonce[NONCE_SIZE];
    unsigned char plaintext[BLOCK_SIZE] = {"Hello World\0"};
    unsigned char ciphertext[BLOCK_SIZE + crypto_secretbox_MACBYTES];
    unsigned char decrypted_plaintext[BLOCK_SIZE];
    
    randombytes_buf(key, KEY_SIZE);
    randombytes_buf(nonce, NONCE_SIZE);
    
    encrypt_block(key, nonce, plaintext, ciphertext);

    int decryption_result = decrypt_block(key, nonce, ciphertext, decrypted_plaintext);
    if (decryption_result != 0) {
        printf("Decryption failed\n");
        return;
    }
    
    if (buffers_equal(plaintext, decrypted_plaintext, BLOCK_SIZE)) {
        printf("Encryption and decryption successful.\n");
    } else {
        printf("Decryption failed. Decrypted plaintext does not match original plaintext.\n");
    }
}

int main(void) {
    if (sodium_init() < 0) {
        exit(2);
    }

    test_pseudorandom_function();
    test_split_text_into_blocks();
    test_encrypt_decrypt_block();
    return 0;
}
