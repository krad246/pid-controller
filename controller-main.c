#include "controller-main.h"

/**
 * Forked thread initializes drivers and all peripherals, then
 * initializes worker threads to perform the various functions.
 */

void *collectData(void *arg);

void init_controller(uint_least8_t index, struct controller_args *ctl_args);
void init_sensor(uint_least8_t index, uint8_t mode, void *args);

void zero_crossing_lineup(uint_least8_t index);

unsigned int pid_scale = HALF_CYCLE_COUNT * CYCLE_RES;
unsigned int currentDuty = 0;

extern int setPoint;

void *mainThread(void *arg0)
{

    // Set up timer used to keep PWM in sync with wall
    init_timer();

    // Sets up GPIO
    // Specifically attaches zero_crossing_lineup to GPIO_22
    init_gpio();

    //sleep to allow the LCD to turn on if plugged directly in
    usleep(400000);
    // Sets up hardware I2C
    init_i2c();

    // Sets up hardware PWM
    init_pwm();

    // Set up SPI
    init_spi();


    // Sets up 1 SPI sensor and 1 controller for that sensor
    // There are 2 PWM pins
    uint8_t pwm_count = 2;

    // Allocate the array for them
    ctls = malloc(pwm_count * sizeof(struct controller));
    sens_list = malloc(pwm_count * sizeof(struct sensor));

    // Set the desired arguments for sample frequency, etc
    struct controller_args args;
    set_sample_time(&args, 1);
    set_tuning_period(&args, 0xFFFFFFFF);
    set_limits(&args, 0, PID_RANGE);
    set_coefficients(&args, 0.25, 0.125, 0.0625);

    // Set up the controller at index 0 of the table
    init_controller(0, &args);
    set_sp(&ctls[0], 25);

    // Set up the sensor at index 0 of the table
    // Set up with alpha = 0.375
    float alpha = 0.375;
    init_sensor(0, EXPONENTIAL, &alpha);

    // Initialize collectData thread
//    pthread_t *thread = malloc(sizeof(pthread_t));
//    pthread_attr_t attrs;
//    pthread_attr_init(&attrs);
//    pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
//    pthread_create(thread, &attrs, collectData, NULL);

//    PWM_start(switchers[0]);

//    int pid_scale_temp = 0;
//    int up = 1;
//    PWM_start(switchers[0]);
    PWM_start(switchers[0]);
    char tempData[20];
    char tempData2[20];
    //PWM_setDuty(switchers[0], pid_scale * 4);
    while (1) {
//        if (up == 1) {
//            pid_scale_temp++;
//            if (pid_scale_temp == 8) {
//                up = 0;
//            }
//        } else {
//            pid_scale_temp--;
//            if (pid_scale_temp == 0) {
//                up = 1;
//            }
//        }
        sensor_read(&sens_list[0]);
        volatile int32_t temp = sensor_get_avg(&sens_list[0]);
        fp32_t data = sensor_get_data(&sens_list[0]);
        int32_t out = control(&ctls[0], data);
        currentDuty = pid_scale * out;
        PWM_setDuty(switchers[0], currentDuty);
        clear();
        setCursor(0,0);
        sprintf(tempData,"Temperature: %.2f C", data / (float) (SCALE32_FACTOR));
        writeToLCD(tempData);
        setCursor(1, 0);
        sprintf(tempData2,"Set Point: %d C", setPoint);
        writeToLCD(tempData2);

        usleep(250000);
    }
    I2C_close(i2c_master);
	return NULL;
}

// Periodically running function that collects temp data
// Temperature data is filtered
// Will be replaced with SPI
void *collectData(void *arg) {
    for (;;) {
        sensor_read(&sens_list[0]);
        printf("temp: %d\n", sensor_get_avg(&sens_list[0]));
    }
}

// Stops and resets the PWM to sync it with the zero crossing
void zero_crossing_lineup(uint_least8_t index){
    PWM_setDuty(switchers[0], 0);
    PWM_setPeriod(switchers[0], 0);
    PWM_stop(switchers[0]);
    // reset_pwm(currentDuty, 0);s
    PWM_start(switchers[0]);
    PWM_setDuty(switchers[0], currentDuty);
    GPIO_disableInt(ZERO_CROSS);
}

void init_controller(uint_least8_t index, struct controller_args *ctl_args) {
    // Initializes controller with parameters
    controller_init(&ctls[index], ctl_args);
}

void init_sensor(uint_least8_t index, uint8_t mode, void *args) {
    sensor_init(&sens_list[index], mode, args);
}
