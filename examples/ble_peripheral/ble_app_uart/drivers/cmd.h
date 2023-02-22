#ifndef _CMD_PARSER_H
#define _CMD_PARSER_H
#include "nrf_error.h"
#include <stdint.h>
#include <stdbool.h>

#define		AT_BAUD_LENGTH			7
#define		AT_EXIT_LENGTH			7
#define		AT_RESET_LENGTH			8
#define		AT_RESTORE_LENGTH		10
#define		AT_M_INFO_LENGTH		9
#define		AT_ROLE_LENGTH			7
#define		AT_MAC_LENGTH			6
#define		AT_B_MAC_LENGTH			8
#define		AT_ADV_IND_LENGTH		10
#define		AT_ADV_LENGTH		    6
#define		AT_BEACON_LENGTH		9
#define		AT_UUID_LENGTH			7
#define		AT_FILT_LENGTH			7
#define		AT_SCAN_RSP_LENGTH		11
#define		AT_SCAN_LENGTH			7
#define		AT_CONN_PARAM_LENGTH	13
#define		AT_CONN_LENGTH			7
#define		AT_DISCONN_LENGTH		10
#define     AT_SLEEP_LENGTH			8
#define		AT_AUTH_LENGTH			7

#define		FAIL					0
#define		SUCCESS					1
#define		SUCCESS_NO_ECHO			2

typedef struct echo_statement_item
{
	uint8_t		data[40];
	uint16_t	data_len;       
}echo_item_s;

/**@brief Echo character list.
 * Instruction execution result.
 */
static echo_item_s echo_item[] =
{
	{"+0\n",3},				/**< Module startup. */
	{"+1\n",3},				/**< Instruction executed successfully. */
	{"+2\n",3},				/**< Enter Transmission mode. */
	{"+3\n",3},				/**< Enter Configuration mode. */
	{"+4\n",3},				/**< The command was not found. */
	{"+5\n",3},				/**< Invalid parameters. */
	{"+6\n",3},				/**< No Memory for operation. */
	{"+7\n",3},					// INVALID DATA
	{"+8\n",3},				/**< Enter sleep mode. */			
	{"+9\n",3},				/**< waked up. */				
	{"beelinker\n",10},		/**< No Memory for operation. */
	{"+Connect\r\n",10},
	{"Disconnect\r\n",11}
};

/**@brief CMD Event IDs.
 * IDs that uniquely identify an event coming from the cmd.c to the main.c.
 */
typedef enum
{
    APP_AT_ENTER,
    APP_AT_EXIT,
	APP_AT_ECHO,
	APP_AT_MAC,
    APP_ADV_MODE,
    APP_SLEEP,
	APP_STOP_SCAN,
	APP_START_SCAN,
	APP_DISCONNECT,
	APP_OUTPUT_INFO,
}cmd_evt_type;

/**@brief Common CMD Event type, wrapping the module event reports. */
typedef struct
{
	uint8_t  *ptr;
	uint16_t len;
    cmd_evt_type  event;
}cmd_evt_t;

/**@brief   Event handler type.
 *
 * @details This is the type of the event handler that is to be provided by the application
 *          of this module to receive events.
 */
typedef void ( *cmd_event_handler_t ) (cmd_evt_t *p_evt);

/**@brief   Command handler type.
 *
 * @details This is the type of the command handler that is to be provided by the cmd
 *          of this module to address of deposit.
 */
typedef uint8_t (*FUN_P)(uint8_t *p_data,uint16_t data_len);

/**@brief The command table initialization structure. */
typedef struct message_item
{
	char 	   at[20];		/**< Store instruction characters. */
	uint16_t   at_len;		/**< The length of the character. */
	FUN_P 	   exe_func;    /**< Handler for the command. */    
}msg_item_s;

typedef struct tx_power_item
{
    int i;
    int8_t 	tx_power;
}power_item_s;

#define UBOUND(Array)     (sizeof(Array)/sizeof(Array[0]))

bool searchmsgmap(uint8_t *p_data,uint16_t len);
void cmd_parser_init(cmd_event_handler_t event_handler);

#endif
