#pragma once

class Serial {
public:

	Serial();
	~Serial();

	int open();

	void write(unsigned char byte);

	void write(unsigned char *bytes, int size);

	void write(string str);

	void writeKeyFrame(WingsKeyframe&);

	void writeMotorSettings(bool on, bool direction, int speed);

	void wait() {
		tcdrain(fd);
	}

	int fd;

	unsigned char	outputBuffer[1024];
	unsigned int	size;

	LightDataOutStructure	lightDataOut;

	//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
	//code released under the therms of the GNU GPL 3.0 license
	static uint8_t CRC8(const uint8_t *data, uint8_t len) {
		uint8_t crc = 0x00;
		while (len--) {
			uint8_t extract = *data++;
			for (uint8_t tempI = 8; tempI; tempI--) {
				uint8_t sum = (crc ^ extract) & 0x01;
				crc = uint8_t(crc >> 1);
				if (sum) {
					crc ^= 0x8C;
				}
				extract = uint8_t(extract >> 1);
			}
		}
		return crc;
	}
};
