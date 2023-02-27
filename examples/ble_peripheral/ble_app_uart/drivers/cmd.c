#include "cmd.h"
#include "app_config.h"
#include "flash.h"
#include "nrf_log.h"
#include "data_format.h"
#include "nrf_fstorage.h"
static cmd_event_handler_t cmd_event_handler;
static config_t* p_config;
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
	if(data_len==AT_RESET_LENGTH)
	{
		NVIC_SystemReset();
	}
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
	if( p_data[AT_B_MAC_LENGTH] == '='&& p_data[AT_B_MAC_LENGTH+1] == '-')
	{
        int invalid_param_flag = 0;
		//AT+B_MAC=-d Delete MAC address.
        if(p_data[AT_B_MAC_LENGTH+2]=='d'&&data_len==12)
        {
            switch(p_data[AT_B_MAC_LENGTH+3])
            {
                case '0':
					p_config->b_addr[0].empty=0;
                    break;
                case '1':
                    p_config->b_addr[1].empty=0;
                    break;
                case '2':
                    p_config->b_addr[2].empty=0;
                    break;				
                default:
                    invalid_param_flag = 1;
                    break;                     
            }
            if( invalid_param_flag == 0 )
            {
                flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
				return	SUCCESS;
            }            
        }
		//AT+MAC=-b Binding MAC address.
        if(p_data[AT_B_MAC_LENGTH+2]=='b'&& data_len == 23)
        {			
			for(int i=0;i<4;i++)
			{
				if(p_config->b_addr[i].empty==0)
				{	
					uint32_t dest[6];
					memset(dest,0,6);
					if(sscanf((char*)&p_data[AT_B_MAC_LENGTH+3], "%2x%2x%2x%2x%2x%2x" ,
						&dest[5],&dest[4],&dest[3],&dest[2],&dest[1],&dest[0]))
					{
						for(int j=0;j<6;j++)
						{
							p_config->b_addr[i].addr[j]=dest[j];
						}
						p_config->b_addr[i].empty=1;
						flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
						while(nrf_fstorage_is_busy(NULL));
						return	SUCCESS;
					}
					else
						return	FAIL;
				}
				if(i==3) echo(echo_item[8].data,echo_item[8].data_len,APP_AT_ECHO);
			}			
        }
	}
	//	Query
	if( p_data[AT_B_MAC_LENGTH] == '?'&&data_len==9)
	{
        char data[20];
		memset(data, 0, sizeof(data));
		for(int i=0;i<4;i++)
		{
			memset(data,'\0',20);
			//Determine whether there is an address
			if(p_config->b_addr[i].empty!=0)
			{
				sprintf(data,"%d %X%X%X%X%X%X\r\n", 
					i,  
					p_config->b_addr[i].addr[5],\
					p_config->b_addr[i].addr[4],\
					p_config->b_addr[i].addr[3],\
					p_config->b_addr[i].addr[2],\
					p_config->b_addr[i].addr[1],\
					p_config->b_addr[i].addr[0]);
				echo((uint8_t*)data,strlen(data),APP_AT_ECHO);				
			}
			else
			{
				sprintf(data,"%d null\n",i);
				echo((uint8_t*)data,strlen(data),APP_AT_ECHO);
			}			
		}
		return	SUCCESS_NO_ECHO;		
	}
    return	FAIL;
}

static uint8_t adv_ind_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t uuid_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

power_item_s power_item[] =
{
    {1,-40},
    {2,-20},
    {3,-16},
    {4,-12},
    {5,-8},
    {6,-4},
    {7,0},
    {8,3},
    {9,4}
};

