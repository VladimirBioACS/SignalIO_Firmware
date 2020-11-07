#ifndef SIG_H
#define SIG_H

    void pin_init(int port_two);
    void signal_led(int time);
    void restart(int time);
    void sys_info(void);
    float battery_check(void);
    void resetWiFi(void);

#endif