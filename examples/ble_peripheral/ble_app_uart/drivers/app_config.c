#include "app_config.h"
#include "flash.h"

#define RESTORE_FILE    	0x0001
#define RESTORE_REC_KEY   	0x0001
#define CONFIG_FLASH_ID    	0x0010
#define CONFIG_FLASH_KEY   	0x0010

static restore_t restore =
{
    .file_id = RESTORE_FILE,
    .key     = RESTORE_REC_KEY,
    .flag	 = true,
};

static config_t config =
{
	.file_id 			= CONFIG_FLASH_ID,
	.key	 			= CONFIG_FLASH_KEY,
	.role	 			= 0,
	.baud	 			= 13,
	.addr_type			= 1,
	.adv_mode			= 1,
	.filt_type			= 0,
	.scan_state 		= 1,
	.company_id			= 0x2023,
	.uuid16				= 0x0001,
	.rx_uuid			= 0x0002,
	.tx_uuid			= 0x0003,
	.scan_interval		= 160,
	.scan_window		= 80,
	.scan_timeout		= 0,
	.filt_companyid		= 0x2023,
	.filt_uuid_16		= 0x0001,
	.adv_interval		= 64,
	.adv_timeout		= 18000,
	.min_conn_interval	= 20,
	.max_conn_interval	= 75,
	.conn_timeout		= 4000,
	.public_addr		= {0x00, 0x00, 0x01, 0x01, 0x23, 0x20},
	.static_addr		= {0x00, 0x01, 0x01, 0x23, 0x20, 0xD0},
	.uuid128			= {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x00, 0x00, 0x40, 0x6E},
	.device_name		= "beelinker",
	.filt_name			= "beelinker",
	.filt_mac			= {0x00, 0x00, 0x01, 0x01, 0x23, 0x20},
	.keys				= "111111",
	.advdata_manuf_data	= {0x20, 0x23, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	.m_beacon_info		= {0x02, 0x15, 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 0x01, 0x02 , 0x03, 0x04, 0xC3},
	.b_addr				= {{0x00, 0x00, 0x01, 0x01, 0x23, 0x20}},
	.adv_tx_power		= 0
};

/**@brief   Function for initializing the config Module.
 *
 * @details Get data from flash to fill the config.
 *
 */
void app_config_init(void)
{
    uint32_t err_code;
	//If flash is empty, write it first
	err_code = flash_read(restore.file_id, restore.key,(uint8_t *)&restore, sizeof(restore_t));
    if(err_code == FDS_ERR_NOT_FOUND)
    {
		flash_write(restore.file_id, restore.key,(uint8_t *)&restore, sizeof(restore_t));
		flash_write(config.file_id, config.key,(uint8_t *)&config, sizeof(config));	
	}
    else
    {
        if(restore.flag)
        {
			flash_read(config.file_id, config.key,(uint8_t *)&config, sizeof(config));
		}
		else
		{
			flash_write(config.file_id, config.key,(uint8_t *)&config, sizeof(config));
            restore.flag = 1;
            flash_write(restore.file_id, restore.key,(uint8_t *)&restore, sizeof(restore_t));			
		}
	}
}

restore_t* get_restore(void)
{
	return &restore;
}

config_t* get_config(void)
{
	return &config;
}

/**
 * @}
 */

