#ifndef APPLICATION_CONFIG_H
#define APPLICATION_CONFIG_H

#include "nrf_uarte.h"

#define MAC_ADDR_LEN    6
#define B_MAC_CNT       6
#define APP_BEACON_INFO_LENGTH          0x17                               /**< Total length of information advertised by the Beacon. */

typedef struct
{
	uint8_t addr[MAC_ADDR_LEN];
	uint8_t empty;
	uint8_t handle;
}b_addr_t;

typedef struct
{
	uint16_t 	file_id;           						//!< The ID of the file that the record belongs to.
    uint16_t 	key;               						//!< The record key.
	
	uint8_t  	role;									//!< The role of the device
	uint8_t  	baud;									//!< Baud rate
	uint8_t 	addr_type;								//!< GAP Address types
	uint8_t   	adv_mode;								//!< Advertising Model. */

	uint8_t 	filt_type;
	uint8_t 	scan_state;						 		//!< Scan status.*/
	uint16_t 	company_id;								//!< Company identifier code. */
	
	uint16_t    uuid16;									//!< 16-bit UUID value or octets 12-13 of 128-bit UUID.*/
	uint16_t    rx_uuid;								//!< Characteristic UUID (16 bits UUIDs).*/
	uint16_t    tx_uuid;								//!< Characteristic UUID (16 bits UUIDs). */
	uint16_t    scan_interval;               			//!< Scan interval in 625 us units. @sa BLE_GAP_SCAN_INTERVALS. */
	uint16_t    scan_window;                      		//!< Scan window in 625 us units. @sa BLE_GAP_SCAN_WINDOW. */
	uint16_t    scan_timeout;                    		//!< Scan timeout in 10 ms units. @sa BLE_GAP_SCAN_TIMEOUT. */
	uint16_t 	filt_companyid;
	uint16_t 	filt_uuid_16;

    uint32_t   	adv_interval;							//!< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
	uint32_t   	adv_timeout;							//!< The advertising duration (180 seconds) in units of 10 milliseconds. */
    uint32_t	min_conn_interval;      				//!< Connection interval uses 1.25 ms units. 7.5 ms-4s */
    uint32_t 	max_conn_interval;      				//!< Connection interval uses 1.25 ms units. 7.5 ms-4s */
    uint32_t 	conn_timeout;							//!< Supervision Timeout uses 10 ms units.	 100ms -32s */

    uint8_t 	public_addr[MAC_ADDR_LEN];				//!< Public (identity) address.	
    uint8_t 	static_addr[MAC_ADDR_LEN];				//!< Random static (identity) address.

    uint8_t 	uuid128[16];							//!< Little-Endian UUID bytes. */

	uint8_t    	device_name[30];						//!< device name. */
	uint8_t 	filt_name[30];

	uint8_t 	filt_mac[6];
	uint8_t  	keys[6];

	uint8_t  	advdata_manuf_data[21];					//!< Additional manufacturer specific data.
	uint8_t  	m_beacon_info[APP_BEACON_INFO_LENGTH];
	
	b_addr_t  	b_addr[4];								//!< Binding address

	int8_t 	   	adv_tx_power;							//!< tx_power Radio transmit power in dBm. */
	
}config_t;

#endif
