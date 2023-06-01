#include "speedcontrol.h"

mcontrol::mcontrol(const char* devID, int* err)
{
	m_open = false;
	*err = 0;
	serial_port = open(devID, O_RDWR);
	if (serial_port != -1)
	{
		m_open = true;
	}
	printf("%d\n", serial_port);
	if (tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		*err = 1;
		disconnect();
		return;
	}
	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag	|= CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR;

	tty.c_cc[VTIME] = 2;
	tty.c_cc[VMIN] = 0;

	cfsetispeed(&tty, B19200);
	cfsetospeed(&tty, B19200);
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		*err = 1;
		disconnect();
		return;
	}
	unsigned int reset = 0xFFFF;
	write(serial_port, &reset, 2);
}

mcontrol::~mcontrol()
{
	disconnect();
}

void mcontrol::disconnect(void)
{
	if (serial_port != -1)
	{
		m_open = false;
		close(serial_port);
	}
}

void mcontrol::setspeed(int speed)
{
	if (!m_open)
	{
		return;
	}
	char msg1 = (speed & 0xFF);
	char msg2 = (speed >> 8);
	write(serial_port, &msg1, sizeof(unsigned char));
	write(serial_port, &msg2, sizeof(unsigned char));
}
