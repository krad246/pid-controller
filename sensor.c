#include "sensor.h"

// Generalized function to allow sensor to use any filtering strategy
void filter(struct sensor *sens, fp32_t input) {
    // Extract the internal filter object
    void *filter_strat = sens->filter_strat;

    // Check the mode it is in
    switch (sens->mode) {
    case WINDOW:
    {
        // Convert the filter object and function to moving average mode
        mv_avg filter_impl = (mv_avg) sens->filter_impl;
        filter_strat = (struct moving_average *) filter_strat;

        // Compute and return the result of the moving average computation
        filter_impl(filter_strat, input);
        break;
    }
    case EXPONENTIAL:
    {
        // Convert the filter object and function to exponential average mode
        ema_avg filter_impl = (ema_avg) sens->filter_impl;
        filter_strat = (struct exp_average *) filter_strat;

        // Compute and return the result of the exponential average computation
        filter_impl(filter_strat, input);
        break;
    }
    default:
        while (1);
    }
}

// Constructor for sensor
void sensor_init(struct sensor *sens, uint8_t mode, void *arg) {
    void *filter_strat = NULL;
    filter_func filter_impl = NULL;

    // Check the input mode
    switch (mode) {
    case WINDOW:
    {
        // If the mode is moving average, allocate and set up an averaging filter
        // First allocate the struct
        filter_strat = (struct moving_average *) malloc(sizeof(struct moving_average));

        // Extract the number of taps from the input argument and perform type casting
        uint8_t taps = *((uint8_t *) arg);

        // Initialize the struct with the tap count and struct
        init_avg_filter(filter_strat, taps);

        // Set the function to moving average
        filter_impl = (filter_func) avg_filter;
        break;
    }
    case EXPONENTIAL:
    {
        // If the mode is exponential moving average, allocate and set up an EMA filter
        // First allocate the EMA filter
        filter_strat = (struct exp_average *) malloc(sizeof(struct exp_average));

        // Extract the alpha coefficient from the argument
        float alpha = *((float *) arg);

        // Initialize the struct with the alpha
        init_ema_filter(filter_strat, alpha);

        // Set the function to EMA
        filter_impl = (filter_func) ema_filter;
        break;
    }

    // In the event that the input mode is wrong, throw an error
    default:
        printf("Invalid sensor configuration.\n");
        while (1);
    }

    // If initialization was unsuccessful, throw an error
    if (filter_strat == NULL || filter_impl == NULL) {
        printf("Error initializing SPI sensor.\n");
        while (1);
    }

    // Otherwise finish initializing the sensor with the data
    // Set the filter mode, object, and function for the sensor
    sens->mode = mode;
    sens->filter_strat = filter_strat;
    sens->filter_impl = filter_impl;
}

// Destructor for sensor
void sensor_destroy(struct sensor *sens) {
    if (sens == NULL) {
        printf("Attempting to free on NULL.\n");
        while (1);
    }

    // Check the mode
    switch (sens->mode) {
        case WINDOW:
        {
            // If it is in moving average mode, run the moving average destructor
            struct moving_average *filter_strat = (struct moving_average *) sens->filter_strat;
            destroy_moving_avg(filter_strat);
            break;
        }
    case EXPONENTIAL:
        {
            // If it is in exponential moving average mode, run the exponential average destructor
            struct exp_average *filter_strat = (struct exp_average *) sens->filter_strat;
            destroy_exp_avg(filter_strat);
            break;
        }
    }

    // Clean up the sensor fully
    free(sens);
}

// Performs a read and filters the data, updating the average
void sensor_read(struct sensor *sens) {
    if (sens->filter_strat == NULL || sens->filter_impl == NULL) {
        printf("Sensor incorrectly configured.\n");
        while (1);
    }

    // Reads SPI sensor for temperature and performs the update
    fp32_t input = read_spi();
    filter(sens, input);
}

// Performs a read on the sensor and updates the average
fp32_t sensor_get_data(struct sensor *sens) {
    // Check the mode
    switch (sens->mode) {
    case WINDOW:
    {
        // If it is in moving average mode, run Get() on the window filter inside
        struct moving_average *filter_strat = (struct moving_average *) sens->filter_strat;
        return filter_strat->curr_avg;
    }
    case EXPONENTIAL:
    {
        // If it is in exponential moving average mode, run Get() on the EMA filter inside
        struct exp_average *filter_strat = (struct exp_average *) sens->filter_strat;
        return filter_strat->curr_avg;
    }
    default:
        // If neither check went through, the function failed
        #define FP32_NULL -(1 << 31);
        return FP16_NULL;
    }
}

// Performs a read on the sensor and updates the average
int32_t sensor_get_avg(struct sensor *sens) {
    // Check the mode
    switch (sens->mode) {
    case WINDOW:
    {
        // If it is in moving average mode, run Get() on the window filter inside
        struct moving_average *filter_strat = (struct moving_average *) sens->filter_strat;
        int32_t avg = get_mv_avg(filter_strat);
        return avg;
    }
    case EXPONENTIAL:
    {
        // If it is in exponential moving average mode, run Get() on the EMA filter inside
        struct exp_average *filter_strat = (struct exp_average *) sens->filter_strat;
        int32_t avg = get_ema_avg(filter_strat);
        return avg;
    }
    default:
        // If neither check went through, the function failed
        #define FP32_NULL -(1 << 31);
        return FP32_NULL;
    }
}
