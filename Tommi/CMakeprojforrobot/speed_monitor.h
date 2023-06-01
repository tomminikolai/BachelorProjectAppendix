#pragma once
#include <pthread.h>
/// <summary>
/// Monitor class for the speedvariable of the robot
/// </summary>
class speed_monitor
{
private:
    pthread_mutex_t speed_mutex;
    unsigned char speed;
public:
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name=""></param>
    speed_monitor(void);
    /// <summary>
    /// Sets the speed
    /// </summary>
    /// <param name="newspeed">The 4 floats describing the new speed</param>
    void set(unsigned char newspeed);
    /// <summary>
    /// Sets the 4 floats at the pointer to be equal to the speed
    /// </summary>
    /// <param name="ref">Pointer to the speed variable you want to copy the speed to</param>
    void get(unsigned char* ref);
};