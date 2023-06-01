///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2022, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "sock.h"
#include "pose_monitor.h"
#include "speed_monitor.h"
#include "robotpos.h"
#include "speedcontrol.h"

using namespace std;

pose_monitor posmon;
speed_monitor speedmon;

pthread_t threadsock, posthread, mcontrolthread, speedthread;

bool issafe = true;

void* sockserver(void* unused)
{
    sock sock1(9999);
    float sendpos[4];
    float opos[4] = { 0, 0, 0, 0 };
    while (true)
    {
        posmon.get(&sendpos[0]);
        if (sendpos[0] != opos[0] || sendpos[1] != opos[1] || sendpos[2] != opos[2] || sendpos[3] != opos[3])
        {
            sock1.send(&sendpos[0], 4 * sizeof(float));
            for (int i = 0; i < 4; i++)
            {
                opos[i] = sendpos[i];
            }
        }
    }
};

void* posservice(void* unused)
{
    float pose[4];
    int err;
    robotpos robpos(&err, &issafe);
    if (err)
    {
        cout << "error starting position service!" << endl;
    }
    else
    {
        while (true)
        {
            robpos.updatepos(&pose[0]);
            sleep(0.1);
            posmon.set(pose);
        }
    }
}

void* speedservice(void* unused)
{
    sock sock2(1234);
    unsigned char speedin;
    bool running = true;
    while (running)
    {
        //cout << "enter speed: ";
        sock2.recv(&speedin, 1);
        speedmon.set(speedin);
        if (speedin == 0x05)
        {
            running = false;
        }
    }
}

void* nav_to_point(void* unused)
{
    sock sock2(1234);
    unsigned char speedin = 0x00;
    unsigned char speedino = 0x00;
    float pose[2] = { 0,0};
    float localpos[4] = { 0,0,0,0 };
    bool running = true;
    while (running)
    {
        //cout << "enter speed: ";
        sock2.recv(&pose, 2 * sizeof(float));
        printf("%.2f, %.2f\n", pose[0], pose[1]);
        float dx = pose[0] - localpos[1];
        float dy = pose[1] - localpos[3];
        posmon.get(localpos);
        while ((dx * dx + dy * dy) > (0.1*0.1))
        {
            float dx = pose[0] - localpos[1];
            float dy = pose[1] - localpos[3];
            float theta = atan(dx / dy);
            int mult = theta / (2 * 3.1415);
            if (mult)
            {
                theta -= mult * 2 * 3.1415;
            }
            if (theta > 3.1415)
            {
                theta -= 2 * 3.1415;
            }
            else if (theta < -3.1415)
            {
                theta += 2 * 3.1415;
            }
            if (abs(theta - localpos[0]) > 0.09)
            {
                while (!(localpos[0] < theta + 0.06 && localpos[0] > theta - 0.06))
                {
                    dx = pose[0] - localpos[1];
                    dy = pose[1] - localpos[3];
                    theta = atan(dx / dy);
                    mult = theta / (2 * 3.1415);
                    if (mult)
                    {
                        theta -= mult * 2 * 3.1415;
                    }
                    if (theta > 3.1415)
                    {
                        theta -= 2 * 3.1415;
                    }
                    else if (theta < -3.1415)
                    {
                        theta += 2 * 3.1415;
                    }
                    if (theta - localpos[0] < 0)
                    {
                        speedin = 0x04;
                    }
                    else
                    {
                        speedin = 0x03;
                    }
                    if (speedin != speedino)
                    {
                        speedmon.set(speedin);
                        speedino = speedin;
                    }
                    //printf("Turning left! %.2f radians to go\n", theta - localpos[0]);
                    posmon.get(localpos);
                }
                speedmon.set(0x00);
            }
            speedmon.set(0x01);
            posmon.get(localpos);
        }
        speedmon.set(0x05);
        running = false;
    }
}

void* steering(void* unused)
{
    int err1;
    int err2;
    mcontrol m1("/dev/ttyUSB0", &err1);
    if (err1 == 1)
    {
        printf("Failed");
    }
    else
    {
        mcontrol m2("/dev/ttyUSB1", &err2);
        if (err2 == 1)
        {
            printf("Failed");
        }
        else
        {
            unsigned char speedin;
            unsigned char speedino = 0;
            bool running = true;
            while (running)
            {
                //cout << "enter speed: ";
                speedmon.get(&speedin);
                if (speedin == 0x01 && !issafe)
                {
                    speedin = 0x00;
                }
                if (speedin != speedino)
                {
                    switch (speedin)
                    {
                    case 0x00:
                        m1.setspeed(0);
                        m2.setspeed(0);
                        break;
                    case 0x01:
                        if (issafe)
                        {
                            m1.setspeed(30);
                            m2.setspeed(30);
                        }
                        else
                        {
                            m1.setspeed(0);
                            m2.setspeed(0);
                        }
                        break;
                    case 0x02:
                        m1.setspeed(-30);
                        m2.setspeed(-30);
                        break;
                    case 0x03:
                        m1.setspeed(-15);
                        m2.setspeed(15);
                        break;
                    case 0x04:
                        m1.setspeed(15);
                        m2.setspeed(-15);
                        break;
                    case 0x05:
                        m1.setspeed(0);
                        m2.setspeed(0);
                        running = false;
                        break;
                    default:
                        break;
                    }
                }
                speedino = speedin;
                sleep(0.1);
            }
        }
        m2.disconnect();
    }
    m1.disconnect();
}

int main(int argc, char **argv) 
{
    if (argc == 2)
    {
        if (argv[1][0] == '-')
        {
            if (argv[1][1] == 'm')
            {
                printf("Manual drive mode!\n");
                pthread_create(&speedthread, NULL, &speedservice, NULL);
                pthread_create(&mcontrolthread, NULL, &steering, NULL);
            }
            else if (argv[1][1] == 'a')
            {
                printf("Automatic drive mode!\n");
                pthread_create(&speedthread, NULL, &nav_to_point, NULL);
                pthread_create(&mcontrolthread, NULL, &steering, NULL);
            }
            
        }
    }
    pthread_create(&threadsock, NULL, &sockserver, NULL);
    pthread_create(&posthread, NULL, &posservice, NULL);
    if (argc == 2)
    {
        if (argv[1][0] == '-')
        {
            if (argv[1][1] == 'm')
            {
                pthread_join(speedthread, NULL);
                pthread_join(mcontrolthread, NULL);
            }
            else if (argv[1][1] == 'a')
            {
                pthread_join(speedthread, NULL);
                pthread_join(mcontrolthread, NULL);
            }

        }
    }
    pthread_join(threadsock, NULL);
    pthread_join(posthread, NULL);
    return EXIT_SUCCESS;
}