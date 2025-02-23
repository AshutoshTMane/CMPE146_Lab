
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Statics */
// Original data to encrypt
static uint8_t Data[16] =
{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc,
        0xdd, 0xee, 0xff };
// AES-256 encryption key (256 bits = 32 bytes)
static uint8_t CipherKey[32] =
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
        0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
static uint8_t DataAESencrypted[16]; // Array to store the encrypted data
static uint8_t DataAESdecrypted[16]; // Array to store the decrypted data


int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    //![Simple AES256 Example]
    /* Load a cipher key to module */
    MAP_AES256_setCipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);

    /* Encrypt data with preloaded cipher key */
    MAP_AES256_encryptData(AES256_BASE, Data, DataAESencrypted);

    /* Load a decipher key to module */
    MAP_AES256_setDecipherKey(AES256_BASE, CipherKey,AES256_KEYLENGTH_256BIT);

    /* Decrypt data with keys that were generated during encryption - takes
     214 MCLK cyles. This function will generate all round keys needed for
     decryption first and then the encryption process starts */
    MAP_AES256_decryptData(AES256_BASE, DataAESencrypted, DataAESdecrypted);
    //![Simple AES256 Example]
    
    uint8_t i = 0; // Loop counter

    // Print the original data
    printf("Input Data: ");
    for (i = 0; i < 16; i++)
        {
        printf("%02X ", Data[i]); // Printing out each byte in hexadecimal
        }
    printf("\n");

    // Print the encrypted data
    printf("Encrypted Data: ");
    for (i = 0; i < 16; i++)
        {
        printf("%02X ", DataAESencrypted[i]);
        }
    printf("\n");

    printf("Decrypted Data: ");
    for (i = 0; i < 16; i++)
        {
        printf("%02X ", DataAESdecrypted[i]);
        }
    printf("\n");


    /* Array DataAESdecrypted should now contain the same data as array Data */
    while(1)
    {
        MAP_PCM_gotoLPM0(); // This is used to transition to Low Power Mode 0 (LPM0) to save power while the program is idle
    }
}
