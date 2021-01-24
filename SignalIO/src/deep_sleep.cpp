#include "deep_sleep.h"


void Sleep::pin_trigger_sleep(){
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,0); 
    esp_deep_sleep_start();
}

void Sleep::tmr_sleep(unsigned int sleep_time){
    esp_sleep_enable_timer_wakeup(sleep_time * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}