
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{

    uint32_t port_module = 0x40004C00; // Base address for the GPIO module, starting address for the registers controling GPIO
    uint8_t port1_in = 0x00; // Offset for Port 1 input register, used for accessing input data on port 1
    uint8_t port2_out = 0x03; // Offset for Port 2 output register, constols the output value for port 2

    // Variable to store button state and previous state
    uint8_t button, previousVal;
    
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
    uint8_t *LED_ADDR = (uint8_t*)(port_module + port2_out);


    // Configuring P2.2 (Pin 2 on Port 2) as an output pin for controlling the LED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    // Set P1.1 as an input pin with an internal pull-up resistor so the button is active-low, meaning pressing it changes it to 0 (activation requirement)
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    // Initialize button state to track state changes, this will always start at 1
    button = MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
    previousVal = button;

    while(1){
        // Reading the current state of the button by directly accessing the Port 1 input register
        button = *(uint8_t*) (port_module + port1_in);

        // The second condition checks if the previous value of the button was high (not pressed)
        if(button != previousVal && (previousVal & 0x2) != 0x0){
            *LED_ADDR ^= 0x4; // The LED is controlled by the 3rd bit of Port 2 (Pin 2)
        }
        
        // Updating the previous button state for the next iteration of the loop
        previousVal = button;
    }

}
