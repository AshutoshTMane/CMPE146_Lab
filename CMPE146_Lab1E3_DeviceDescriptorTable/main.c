/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

int main(void)
{
    // Initialize pointer variables for easy iteration during loops
    uint32_t* checkSumPtr = (uint32_t*) 0x00201000;
    uint32_t* endWordPtr = (uint32_t*) 0x00201148;

    //uint32_t iterator = 0x04;

    // To my understanding, this table involves 1. the tag, 2. the length, 3. the data items
    // Descriptor Table Addresses can be found on page 175 of the MCU datasheet (section 6.13)
    // General Info on this table can be found on page 289 of the Technical Reference document (section 4.9)


    // Print CheckSum value by de-referencing the checkSumPtr (per unit)
    printf("Checksum: %08X\n", *checkSumPtr);

    // Set currentAddress to first TLV block address (if any)
    uint32_t* currentAddress = checkSumPtr + 7; // This may need to be 0x1C

    // Use for-loop to iterate until the currentAddress matches the endWord address
    // Iterate using ++ because this is a pointer variable and not integer
    for (; currentAddress < endWordPtr; currentAddress++){
        if (currentAddress >= endWordPtr) break;

        uint32_t tag = *currentAddress;
        currentAddress++;

        uint32_t length = *currentAddress;
        currentAddress++;

        printf("Tag: %08X Length: %08X ", tag, length);

        int i = 0;
        for (; i < length; i++){
            printf("%08X ", *currentAddress);
            currentAddress++;
        }
        printf("\n");
    }

    printf("End Word: %08X\n", *endWordPtr);

    return 0;
}
