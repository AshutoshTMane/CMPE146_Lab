
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

    uint32_t alias_addr = 0x42000000;
    uint32_t port_addr = 0x40000000;


    uint8_t button, previousVal;

    uint8_t* button_alias = (uint8_t*) ((port_module + port1_in - port_addr) * 32 + (1 * 4) + alias_addr);
    uint8_t* led_alias = (uint8_t*) ((port_module + port2_out - port_addr) * 32  + (4 * 2) + alias_addr);

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);

    button = *button_alias;

    previousVal = button;

    //config push button as input P1.1

    while(1){
        button = *button_alias;

        if(button != previousVal && (previousVal) != 0x0){
            *led_alias ^= 0x1;
        }
        previousVal = button;
    }


}
