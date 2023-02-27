#ifndef _FLASH_H
#define _FLASH_H

#include "fds.h"

#define FDS_READ            	0x00
#define FDS_WRITE				0x01

#define TEMPERATURE_FLASH_KEY   0x00B0

#define FDS_BUFFER_SIZE			512

/*********************************************************************
 * API FUNCTIONS
 */
bool Fds_FlashInit(void);
void Fds_FlashContrl(uint16_t fileId,uint16_t keyId, uint8_t readWriteFlag, uint8_t *pData, uint32_t dataLen);
ret_code_t flash_read(uint16_t fileId, uint16_t key, uint8_t *pData, uint8_t dataLen);
bool flash_write(uint16_t fileId, uint16_t key, uint8_t *pData, uint32_t dataLen);
bool flash_fds_init(void);


#endif
