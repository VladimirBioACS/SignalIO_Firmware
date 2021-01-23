#include "Arduino.h"
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"


#ifndef SENSORS_H
#define SENSORS_H

#define DHTTYPE DHT11

#define DHT11_SENSOR 0xC7
#define DIGITAL_SENSOR 0xC6
#define ANALOG_SENSOR 0xC5
#define RELAY 0xC4
#define PIR_SENSOR 0xC3

class sensors
{
private:
    const int calibration_time = 20;
    float hum;
    float temp;

public:

    uint8_t module_pin;

    //dht11 sensor
    void dht11_init(void);
    float read_hum(void);
    float read_temp(void);

    //simple sensor interface
    void sensor_init(void);
    int digital_sensor_read(void);
    int analog_sensor_read(void);
    void relay_init(void);
};

#endif