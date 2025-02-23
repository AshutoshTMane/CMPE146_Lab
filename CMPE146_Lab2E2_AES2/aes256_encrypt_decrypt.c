
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void encrypt_message_16(char* str, uint8_t* encrypted, uint8_t* key);
void decrypt_message_16(uint8_t* data, uint8_t* decrypted, uint8_t* key);


static uint8_t CipherKey[32] =
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
        0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
static uint8_t DataAESencrypted[16];       // Encrypted data
static uint8_t DataAESdecrypted[16];       // Decrypted data


int main(void)
{
    char* message1 = "0123456789ABCDE";  // 16 character message
    char* message2 = "0123456789";        // Less than 16 character message
    uint8_t i;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    // DEBUGGING
    printf("Input Message 1: %s\n", message1);

    // Encrypt the first message using the encryption function
    encrypt_message_16(message1, DataAESencrypted, CipherKey);

    // Print the encrypted message in hexadecimal format
    printf("Encrypted Message 1: ");
    for (i = 0; i < 16; i++) {
        printf("%02X ", DataAESencrypted[i]);  // Print each byte in hex
    }
    printf("\n");

    // Decrypt the encrypted message to verify correctness
    decrypt_message_16(DataAESencrypted, DataAESdecrypted, CipherKey);

    // Print the decrypted message as a string (stop at null byte)
    printf("Decrypted Message 1: ");
    for (i = 0; i < 16; i++) {
        if (DataAESdecrypted[i] == 0) break; // Stop at null byte to avoid printing garbage
        printf("%c", DataAESdecrypted[i]);   // Print each character in decrypted message
    }
    printf("\n");

    printf("Input Message 2: %s\n", message2);

    encrypt_message_16(message2, DataAESencrypted, CipherKey);

    // Print the encrypted version of the second message in hex format
    printf("Encrypted Message 2: ");
    for (i = 0; i < 16; i++) {
        printf("%02X ", DataAESencrypted[i]);  // Print each byte in hex
    }
    printf("\n");

    // Decrypt the second encrypted message to verify correctness
    decrypt_message_16(DataAESencrypted, DataAESdecrypted, CipherKey);

    // Print the decrypted message for the second input message (as a string)
    printf("Decrypted Message 2: ");
    for (i = 0; i < 16; i++) {
        if (DataAESdecrypted[i] == 0) break; // Stop at null byte
        printf("%c", DataAESdecrypted[i]);   // Print the decrypted message
    }
    printf("\n");

    /* Array DataAESdecrypted should now contain the same data as array Data */
    while(1)
    {
        MAP_PCM_gotoLPM0();
    }
}

void encrypt_message_16(char* str, uint8_t* encrypted, uint8_t* key)
{
    // Configure AES256 with the encryption key
    MAP_AES256_setCipherKey(AES256_BASE, key, AES256_KEYLENGTH_256BIT);

    // Start encryption process
    MAP_AES256_startEncryptData(AES256_BASE, (uint8_t*)str);

    // Wait for the encryption to complete
    while (MAP_AES256_isBusy(AES256_BASE));

    // Retrieve the encrypted data
    MAP_AES256_getDataOut(AES256_BASE, encrypted);
}

void decrypt_message_16(uint8_t* data, uint8_t* decrypted, uint8_t* key)
{
    // Configure AES256 with the decryption key
    MAP_AES256_setDecipherKey(AES256_BASE, key, AES256_KEYLENGTH_256BIT);

    // Start decryption process
    MAP_AES256_startDecryptData(AES256_BASE, data);

    // Wait for the decryption to complete
    while (MAP_AES256_isBusy(AES256_BASE));

    // Retrieve the decrypted data
    MAP_AES256_getDataOut(AES256_BASE, decrypted);
}
