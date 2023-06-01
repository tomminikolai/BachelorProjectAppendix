#include "pose_monitor.h"

pose_monitor::pose_monitor(void)
{
    pthread_mutex_init(&pose_mutex, NULL);
    for (int i = 0; i < 4; i++)
    {
        pose[i] = 0;
    }
}

void pose_monitor::set(float newpose[4])
{
    pthread_mutex_lock(&pose_mutex);
    for (int i = 0; i < 4; i++)
    {
        pose[i] = newpose[i];
    }
    pthread_mutex_unlock(&pose_mutex);
}

void pose_monitor::get(float* ref)
{
    pthread_mutex_lock(&pose_mutex);
    for (int i = 0; i < 4; i++)
    {
        ref[i] = pose[i];
    }
    pthread_mutex_unlock(&pose_mutex);
}