/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#define ROUNDS 12
#define RATE 8
#define CAPACITY 32
#define KEY_SIZE 16

typedef uint64_t word;

word rotl(word x, unsigned int n) {
    return (x << n) | (x >> (64 - n));
}

void ascon_permutation(word* state) {
    int i;

    for (i = 0; i < ROUNDS; ++i) {
        state[0] ^= state[1]; state[4] ^= state[0];
        state[2] ^= state[4]; state[1] ^= state[2];
        state[4] ^= state[1]; state[3] ^= state[4];
        state[2] ^= state[3]; state[0] ^= state[2];

        state[1] = rotl(state[1], 19);
        state[2] = rotl(state[2], 28);
        state[3] = rotl(state[3], 36);
        state[4] = rotl(state[4], 45);

        state[1] ^= state[0]; state[4] ^= state[1];
        state[2] ^= state[4]; state[1] ^= state[2];
        state[4] ^= state[1]; state[3] ^= state[4];
        state[2] ^= state[3]; state[0] ^= state[2];

        state[1] = rotl(state[1], 61);
        state[2] = rotl(state[2], 39);
        state[3] = rotl(state[3], 18);
        state[4] = rotl(state[4],  9);
    }
}

void ascon_key_setup(const uint8_t* key, word* state) {
    state[0] = 0x0;
    state[1] = ((word)key[0]) | (((word)key[1]) << 8) | (((word)key[2]) << 16) | (((word)key[3]) << 24);
    state[2] = ((word)key[4]) | (((word)key[5]) << 8) | (((word)key[6]) << 16) | (((word)key[7]) << 24);
    state[3] = ((word)key[8]) | (((word)key[9]) << 8) | (((word)key[10]) << 16) | (((word)key[11]) << 24);
    state[4] = ((word)key[12]) | (((word)key[13]) << 8) | (((word)key[14]) << 16) | (((word)key[15]) << 24);
}

void ascon_encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext) {
    word state[5] = {0};
    int i;

    ascon_key_setup(key, state);

    for (i = 0; i < RATE; ++i) {
        state[i] ^= ((word)plaintext[i]) << (8 * i);
    }

    ascon_permutation(state);

    for (i = 0; i < RATE; ++i) {
        ciphertext[i] = state[i] & 0xFF;
    }
}

void ascon_decrypt(const uint8_t* ciphertext, const uint8_t* key, uint8_t* plaintext) {
    word state[5] = {0};
    int i;

    ascon_key_setup(key, state);

    for (i = 0; i < RATE; ++i) {
        state[i] ^= ((word)ciphertext[i]) << (8 * i);
    }

    ascon_permutation(state);

    for (i = 0; i < RATE; ++i) {
        plaintext[i] = state[i] & 0xFF;
    }
}

int main() {
    // Example usage
    const uint8_t key[KEY_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    const uint8_t plaintext[RATE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    // Adjust the size of ciphertext and decrypted_text
    uint8_t ciphertext[RATE] = {0};
    uint8_t decrypted_text[RATE] = {0};
    printf("Plaintext: ");
    for (int i = 0; i < RATE; ++i) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");

    ascon_encrypt(plaintext, key, ciphertext);

    printf("Ciphertext: ");
    // Iterate over the correct size
    for (int i = 0; i < RATE; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    ascon_decrypt(ciphertext, key, decrypted_text);

    printf("Decrypted Text: ");
    for (int i = 0; i < RATE; ++i) {
        printf("%02X ", decrypted_text[i]);
    }
    printf("\n");
    // nfhg/f

    return 0;
}
