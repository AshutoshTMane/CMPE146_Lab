
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{

    uint8_t button, previousVal;
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

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
