/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


void encrypt_message_16(char* str, uint8_t* encrypted, char* key);
void decrypt_message_16(uint8_t* data, uint8_t* decrypted, char* key);

/* Statics */
static uint8_t DataAESencrypted[16];  // Encrypted data buffer
static uint8_t DataAESdecrypted[16];  // Decrypted data buffer

// Define the text keys (same for both)
char key1[] = "Password is CMPE146-01";
char key2[] = "Password is CMPE146-01";

int main(void)
{

    uint8_t i;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    // Define the input message
    char* message1 = "0123456789ABCDE";  // Message to encrypt/decrypt

    // Encrypt the message using key1
    encrypt_message_16("0123456789ABCDE", DataAESencrypted, key1);

    // Print the input message for clarity
    printf("Input Message: %s\n", message1);

    // Print the encrypted message in hexadecimal format
    printf("Encrypted Message: ");
    for (i = 0; i < 16; i++) {
        printf("%02X ", DataAESencrypted[i]);  // Print each byte in hex
    }
    printf("\n");

    // Decrypt the encrypted message using key1
    decrypt_message_16(DataAESencrypted, DataAESdecrypted, key1);

    // Print the decrypted message as a string (stop at null byte)
    printf("Decrypted Message (key1): ");
    for (i = 0; i < 16; i++) {
        if (DataAESdecrypted[i] == 0)
            break;
        printf("%c", DataAESdecrypted[i]);   // Print each character
    }
    printf("\n");

    // Now, use key2 to decrypt (same text key, should work)
    decrypt_message_16(DataAESencrypted, DataAESdecrypted, key2);

    // Print the decrypted message using key2
    printf("Decrypted Message (key2): ");
    for (i = 0; i < 16; i++) {
        if (DataAESdecrypted[i] == 0) break; // Stop at null byte
        printf("%c", DataAESdecrypted[i]);   // Print each character
    }
    printf("\n");

    while (1)
    {
        MAP_PCM_gotoLPM0();  // Go to low-power mode
    }
}


void encrypt_message_16(char* str, uint8_t* encrypted, char* key)
{
    // Create a local copy of the key to avoid memory issues
    char key_copy[32];  // AES 256-bit key requires 32 bytes
    memset(key_copy, 0, sizeof(key_copy));  // Ensure no garbage values
    memcpy(key_copy, key, strlen(key));  // Copy the key safely

    // Configure AES256 with the encryption key
    MAP_AES256_setCipherKey(AES256_BASE, (uint8_t*)key_copy, AES256_KEYLENGTH_256BIT);

    // Start encryption process
    MAP_AES256_startEncryptData(AES256_BASE, (uint8_t*)str);

    // Wait for the encryption to complete
    while (MAP_AES256_isBusy(AES256_BASE));

    // Retrieve the encrypted data
    MAP_AES256_getDataOut(AES256_BASE, encrypted);
}

void decrypt_message_16(uint8_t* data, uint8_t* decrypted, char* key)
{
    // Create a local copy of the key to avoid memory issues
    char key_copy[32];  // AES 256-bit key requires 32 bytes
    memset(key_copy, 0, sizeof(key_copy));  // Ensure no garbage values
    memcpy(key_copy, key, strlen(key));  // Copy the key safely

    // Configure AES256 with the decryption key
    MAP_AES256_setDecipherKey(AES256_BASE, (uint8_t*)key_copy, AES256_KEYLENGTH_256BIT);

    // Start decryption process
    MAP_AES256_startDecryptData(AES256_BASE, data);

    // Wait for the decryption to complete
    while (MAP_AES256_isBusy(AES256_BASE));

    // Retrieve the decrypted data
    MAP_AES256_getDataOut(AES256_BASE, decrypted);
}
