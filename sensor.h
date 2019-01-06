#ifndef SENSOR_H
#define SENSOR_H
#define FP16_NULL 32
#include "filter.h"
#include "spi.h"

// Abstraction of SPI temperature sensor
struct sensor {
    uint8_t mode;                                                                       // Filtering mode in use
    void *filter_strat;                                                                 // Filter struct for corresponding mode
    void (*filter_impl)(void *self, fp32_t input);                                      // Filter equation for corresponding mode
};

struct sensor *sens_list;

void filter(struct sensor *sens, fp32_t input);                                        // Function to extract filter equation and struct                                                                   // And then perform the filtering strategy

// Constructor
void sensor_init(struct sensor *sens, uint8_t mode, void *arg);

// Destructor
void sensor_destroy(struct sensor *sens);

// Reads the pin and updates the average
void sensor_read(struct sensor *sens);

// Get() for internal data
fp32_t sensor_get_data(struct sensor *sens);

// Get() for the internal average
int32_t sensor_get_avg(struct sensor *sens);

#endif
