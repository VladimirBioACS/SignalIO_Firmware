#include "Arduino.h"

#ifndef SENSORS_H
#define SENSORS_H

#define DHTTYPE DHT11

#define DHT11 0xC7
#define DIGITAL_SENSOR 0xC6
#define ANALOG_SENSOR 0xC5
#define RELAY 0xC4

class sensors
{
private:
    const int calibration_time = 25;
    float hum;
    float temp;

public:
    sensors(/* args */);
    ~sensors();

    //dht11 sensor
    void dht11_init(void);
    float read_hum(void);
    float read_temp(void);

    //simple sensor interface
    void sensor_init(void);
    int digital_sensor_read(void);
    float analog_sensor_read(void);
};

sensors::sensors(/* args */)
{
}

sensors::~sensors()
{
}

#endif