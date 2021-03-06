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
#include "sensors/VL53L0X/VL53L0X.h"
#include "chprintf.h"
#include "usbcfg.h"
#include "epuck1x/uart/e_uart_char.h"
#include "stdio.h"
#include "serial_comm.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();
    // Initiate the USB communication peripheral
    clear_leds();
    spi_comm_start();
    // Initiate the internal process communication bus
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    //
    proximity_start();
    calibrate_ir();
    // Initiate the motors
    motors_init();
    // Initiate the distance sensor module
    VL53L0X_start();
    // Initiate the USB communication peripheral
    usb_start();
    // Initialise the UART1 channel
    serial_start();

    /* Infinite loop. */
    while (1) {
    	// LED setting
    	//set_led(LED1, 2);
    	//set_rgb_led(LED2, 5, 5, 5);
    	set_body_led(2);
    	//waits 1 second
    	chThdSleepMilliseconds(1000);

    	// Motor setting
    	left_motor_set_speed(-420);
    	right_motor_set_speed(420);
    	//waits 1 second
        chThdSleepMilliseconds(1000);

        // Read proximity value
        for (int i = 0; i <= 7; i++){
        	double proximity_value = get_calibrated_prox(i);
        	if (SDU1.config->usbp->state==USB_ACTIVE) {
        		chprintf((BaseSequentialStream*)&SDU1, "sensor %d: %2d\n", i, proximity_value);
        	}
        }
        chprintf((BaseSequentialStream*)&SDU1, "\n");
        //waits 1 second
        chThdSleepMilliseconds(1000);

//        // Read the distance
//        double dist = VL53L0X_get_dist_mm();
//        // Skip printing if port not opened.
//
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
