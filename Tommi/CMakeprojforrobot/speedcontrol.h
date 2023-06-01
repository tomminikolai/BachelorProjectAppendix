#pragma once


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
/// <summary>
/// Class for controlling motor control software on arduinos over UART
/// </summary>
class mcontrol
{
private:
	/// <summary>
	/// serial port (/dev/ttyUSBX)
	/// </summary>
	int serial_port;
	struct termios tty;
	bool m_open;
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="devID">path to serial device(/dev/ttyUSBX)</param>
	/// <param name="err">pointer to error variable</param>
	mcontrol(const char* devID, int* err);
	~mcontrol();
	/// <summary>
	/// closes the serial port
	/// </summary>
	/// <param name=""></param>
	void disconnect(void);
	/// <summary>
	/// Sends new speed to motor control
	/// </summary>
	/// <param name="speed">Wanted speed</param>
	void setspeed(int speed);
};