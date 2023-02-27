#include "flash.h"

#include "nrf_delay.h"
#include "nrf_log.h"

#define RESTORE_FILE    	0x0100
#define RESTORE_REC_KEY   	0x0100

static bool volatile m_fds_initialized;

/*********************************************************************
 * LOCAL VARIABLES
 */
static bool volatile s_fdsIfInitialized;												
static fds_record_desc_t s_recordDesc;
static uint8_t s_dataBuffer[FDS_BUFFER_SIZE];
static uint16_t s_currentId;

static void waitForFlashOperationToComplete(void)
{
	nrf_delay_ms(20);
}

static uint32_t flashWriteWithWaiting(fds_record_desc_t *pDesc, fds_record_t *pRec)
{
	ret_code_t retCode; 
	
	retCode = fds_record_write(pDesc, pRec);
	NRF_LOG_INFO("retCode3 = %x",retCode);
	waitForFlashOperationToComplete();

	return retCode;
}

static uint32_t flashDelete(fds_record_desc_t *pDesc)
{
	ret_code_t retCode; 
	
	retCode = fds_record_delete(pDesc);
	waitForFlashOperationToComplete();

	return retCode;
}


static uint32_t flashUpdateWithWaiting(fds_record_desc_t *pDesc, fds_record_t *pRec)
{
	ret_code_t retCode; 
	
	retCode = fds_record_update(pDesc, pRec);
	waitForFlashOperationToComplete();
	
	return retCode;
}


static uint8_t getWordLength(uint32_t dataLen)
{
	uint8_t word_length ;
	
	if(FDS_BUFFER_SIZE < dataLen)
	{
		dataLen = FDS_BUFFER_SIZE;
	}
	word_length = dataLen / 4;
	
	if(dataLen % 4)
	{
		word_length += 1;	
	}
	return word_length;
}

bool flash_write(uint16_t fileId, uint16_t key, uint8_t *pData, uint32_t dataLen)
{
	ret_code_t retCode;
	fds_record_desc_t recordDesc = {0};
	fds_find_token_t token = {0};
	fds_record_t record = {0};
		
	retCode = fds_record_find(fileId, key, &recordDesc, &token);
		
	record.file_id = fileId;
	record.key = key;		
	memcpy(s_dataBuffer, pData, dataLen);
	record.data.p_data = (uint8_t *) s_dataBuffer;
	record.data.length_words = getWordLength(dataLen);

	if(retCode == NRF_SUCCESS)			
	{
		retCode = flashUpdateWithWaiting(&recordDesc, &record);
		NRF_LOG_INFO("retCode1 = %x",retCode);
	}
	else 
	{
		flashDelete(&recordDesc);
		retCode = flashWriteWithWaiting(&recordDesc, &record);
		NRF_LOG_INFO("retCode2 = %x",retCode);
	}
	return true;
}

ret_code_t flash_read(uint16_t fileId, uint16_t key, uint8_t *pData, uint8_t dataLen)
{
	ret_code_t retCode;
	fds_record_desc_t recordDesc = {0};
	fds_find_token_t token = {0};

	retCode = fds_record_find(fileId, key, &recordDesc, &token);
	if(retCode == NRF_SUCCESS)
	{
		s_currentId = recordDesc.record_id;
			
		fds_flash_record_t record = {0};
		fds_record_open(&recordDesc, &record);

		memcpy(pData, record.p_data, dataLen);
		fds_record_close(&recordDesc);

		return NRF_SUCCESS;
	}
	else if(retCode == FDS_ERR_NOT_FOUND)
	{
		return FDS_ERR_NOT_FOUND;
	}
	else return false;
}

void Fds_FlashContrl(uint16_t fileId,uint16_t keyId, uint8_t readWriteFlag, uint8_t *pData, uint32_t dataLen)
{
    if(readWriteFlag == FDS_READ)                       								
    {
		flash_read(fileId, keyId, pData, dataLen);
    }
    else if(readWriteFlag == FDS_WRITE) 																				
    {
		flash_write(fileId, keyId, pData, dataLen);
    }
}

static void fdsCallbackFunc(fds_evt_t const *pFdsEvent)
{
    switch(pFdsEvent->id)
    {
        case FDS_EVT_INIT:
		{
            if(pFdsEvent->result == NRF_SUCCESS)
            {
               s_fdsIfInitialized = true;
            }
		} break;

        case FDS_EVT_WRITE:
        {
			if(pFdsEvent->result == NRF_SUCCESS)
			{
				NRF_LOG_INFO("Record IDw:\t0x%04x",  pFdsEvent->write.record_id);
				NRF_LOG_INFO("File ID:\t0x%04x",    pFdsEvent->write.file_id);
				NRF_LOG_INFO("Record key:\t0x%04x", pFdsEvent->write.record_key);
			}
        } break;
		
		case FDS_EVT_UPDATE:
        {
			if(pFdsEvent->result == NRF_SUCCESS)
			{
				NRF_LOG_INFO("Record IDu:\t0x%04x",  pFdsEvent->write.record_id);
				NRF_LOG_INFO("File ID:\t0x%04x",    pFdsEvent->write.file_id);
				NRF_LOG_INFO("Record key:\t0x%04x", pFdsEvent->write.record_key);
			}
        } break;

        case FDS_EVT_DEL_RECORD:
        {
            if(pFdsEvent->result == NRF_SUCCESS)
            {
                NRF_LOG_INFO("Record IDd:\t0x%04x",  pFdsEvent->del.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    pFdsEvent->del.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", pFdsEvent->del.record_key);
            }
        } break;

        default:
			break;
    }
}


bool flash_fds_init(void)
{
	ret_code_t retCode;
	fds_record_t record;
		
	(void)fds_register(fdsCallbackFunc);										
	
	retCode = fds_init();																		
	APP_ERROR_CHECK(retCode);
	while(!s_fdsIfInitialized)															
    {
        sd_app_evt_wait();																
    }	
	

	retCode = fds_record_write(&s_recordDesc, &record);
	
	if(retCode == NRF_SUCCESS)
	{
		return true;
	}
	else
	{
		fds_gc();
	}
	return false;
}
