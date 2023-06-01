#include "speed_monitor.h"

speed_monitor::speed_monitor(void)
    :speed(0)
{
    pthread_mutex_init(&speed_mutex, NULL);
}

void speed_monitor::set(unsigned char newspeed)
{
    pthread_mutex_lock(&speed_mutex);
    speed = newspeed;
    pthread_mutex_unlock(&speed_mutex);
}

void speed_monitor::get(unsigned char* ref)
{
    pthread_mutex_lock(&speed_mutex);
    *ref = speed;
    pthread_mutex_unlock(&speed_mutex);
}