static uint8_t adv_func(uint8_t *p_data,uint16_t data_len)
{
	if( p_data[AT_ADV_LENGTH] == '=' && p_data[AT_ADV_LENGTH+1] == '-')//
	{
		uint32_t val=0;
        int invalid_param_flag = 0;
        switch(p_data[AT_ADV_LENGTH+2])
        {
            case 'i':
				if(sscanf((char*)&p_data[AT_ADV_LENGTH+3],"%d",&val))
				{
					if(val>=4&&val<=16384)
					{
						p_config->adv_interval = val;
						invalid_param_flag = 1;							
					}
				}
                break;
				
			case 't':
				if(sscanf((char*)&p_data[AT_ADV_LENGTH+3],"%d",&val))
				{
					if(val<=65535)
					{
						p_config->adv_timeout = val;
						invalid_param_flag = 1;							
					}
				}				
				break;
				
            case 'm':
				if((p_data[AT_ADV_LENGTH+3]=='0'||p_data[AT_ADV_LENGTH+3]=='1'||p_data[AT_ADV_LENGTH+3]=='2')&&data_len==10)
				{
					int mode=0;
					sscanf((char*)&p_data[AT_ADV_LENGTH+3], "%d", &mode);					
					if(p_config->adv_mode!=mode)
					{
						p_config->adv_mode = mode;						
						cmd_evt_t  cmd_evt;
						cmd_evt.event = APP_ADV_MODE;	
						(*cmd_event_handler)(&cmd_evt);						
					}
					invalid_param_flag = 1;
				}
                break;
				
            case 'p':				
				if(data_len==10)
				{
					int data=atoi((const char *)&(p_data[AT_ADV_LENGTH+3]));
					for(int n=0;n<9;n++)
					{		
						if(power_item[n].i == data)
						{
							p_config->adv_tx_power = power_item[n].tx_power;
							invalid_param_flag = 1;
						}
					}
				}
                break;
				
            case 'n':
				if(data_len>=9&&data_len<=36)
				{
					memset(p_config->device_name,0,sizeof(p_config->device_name));
					memcpy(p_config->device_name,&p_data[AT_ADV_LENGTH+3], data_len-(AT_ADV_LENGTH+3));
					invalid_param_flag = 1;
				}				
                break;
				
			default:
				break;
        }
        if(invalid_param_flag)
		{
			flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
			while(nrf_fstorage_is_busy(NULL));
			return	SUCCESS;
		}	
	}
    if(p_data[AT_ADV_LENGTH] == '?'&&data_len==7)
    {
		int tx_p_id;
        char data[200];
		for(int n=0;n<9;n++)
		{		
			if(power_item[n].tx_power == p_config->adv_tx_power)
			{
				tx_p_id = power_item[n].i;
			}
		}		
        sprintf(data, "-m%d-i%d-t%d-p%d-n%s\r\n",
				p_config->adv_mode,
				p_config->adv_interval,\
				p_config->adv_timeout,\
				tx_p_id,\
				p_config->device_name);
		echo((uint8_t*)data,strlen(data),APP_AT_ECHO);     ///   
		return	SUCCESS_NO_ECHO;
    }
	return	FAIL;
}

