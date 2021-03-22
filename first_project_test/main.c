#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "spi_comm.h"
#include "sensors/proximity.h"
#include "motors.h"





int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    // Initiate and turn off all eight ring LEDs
    clear_leds();
    spi_comm_start();

    // Initiate the motors
    motors_init();

    //messagebus_tbus;
    //MUTEX_DECL(bus_lock);
    //CONDVAR_DECL(bus_condvar);
    //
    //// Initiate the inter processcommunication bus
    //messagebus_init(&bus, &bus_lock, &bus_condvar);

    /* Infinite loop. */
    while (1) {
    	// LED setting
    	//set_led(LED1, 2);
    	//set_rgb_led(LED2, 5, 5, 5);
    	set_body_led(2);
    	//waits 1 second
    	chThdSleepMilliseconds(1000);

    	// Motor setting for running
    	left_motor_set_speed(-300);
    	right_motor_set_speed(300);
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
