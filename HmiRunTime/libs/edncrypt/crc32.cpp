#include "crc32.h"

#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
  
static unsigned int crc_table[256];  
 
crc32::crc32()
{
	init_crc_table();
}

crc32::~crc32()
{

}

void crc32::init_crc_table(void)  
{  
    unsigned int c;  
    unsigned int i, j;  
      
    for (i = 0; i < 256; i++) {  
        c = (unsigned int)i;  
        for (j = 0; j < 8; j++) {  
            if (c & 1)  
                c = 0xedb88320L ^ (c >> 1);  
            else  
                c = c >> 1;  
        }  
        crc_table[i] = c;  
    }  
}  
 
unsigned int crc32::CRC(unsigned char *buffer, unsigned int size)  
{  
	unsigned int crc = 0xffffffff;
    unsigned int i;  
    for (i = 0; i < size; i++) {  
        crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);  
    }  
    return crc ;  
}  
  
