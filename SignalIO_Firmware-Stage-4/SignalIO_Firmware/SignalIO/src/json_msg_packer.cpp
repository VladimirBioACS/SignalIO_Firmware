#include "json_msg_packer.h"

DynamicJsonDocument MessagePacker::pack(DynamicJsonDocument sensor_data){
    DynamicJsonDocument req(255);
    return req;
}

DynamicJsonDocument MessagePacker::parser(DynamicJsonDocument callback){
    DynamicJsonDocument call(255);
    return call;
}