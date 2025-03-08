/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

uint32_t compute_simple_checksum(uint8_t* data, uint32_t length);

int main(void)
{
    static uint8_t myData[10240];
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    uint32_t checksum = compute_simple_checksum(myData, 10240);

    printf("checksum: %08x\n", checksum);

    while(1)
    {
        
    }
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
