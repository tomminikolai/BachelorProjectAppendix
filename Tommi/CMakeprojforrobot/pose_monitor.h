#pragma once
#include <pthread.h>
/// <summary>
/// Monitor class for the posevariable of the robot
/// </summary>
class pose_monitor
{
private:
    pthread_mutex_t pose_mutex;
    float pose[4];
public:
    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name=""></param>
    pose_monitor(void);
    /// <summary>
    /// Sets the pose
    /// </summary>
    /// <param name="newpose">The 4 floats describing the new pose</param>
    void set(float newpose[4]);
    /// <summary>
    /// Sets the 4 floats at the pointer to be equal to the pose
    /// </summary>
    /// <param name="ref">Pointer to the pose variable you want to copy the pose to</param>
    void get(float* ref);
};