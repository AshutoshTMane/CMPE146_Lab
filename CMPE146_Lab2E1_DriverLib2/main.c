
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{

    uint32_t port_module = 0x40004C00;
    uint8_t port1_in = 0x00;
    uint8_t port2_out = 0x03;

    uint8_t button, previousVal;
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
    uint8_t *LED_ADDR = (uint8_t*)(port_module + port2_out);


    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);

    previousVal = button;

    while(1){
        button = *(uint8_t*) (port_module + port1_in);

        if(button != previousVal && (previousVal & 0x2) != 0x0){
            *LED_ADDR ^= 0x4;
        }
        previousVal = button;
    }

}
