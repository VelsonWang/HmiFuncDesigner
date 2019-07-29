#ifndef CRC32_H
#define CRC32_H

#include <QObject>

class crc32
{

public:
	crc32();
	~crc32();
	unsigned int CRC(unsigned char * buffer, unsigned int size);
private:
	void init_crc_table(void);
};

#endif // CRC32_H
