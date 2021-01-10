#include "Arduino.h"
#include "json_lib.h"

#ifndef PACKER_H
#define PACKER_H

class MessagePacker
{
private:
    /* data */
public:
    DynamicJsonDocument pack(DynamicJsonDocument sensor_data);
    DynamicJsonDocument parser(DynamicJsonDocument callback);
};



#endif