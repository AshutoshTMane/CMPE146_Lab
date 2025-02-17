
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{

    const uint32_t port_module = 0x40004C00;
    const uint8_t PORT1_IN = 0x000;
    const uint8_t PORT2_IN = 0x001;
    const uint8_t PORT1_OUT = 0x002;
    const uint8_t PORT2_OUT = 0x03;

    uint8_t button, previousVal;
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);

    previousVal = button;

    while (1)
    {
        button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);

        if(button != previousVal && previousVal == GPIO_INPUT_PIN_HIGH)
        {
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
        }
        previousVal = button;
    }
}
