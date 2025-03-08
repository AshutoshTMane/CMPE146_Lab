
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>


#define CRC32_POLY              0xEDB88320
#define CRC32_INIT              0xFFFFFFFF

uint32_t compute_simple_checksum(uint8_t* data, uint32_t length);

// static const uint8_t myData[9] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
static uint8_t myData[10240];
static uint32_t calculateCRC32(uint8_t* data, uint32_t length);

volatile uint32_t hwCalculatedCRC, swCalculatedCRC;

//![Simple CRC32 Example] 
int main(void)
{
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);
    printf("%u\n", MAP_CS_getMCLK());

    uint32_t ii;

    /* Stop WDT */
    MAP_WDT_A_holdTimer();

    MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);

    for (ii = 0; ii < 9; ii++)
        MAP_CRC32_set8BitData(myData[ii], CRC32_MODE);

    uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);

    /* Getting the result from the hardware module */
    hwCalculatedCRC = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;

    uint32_t t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
    uint32_t difference = t0 - t1;
    printf("hwC: %08x\n", hwCalculatedCRC);
    printf("hwC Counter diff: %u\n",  difference);


    t0 = MAP_Timer32_getValue(TIMER32_0_BASE);

    /* Calculating the CRC32 checksum through software */
    swCalculatedCRC = calculateCRC32((uint8_t*) myData, 9);

    t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
    difference = t0 - t1;
    printf("swC: %08x\n", swCalculatedCRC);
    printf("swC Counter diff: %u\n",  difference);

    t0 = MAP_Timer32_getValue(TIMER32_0_BASE);

    uint32_t checksum = compute_simple_checksum(myData, 10240);

    t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
    difference = t0 - t1;
    printf("checksum: %08x\n", checksum);
    printf("Counter diff: %u\n",  difference);

    /* Pause for the debugger */
    __no_operation();
}
//![Simple CRC32 Example] 

/* Standard software calculation of CRC32 */
static uint32_t calculateCRC32(uint8_t* data, uint32_t length)
{
    uint32_t ii, jj, byte, crc, mask;;

    crc = 0xFFFFFFFF;

    for(ii=0;ii<length;ii++)
    {
        byte = data[ii];
        crc = crc ^ byte;

        for (jj = 0; jj < 8; jj++)
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (CRC32_POLY & mask);
        }

    }

    return ~crc;
}


uint32_t compute_simple_checksum(uint8_t* data, uint32_t length) {
    uint32_t checksum = 0;
    uint32_t i;

    for(i = 0; i < length; i++){

        uint32_t bytePosition = i % 4;
        uint32_t shiftAmount = bytePosition * 8;

        uint32_t byte = (uint32_t)data[i] << shiftAmount;

        checksum += byte;
    }
    checksum = ~checksum;

    return checksum;
}
