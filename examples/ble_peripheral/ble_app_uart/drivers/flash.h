#ifndef _FLASH_H
#define _FLASH_H

#include "fds.h"

#define RESTORE_FILE		 (0x0100)
#define RESTORE_REC_KEY      (0x0100)

#define ROLE_FILE		  (0x0200)
#define ROLE_REC_KEY      (0x0100)

#define PRODUCTION_FILE		 (0x0200)
#define PRODUCTION_REC_KEY   (0x0100)

#define UART_CONFIG_FILE     (0x0300)
#define UART_CONFIG_REC_KEY  (0x0100)

#define MAC_FILE     (0x0400)
#define MAC_REC_KEY  (0x0100)

#define ADV_CONFIG_FILE     (0x0600)
#define ADV_CONFIG_REC_KEY  (0x0100)

#define ADVDATA_FILE        (0x0700)
#define ADVDATA_REC_KEY     (0x0100)

#define IBEACON_FILE        (0x0800)
#define IBEACON_REC_KEY     (0x0100)

#define SCAN_RSP_FILE       (0x0900)
#define SCAN_RSP_REC_KEY    (0x0100)

#define SCAN_CONFIG_FILE    (0x0A00)
#define SCAN_CONFIG_REC_KEY  (0x0100)

#define SCAN_FILT_FILE     (0x0B00)
#define SCAN_FILT_REC_KEY  (0x0100)

#define SERVICE_CONFIG_FILE     (0x0C00)
#define SERVICE_CONFIG_REC_KEY  (0x0100)

#define CONN_CONFIG_FILE     (0x0D00)
#define CONN_CONFIG_REC_KEY  (0x0100)

#define PEER_MAC_CONFIG_FILE     (0x0E00)
#define PEER_MAC_REC_KEY         (0x0100)

#define PATTERN_CONFIG_FILE     (0x0F00)
#define PATTERN_CONFIG_REC_KEY  (0x0100)

#define OTHER_CONFIG_FILE     (0x0100)
#define OTHER_CONFIG_REC_KEY  (0x0200)

#define B_MAC_FILE     (0x0200)
#define B_MAC_REC_KEY  (0x0200)

#define UUID_FILE     (0x0300)
#define UUID_KEY  	  (0x0200)

#define PASSWORD_FILE     (0x0400)
#define PASSWORD_KEY  	  (0x0200)

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
