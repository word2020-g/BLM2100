#ifndef DATA_FORMAT_H
#define DATA_FORMAT_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool StrToHexArray(const uint8_t *p_src,uint8_t *p_dest,uint16_t len);
bool StrToInt(const uint8_t *p_src,uint32_t *p_dest,uint16_t len);
bool StrToInt_u16(const uint8_t *p_src,uint16_t *p_dest,uint16_t len);

bool StrToU8_Hex(const uint8_t *p_src,uint8_t *p_dest,uint16_t len);
bool StrToU16_Hex(const uint8_t *p_src,uint16_t *p_dest,uint16_t len);
bool StrToU8_Hex_Lsb(const uint8_t *p_src,uint8_t *p_dest,uint16_t len);
void hex_to_char_msb(char *p_dest,uint8_t *p_src,uint16_t data_len);
void u8_to_char_hex(uint8_t *p_src,char *p_dest,uint16_t data_len);

#endif
