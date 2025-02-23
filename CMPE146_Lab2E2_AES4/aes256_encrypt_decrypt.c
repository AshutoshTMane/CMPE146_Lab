/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Maximum size for encrypted and decrypted data buffers
#define MAX_DATA_SIZE 1600

// Buffers to hold encrypted and decrypted data
static uint8_t DataAESencrypted[MAX_DATA_SIZE];
static uint8_t DataAESdecrypted[MAX_DATA_SIZE];

// Define the text key (same for both)
char key1[] = "Password is CMPE146-01";

// Function prototypes for encryption and decryption
void encrypt_message(char* str, uint8_t* encrypted, char* key);
void decrypt_message(uint8_t* data, int data_length, char* decrypted, char* key);

int main(void)
{
    // Test messages
    int m, i;
    char* messages[] = {
        "12345",
        "0123456789ABCDEF",
        "The quick brown fox jumps over the lazy dog."
    };

    for (m = 0; m < 3; m++) {
        char* message = messages[m];
        int message_len = strlen(message);

        // Encrypt the message
        encrypt_message(message, DataAESencrypted, key1);

        // Print the original message
        printf("Input Message: %s\n", message);

        // Print the encrypted message in hex format
        printf("Encrypted Message: ");
        for (i = 0; i < (message_len + 15) / 16 * 16; i++) {
            printf("%02X ", DataAESencrypted[i]);
        }
        printf("\n");

        // Decrypt the encrypted message
        decrypt_message(DataAESencrypted, message_len, DataAESdecrypted, key1);

        // Print the decrypted message
        printf("Decrypted Message: ");
        for (i = 0; i < message_len; i++) {
            printf("%c", DataAESdecrypted[i]);
        }
        printf("\n\n");
    }

    return 0;
}

void encrypt_message(char* str, uint8_t* encrypted, char* key)
{
    int block, i;

    // Create a local copy of the key to avoid memory issues
    char key_copy[32];  // AES 256-bit key requires 32 bytes
    memset(key_copy, 0, sizeof(key_copy));  // Ensure no garbage values
    memcpy(key_copy, key, strlen(key));  // Copy the key safely

    // Initialize AES module with DriverLib
    MAP_AES256_setCipherKey(AES256_BASE, (uint8_t*)key_copy, AES256_KEYLENGTH_256BIT);

    int str_len = strlen(str);
    int blocks = (str_len + 15) / 16;  // Calculate the number of 16-byte blocks needed

    for (block = 0; block < blocks; block++) {
        // Get the current block to process (16 bytes at a time)
        uint8_t block_data[16] = {0};
        int offset = block * 16;
        for (i = 0; i < 16 && offset + i < str_len; i++) {
            block_data[i] = str[offset + i];  // Copy data into block (pad with 0 if needed)
        }

        // Encrypt the block using DriverLib AES encryption function
        MAP_AES256_startEncryptData(AES256_BASE, block_data);
        while (MAP_AES256_isBusy(AES256_BASE));  // Wait for encryption to complete

        // Retrieve the encrypted data
        MAP_AES256_getDataOut(AES256_BASE, encrypted + block * 16);
    }
}

void decrypt_message(uint8_t* data, int data_length, char* decrypted, char* key)
{
    int block, i;
    // Create a local copy of the key to avoid memory issues
    char key_copy[32];  // AES 256-bit key requires 32 bytes
    memset(key_copy, 0, sizeof(key_copy));  // Ensure no garbage values
    memcpy(key_copy, key, strlen(key));  // Copy the key safely

    // Initialize AES module with DriverLib
    MAP_AES256_setDecipherKey(AES256_BASE, (uint8_t*)key_copy, AES256_KEYLENGTH_256BIT);

    int blocks = (data_length + 15) / 16;  // Calculate the number of blocks

    for (block = 0; block < blocks; block++) {
        // Get the current block to process (16 bytes at a time)
        uint8_t block_data[16] = {0};
        for (i = 0; i < 16; i++) {
            block_data[i] = data[block * 16 + i];  // Copy data from the encrypted buffer
        }

        // Decrypt the block using DriverLib AES decryption function
        MAP_AES256_startDecryptData(AES256_BASE, block_data);
        while (MAP_AES256_isBusy(AES256_BASE));  // Wait for decryption to complete

        // Retrieve the decrypted data
        MAP_AES256_getDataOut(AES256_BASE, decrypted + block * 16);
    }
}