static uint8_t beacon_func(uint8_t *p_data,uint16_t data_len)
{
	uint32_t val[2] ={0};
	uint8_t *p;
	if( p_data[AT_BEACON_LENGTH] == '=' && p_data[AT_BEACON_LENGTH+1] == '-')//
	{
        int success_flag = 0;
        switch(p_data[AT_BEACON_LENGTH+2])
        {
            case 'i':
				if(data_len==16)
				{
					if(sscanf((char*)&p_data[AT_BEACON_LENGTH+3],"%4x",val))
					{
						//Data interception.
						p_config->company_id = val[0];
						success_flag = 1;
					}				
				}
                break;
				
            case 'u':
				if(data_len==44)
				{
					if(StrToU8_Hex_Lsb(&p_data[AT_BEACON_LENGTH+3],&p_config->m_beacon_info[2],32))
					{
						NRF_LOG_INFO("val=%2x",p_config->m_beacon_info[18]);
						NRF_LOG_INFO("val=%2x",p_config->m_beacon_info[19]);
						success_flag = 1;
					}
				}
                break;
					
            case 'a':
				if(data_len==16)
				{					
					if(sscanf((char*)&p_data[AT_BEACON_LENGTH+3],"%2x%2x",&val[0],&val[1]))
					{
						NRF_LOG_INFO("%x",val[0]);
						NRF_LOG_INFO("%x",val[1]);
						p_config->m_beacon_info[18]=val[0];
						p_config->m_beacon_info[19]=val[1];
						success_flag = 1;
					}	
				}
                break;
				
            case 'n':
				if(data_len==16)
				{
					if(sscanf((char*)&p_data[AT_BEACON_LENGTH+3],"%2x%2x",&val[0],&val[1]))
					{
						NRF_LOG_INFO("%x",val[0]);
						NRF_LOG_INFO("%x",val[1]);
						p_config->m_beacon_info[20]=val[0];
						p_config->m_beacon_info[21]=val[1];
						success_flag = 1;
					}	
				}
                break;
				
            case 'r':
				if(data_len == 14)
				{
					if(sscanf((char*)&p_data[AT_BEACON_LENGTH+3],"%2x",&val[0]))
					{
						NRF_LOG_INFO("%x",val[0]);
						p_config->m_beacon_info[22]=val[0];
						success_flag = 1;
					}	
				}
                break;
				
			default:
				break;
			
        }
        if( success_flag)
		{
			flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
			while(nrf_fstorage_is_busy(NULL));
			return	SUCCESS;
		}		
	}
    if(p_data[AT_BEACON_LENGTH] == '?'&&data_len==10)
    {
		char data[57];
        memset(data, 0, sizeof(data));
        sprintf(data,"-i%04X-u",p_config->company_id);
		u8_to_char_hex(&p_config->m_beacon_info[2],&data[8],16);
        sprintf(&data[40],"-a");
		u8_to_char_hex(&p_config->m_beacon_info[18],&data[42],2);
        sprintf(&data[46],"-n");
		u8_to_char_hex(&p_config->m_beacon_info[20],&data[48],2);
        sprintf(&data[52],"-r%02x\n",p_config->m_beacon_info[22]);
		echo((uint8_t*)data,57,APP_AT_ECHO);        ///
		return	SUCCESS_NO_ECHO;
    }
	return	FAIL;
}

static uint8_t filt_func(uint8_t *p_data,uint16_t data_len)
{
	int var;
	int success_flag = 0;
	if( p_data[AT_FILT_LENGTH] == '=' && p_data[AT_FILT_LENGTH+1] == '-')//
	{
        switch(p_data[AT_FILT_LENGTH+2])
        {
            case 't':				
				var = atoi((const char *)&(p_data[AT_FILT_LENGTH+3]));				
				if(var<5)
				{
					p_config->filt_type = var;
					success_flag = 1;
				}
                break;
            case 'n':
				if(data_len>=10&&data_len<=37)
				{
					memset(p_config->filt_name, 0, sizeof(p_config->filt_name));
					memcpy(p_config->filt_name,&p_data[AT_FILT_LENGTH+3], data_len-(AT_FILT_LENGTH+3));
					success_flag = 1;
				}
                break;
            case 'i':
                if(data_len==14)
                {
					//Inerger Promotion,Signed and unsigned char, short, and bit-fields are first integer lifted before they can perform arithmetic operations.
					//The scanf output variable memory occupies 32 bits.
					uint32_t dest;
					if(sscanf((char*)&p_data[AT_FILT_LENGTH+3],"%4x",&dest))
					{
						//Data interception.
						p_config->filt_companyid = dest;
						success_flag = 1;
					}
                }
                break;
            case 'u':
                if(data_len==14)
                {
					uint32_t dest;
					if(sscanf((char*)&p_data[AT_FILT_LENGTH+3],"%4x",&dest))
					{
						p_config->filt_uuid_16 = dest;
						success_flag = 1;
					}				
                }
                break;
            case 'm':
                if(data_len==22)
                {
					uint32_t dest[6];
					memset(dest,0,6);
					if(sscanf((char*)&p_data[AT_FILT_LENGTH+3], "%2x%2x%2x%2x%2x%2x" ,
						&dest[5],&dest[4],&dest[3],&dest[2],&dest[1],&dest[0]))
					{
						for(int i=0;i<6;i++)
						{
							p_config->filt_mac[i]=dest[i];
						}
						success_flag = 1;
					}						
                }
                break;
			default:
				break;
        }
        if(success_flag)
		{
			flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
			while(nrf_fstorage_is_busy(NULL));
			return	SUCCESS;
		}
	}
    if(p_data[AT_FILT_LENGTH] == '?'&&data_len==8)
    {
		char data[100];
        memset(data,0,sizeof(data));		
        sprintf(data,"-t%d-n%s-i%04X-u%04X-m%X%X%X%X%X%X\r\n",	
			p_config->filt_type,\
			p_config->filt_name,\
			p_config->filt_companyid,\
			p_config->filt_uuid_16,\
			p_config->filt_mac[5],\
			p_config->filt_mac[4],\
			p_config->filt_mac[5],\
			p_config->filt_mac[2],\
			p_config->filt_mac[1],\
			p_config->filt_mac[0]);
		
		echo((uint8_t*)data,strlen((const char*)data),APP_AT_ECHO);	
		return	SUCCESS_NO_ECHO;
    }
	return	FAIL;
}

