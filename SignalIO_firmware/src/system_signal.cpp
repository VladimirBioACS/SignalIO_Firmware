#include <Arduino.h>
#include <system_signal.h>


void signal_led(int time){
  digitalWrite(13, LOW);
  delay(time);
  digitalWrite(13, HIGH);
}
