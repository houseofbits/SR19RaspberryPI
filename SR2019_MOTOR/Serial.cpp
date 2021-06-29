#include "Core.h"


Serial::Serial() : fd(-1) {

}

Serial::~Serial()
{
	sleep(2);
	tcflush(fd, TCIOFLUSH);
	::close(fd);
}

int Serial::open() {

	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	fd = ::open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (fd == -1)
	{
		cout << "Error - Unable to open UART.  Ensure it is not in use by another application" << endl;
		return -1;
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	//115200
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag = B115200 | CS8 | CLOCAL;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	return fd;
}

void Serial::write(unsigned char byte) {

	if (fd != -1) {
		::write(fd, &byte, 1);
	}
}

void Serial::write(unsigned char *bytes, int size) {

	if (fd != -1) {
		::write(fd, &bytes[0], size);
	}
}

void Serial::write(string str)
{
	if (fd != -1) {
		::write(fd, (char*)str.c_str(), str.length());
	}
}

#define DATA_SIZE 252

void Serial::writeKeyFrame(WingsKeyframe& k) {

	for (unsigned int a = 0; a < 12; a++)
	{
		for (unsigned int i = 0; i < 7; i++)
		{
			lightDataOut.wingData[a].colors[i].r = (unsigned char)(k.colors[a][i].x * 255.0f);
			lightDataOut.wingData[a].colors[i].g = (unsigned char)(k.colors[a][i].y * 255.0f);
			lightDataOut.wingData[a].colors[i].b = (unsigned char)(k.colors[a][i].z * 255.0f);
		}
	}

	outputBuffer[0] = (unsigned char)83;
	outputBuffer[1] = (unsigned char)82;
	outputBuffer[2] = (unsigned char)49;
	outputBuffer[3] = (unsigned char)57;

	unsigned char* dta = (unsigned char*)&lightDataOut.wingData;

	write(outputBuffer, 4);

	write(dta, DATA_SIZE);

	outputBuffer[0] = Serial::CRC8(dta, DATA_SIZE);

	write(outputBuffer, 1);
}

void Serial::writeMotorSettings(bool on, bool direction, int speed)
{
	outputBuffer[0] = (unsigned char)'M';
	outputBuffer[1] = (unsigned char)'O';
	outputBuffer[2] = (unsigned char)'T';
	outputBuffer[3] = (unsigned char)'O';

	write(outputBuffer, 4);

	/*
	outputBuffer[0] = (on) ? 1 : 0;
	outputBuffer[1] = (direction) ? 1 : 0;
	outputBuffer[2] = (unsigned char)speed;
	outputBuffer[3] = Serial::CRC8(outputBuffer, 3);
	write(outputBuffer, 4);
	*/

	outputBuffer[0] = (on) ? 1 : 0;
	outputBuffer[1] = (direction) ? 1 : 0;
	outputBuffer[2] = (unsigned char)speed;
	outputBuffer[3] = 'S';
	outputBuffer[4] = 'T';
	outputBuffer[5] = 'P';
	outputBuffer[6] = 'R';
	outputBuffer[7] = Serial::CRC8(outputBuffer, 7);
	write(outputBuffer, 8);

}