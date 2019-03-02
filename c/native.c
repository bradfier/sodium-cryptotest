#include <stdio.h>
#include <stdlib.h>
#include <sodium.h>
#include <string.h>

/* 64MiB */
#define BUFSIZE 67108864

int main()
{
    if (sodium_init() < 0) {
        printf("Failed to init libsodium\n");
        return 1;
    }

    /* Allocate a buffer and read the data from 'test_data' */
    unsigned char *plaintext = malloc(BUFSIZE);
    FILE *fp = fopen("../test_data", "r");

    size_t len = 0;

    if (fp != NULL) {
        len = fread(plaintext, sizeof(char), BUFSIZE, fp);
        if (ferror(fp) != 0) {
            fputs("Error reading file", stderr);
            return 1;
        } else {
            printf("Read %d bytes\n", len);
        }
        fclose(fp);
    } else {
        printf("Failed opening file\n");
        return 1;
    }

    /* Encryption parameters, default from libsodium */
    unsigned char key[crypto_secretbox_KEYBYTES];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char *ciphertext = malloc(len + crypto_secretbox_MACBYTES);

    /* Key and nonce generation */
    crypto_secretbox_keygen(key);
    randombytes_buf(nonce, sizeof(nonce));

    /* Do encryption */
    crypto_secretbox_easy(ciphertext, plaintext, len, nonce, key);

    /* Decryption of the existing buffer */
    unsigned char *decrypted = malloc(len);
    if (crypto_secretbox_open_easy(decrypted, ciphertext, len + crypto_secretbox_MACBYTES, nonce, key)) {
        printf("MAC failed somehow");
        return 1;
    }

    if (memcmp(plaintext, decrypted, len)) {
        printf("Plaintext and decrypted buffer do not match!\n");
        return 1;
    } else {
        printf("Plaintext and decrypted buffer match.\n");
    }


    free(plaintext);
    free(ciphertext);
    free(decrypted);

    return 0;
}