static uint8_t scan_rsp_func(uint8_t *p_data,uint16_t data_len)
{
	;
}

static uint8_t scan_func(uint8_t *p_data,uint16_t data_len)
{
	
    if( p_data[AT_SCAN_LENGTH] == '='&&p_data[AT_SCAN_LENGTH+1] == '-'&&data_len<=15)
    {
		uint32_t val=0;
		int invalid_param_flag = 0;
		switch (p_data[AT_SCAN_LENGTH+2])
		{
			case 's':
				if(data_len==11)
				{					
					if(p_data[AT_SCAN_LENGTH+3]=='0')
					{
							p_config->scan_state = 0;
							cmd_evt_t  cmd_evt;
							cmd_evt.event = APP_STOP_SCAN;	
							(*cmd_event_handler)(&cmd_evt);	
							invalid_param_flag = 1;								
					}
					else if(p_data[AT_SCAN_LENGTH+3]=='1')
					{
							p_config->scan_state = 1;
							cmd_evt_t  cmd_evt;
							cmd_evt.event = APP_START_SCAN;	
							(*cmd_event_handler)(&cmd_evt);	
							invalid_param_flag = 1;								
					}
					else if(p_data[AT_SCAN_LENGTH+3]=='2')
					{
							p_config->scan_state = 2;
							invalid_param_flag = 1;
					}
					else;
				}						
				break;	
				
			case 'i':
				if(sscanf((char*)&p_data[AT_SCAN_LENGTH+3],"%d",&val))
				{
					if(val>=4&&val<=65535)
					{
						p_config->scan_interval = val;
						invalid_param_flag = 1;							
					}
				}									
				break;
				
			case 'w':
				if(sscanf((char*)&p_data[AT_SCAN_LENGTH+3],"%d",&val))
				{
					if(val>=4&&val<=65535)
					{
						p_config->scan_window = val;
						invalid_param_flag = 1;							
					}
				}				
				break;
				
			case 't':
				if(sscanf((char*)&p_data[AT_SCAN_LENGTH+3],"%d",&val))
				{
					if(val<=65535)
					{
						p_config->scan_timeout = val;
						invalid_param_flag = 1;							
					}
				}				
				break;
				
			default:
				break;
		}
		if(invalid_param_flag)
		{
			//scan_interval>=scan_window
			if(p_config->scan_interval>=p_config->scan_window)
			{
				flash_write(p_config->file_id, p_config->key,(uint8_t *)p_config, sizeof(config_t));
				while(nrf_fstorage_is_busy(NULL));
				return	SUCCESS;
			}
		}          
    }
	
    if(p_data[AT_SCAN_LENGTH] == '?'&&data_len==8)
    {
        char data [20];
        sprintf(data,"-s%d-i%d-w%d-t%d\n",	
			p_config->scan_state,\
			p_config->scan_interval,\
			p_config->scan_window,\
			p_config->scan_timeout);
		echo((uint8_t*)data,strlen(data),APP_AT_ECHO);				//
		return	SUCCESS_NO_ECHO;
    }
    return	FAIL;
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
	{ "+++"	,			3 , 	                enter_func  	},
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
			if(val==SUCCESS)
			{
				echo(echo_item[1].data,echo_item[1].data_len,APP_AT_ECHO);
			}
			else if(val==SUCCESS)
			{
				echo(echo_item[0].data,echo_item[0].data_len,APP_AT_ECHO);
			}
			else
			{
				;
			}
			return true;
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
	p_config = get_config();
    cmd_event_handler=event_handler;
}
