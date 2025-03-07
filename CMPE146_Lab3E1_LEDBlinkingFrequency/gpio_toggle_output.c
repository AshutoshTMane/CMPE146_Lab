#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//![Simple GPIO Config]
int main(void)
{

    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
    TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);
    printf("%u\n", MAP_CS_getMCLK());

    int timeMs, freqHz;

    uint32_t systemClock = MAP_CS_getMCLK();
    printf("System clock: %u Hz\n", systemClock);

    volatile uint32_t ii;

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    //uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);

    while (1)
    {
        uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
        /* Delay Loop */
        for(ii=0;ii<50000;ii++)
        {

        }
        uint32_t t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
        uint32_t difference = t0 - t1;

        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);

        float timeMs = (float)difference / (float)systemClock * 1000.0f;
        float freqHz = 1000.0f / (timeMs * 2);

        printf("Counter diff: %u, Duration: %.2f ms, Frequency: %.2f Hz\r\n", difference, timeMs, freqHz);

    }
}
//![Simple GPIO Config]


