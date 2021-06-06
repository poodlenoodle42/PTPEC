#pragma once
#include <stdint.h>
#include <openssl/evp.h>
typedef struct _External{
    uint8_t iv[16];
    char salt[PKCS5_SALT_LEN];
}External_Key_Information;

typedef struct _Internal{
    int external_init;
    External_Key_Information ex_key_info;

    uint8_t key[32];
    char* passwd;
}Internal_Key_Information;

extern Internal_Key_Information key_info;

void fill_key_info(char* pwd, External_Key_Information ex_key_info);
External_Key_Information random_external_key_info(void);


//From the Openssl Wiki https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);