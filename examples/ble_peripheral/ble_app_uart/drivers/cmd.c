#include "cmd.h"
#include "app_config.h"
#include "flash.h"
#include "nrf_log.h"
static cmd_event_handler_t cmd_event_handler;

void echo(uint8_t* p_data,uint16_t data_len,cmd_evt_type cmd_event)
{
	cmd_evt_t  cmd_evt;
	cmd_evt.event = cmd_event;
	cmd_evt.len	= data_len;
	cmd_evt.ptr = p_data;
	(*cmd_event_handler)(&cmd_evt);	
}

static uint8_t enter_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t exit_func (uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t reset_func (uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t restore_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t m_info_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t baud_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t role_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t mac_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t b_mac_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t adv_ind_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t uuid_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t adv_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t beacon_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t filt_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t scan_rsp_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t scan_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t conn_param_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t conn_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t disconn_func(uint8_t *p_data,uint16_t data_len)
{	
	;
}

static uint8_t sleep_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t auth_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

msg_item_s msg_map[]={
	{ "+++"	, 		    3 , 	                enter_func  	},
	{ "AT+EXIT",		AT_EXIT_LENGTH , 		exit_func 		},
	{ "AT+RESET",		AT_RESET_LENGTH , 		reset_func 		},
	{ "AT+RESTORE",		AT_RESTORE_LENGTH , 	restore_func 	},
	{ "AT+M_INFO",		AT_M_INFO_LENGTH , 		m_info_func 	},
	{ "AT+BAUD",		AT_BAUD_LENGTH , 		baud_func 		},
	{ "AT+ROLE",		AT_ROLE_LENGTH , 		role_func 		},
	{ "AT+MAC" ,		AT_MAC_LENGTH ,  		mac_func 		},
	{ "AT+B_MAC",		AT_B_MAC_LENGTH ,  		b_mac_func 		},
	{ "AT+ADV_IND",		AT_ADV_IND_LENGTH , 	adv_ind_func 	},
	{ "AT+UUID",	    AT_UUID_LENGTH , 		uuid_func 		},
	{ "AT+ADV",		    AT_ADV_LENGTH , 	    adv_func 	    },
	{ "AT+BEACON",		AT_BEACON_LENGTH , 		beacon_func 	},
	{ "AT+FILT",		AT_FILT_LENGTH , 		filt_func 		},
	{ "AT+SCAN_RSP",	AT_SCAN_RSP_LENGTH , 	scan_rsp_func 	},
	{ "AT+SCAN",		AT_SCAN_LENGTH , 		scan_func 		},
	{ "AT+CONN_PARAM",	AT_CONN_PARAM_LENGTH , 	conn_param_func },
	{ "AT+CONN",		AT_CONN_LENGTH , 		conn_func 		},
	{ "AT+DISCONN",	    AT_DISCONN_LENGTH , 	disconn_func 	},
	{ "AT+SLEEP",		AT_SLEEP_LENGTH , 		sleep_func 		},
	{ "AT+AUTH",		AT_AUTH_LENGTH , 		auth_func 		},	
};

/** @brief Function for query the string from the order table.
 *
 * @param[in] p_data   	Pointer to the instruction character.
 * @param[in] data_len  The length of the string.
 */
bool searchmsgmap(uint8_t *p_data,uint16_t data_len)
{	
	int n;
	uint8_t val;
    for (n=0;n<UBOUND(msg_map);n++)
    {
        if(!strncmp(msg_map[n].at,(char*)p_data,msg_map[n].at_len))
        {
			p_data[data_len]='\0';
            val = msg_map[n].exe_func(p_data,data_len);			
        }
		/**/
    }
	if( n == UBOUND(msg_map) )
	{
		echo(echo_item[4].data,echo_item[4].data_len,APP_AT_ECHO);
	}
	return	false;
}

/** @brief Function for initializing the cmd event handle.
 *
 * @param[in] event_handler   	cmd event.
 */
void cmd_parser_init(cmd_event_handler_t event_handler)
{
    cmd_event_handler=event_handler;
}
