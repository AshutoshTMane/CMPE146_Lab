
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{

    // Variable to store button state and previous state
    uint8_t button, previousVal;

    MAP_WDT_A_holdTimer();

    // Set P2.2 (Blue LED) as an output pin and make sure LED is set to OFF at startup
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    // Set P1.1 as an input pin with an internal pull-up resistor so the button is active-low, meaning pressing it changes it to 0 (activation requirement)
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    // Initialize button state to track state changes, this will always start at 1
    button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
    previousVal = button;

    while (1)
    {
        // Read the current state of the button
        button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);

        // If the button state changed from high to low (button pressed), then toggle LED
        if(button != previousVal && previousVal == GPIO_INPUT_PIN_HIGH)
        {
            // This will change the LED to ON if OFF and vice versa because we are comparing with both the previous value and also checking whether a press occurred (so toggle doesn't occur during hold and release)
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
        }
        // Update previous button state for next iteration
        previousVal = button;
    }
}
