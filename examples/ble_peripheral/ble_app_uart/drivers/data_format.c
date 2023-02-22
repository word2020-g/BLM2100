#include "data_format.h"
#include "nrf_log.h"
/** @brief Function for strings are formatted in unsigned int.
 *
 * @param[in] p_dest   Pointer to the unsigned int.
 * @param[in] p_src    Pointer to the strings.
 * @param[in] len      The length of the string.
 */
bool StrToInt(const uint8_t *p_src,uint32_t *p_dest,uint16_t len)
{
	*p_dest=0;
	if (NULL == *p_src)	
		return false;
	else {
		for(int i=0;i<len;i++)
		{
			if (*p_src<'0' || *p_src>'9')		
				return false;
			else
				*p_dest = *p_dest * 10 + (*p_src - '0');
			p_src++;
		}
	}
	return true;
}

bool StrToInt_u16(const uint8_t *p_src,uint16_t *p_dest,uint16_t len)
{
	*p_dest=0;
	if (NULL == *p_src)	
		return false;
	else {
		for(int i=0;i<len;i++)
		{
			if (*p_src<'0' || *p_src>'9')		
				return false;
			else
				*p_dest = *p_dest * 10 + (*p_src - '0');
			p_src++;
		}
	}
	return true;
}

/** @brief Function for strings are formatted in hexadecimal.
 *
 * @param[in] p_dest   Pointer to the hexadecimal array.
 * @param[in] p_src    Pointer to the strings.
 * @param[in] len      The length of the string.
 */
bool StrToHexArray(const uint8_t *p_src,uint8_t *p_dest,uint16_t len)
{
	uint8_t src;
	uint32_t dest;
	memset(p_dest,'\0',len);
	for(int i=0;i<len;i++)
	{
		src=p_src[i];
		if((src>='0'&&src<='9') || (src>='A'&&src<='F') || (src>='a'&&src<='f'))
		{
			sscanf((char*)&src,"%x",&dest);
			p_dest[i]=dest;
			src=0;
			dest=0;
		}
		else
		{
			return false;
		}
	}
	return true;
}

/** @brief Function for strings are formatted in 8-bit hexadecimal data.
 *
 * @param[in] p_dest   Pointer to the hexadecimal array.
 * @param[in] p_src    Pointer to the strings.
 * @param[in] len      The length of the string.
 */
bool StrToU8_Hex(const uint8_t *p_src,uint8_t *p_dest,uint16_t len)
{
	uint8_t src_hex[len];
	uint32_t data;
	bool flag =1;
	memset(p_dest,'\0',sizeof(p_dest));
	if(StrToHexArray(p_src,src_hex,len))
	{
		for(int i=0;i<len;i+=2)
		{
			data=src_hex[i]<<4;
			data=data|src_hex[i+1];			
			p_dest[i/2]=(uint8_t)data;
		}
		return true;		
	}
	else return false;
}

bool StrToU8_Hex_Lsb(const uint8_t *p_src,uint8_t *p_dest,uint16_t len)
{
	uint8_t src_hex[len];
	uint32_t data;
	
	memset(p_dest,'\0',sizeof(p_dest));
	if(StrToHexArray(p_src,src_hex,len))
	{
		for(int i=0;i<len;i+=2)
		{
			data=src_hex[i]<<4;
			data=data|src_hex[i+1];
			p_dest[(len/2)-(i/2)-1]=(uint8_t)data;
		}
		return true;		
	}
	else
		return false;
}

void hex_to_char_msb(char *p_dest,uint8_t *p_src,uint16_t data_len)
{
	uint8_t data[data_len];
	for (int j = 0; j < data_len ; j++)
	{
		data[j] = p_src[data_len-j-1];
	}
	for (int i = 0; i < data_len; i++)
    {
        sprintf(&p_dest[i * 2],"%02X",data[i]);
    }

}

void u8_to_char_hex(uint8_t *p_src,char *p_dest,uint16_t data_len)
{
	for (int i = 0; i < data_len; i++)
    {
        sprintf(&p_dest[i * 2],"%02X",p_src[i]);
    }
}

/** @brief Function for strings are formatted in 16-bit hexadecimal data.
 *
 * @param[in] p_dest   Pointer to the hexadecimal array.
 * @param[in] p_src    Pointer to the strings.
 * @param[in] len      The length of the string.
 */
bool StrToU16_Hex(const uint8_t *p_src,uint16_t *p_dest,uint16_t len)
{
	uint8_t src_hex[len];
	uint32_t data;
	
	memset(p_dest,'\0',sizeof(p_dest));
	if(StrToHexArray(p_src,src_hex,len))
	{
		for(int i=0,j=0;i<len;i+=4,j++)
		{
			data=src_hex[i]<<12|src_hex[i+1]<<8|src_hex[i+2]<<4|src_hex[i+3];
			p_dest[i/4]=(uint16_t)data;
		}
	}
	else return false;
	
	return true;
}
