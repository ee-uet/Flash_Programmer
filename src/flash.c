/****************************************************************** 
* Filename :    flash.c                                    
* Date     :    28-12-2021                                
* Author   :    Muhammad Tahir                            
*                                                         
* Description:  Flash memory driver for writing and 
*               reading its contents.                                 
******************************************************************/ 
 
#include <inttypes.h>
#include "../include/spi.h"
#include "../include/flash.h"

// Global variables
unsigned char Manf_ID = 0;
unsigned char Device_ID1 = 0;
unsigned char Device_ID2 = 0;


uint8_t Flash_Read_Status_Register(void)
{
	uint8_t status = 0;
	
	  SPI_EN();	
		SPI_Transfer(FLASH_READ_STATUS_REG);
		status = SPI_Transfer(0x00);
	  SPI_DIS();
		return status;
}

void Flash_Write_Enable(void)
{
	
	SPI_EN();	
	SPI_Transfer(FLASH_CMD_WRITE_ENABLE);
	SPI_DIS();
}

void Flash_Write_Status_Register(void)
{
	  SPI_EN();		
		SPI_Transfer(FLASH_WRITE_STATUS_REG);
	  SPI_Transfer(FLASH_STATUS_REG_VALUE);
	  SPI_DIS();
}

void Flash_Write_Data(uint32_t addr, uint8_t *data, uint16_t len)
{	
	uint8_t *src = data;
	uint32_t dst = addr;
	uint16_t bytes_left = len, bytes = 0;
	

	if (bytes_left < FLASH_PAGE_SIZE)
		 bytes = bytes_left;
	else
		 bytes = FLASH_PAGE_SIZE;
	

  while(bytes_left > 0)
	{
		Flash_Write_Enable();
		Flash_Write_Page(dst, src, bytes);

		src += bytes;
		dst += bytes;
		bytes_left -= bytes;
    
		if (bytes_left < FLASH_PAGE_SIZE){
       bytes = bytes_left;
    }
  }
	
}

void Flash_Write_Page(uint32_t addr, uint8_t *data, uint16_t len)
{
	uint16_t i = 0;
	uint8_t status = 0;
	
	  SPI_EN();		
		
		SPI_Transfer(FLASH_CMD_WRITE_PAGE);
	  Flash_Write_Addr(addr);
		
	  for(i=0; i < len; i++)
	    SPI_Transfer(*data++);
	
	  SPI_DIS();
	
		do{
			status = Flash_Read_Status_Register();
		}	while(status & 0x01);
}
		
void Flash_Read_Data(uint32_t addr, uint8_t *data, uint16_t len)
{
	uint16_t i = 0;
	
	  SPI_EN();	
		
		SPI_Transfer(FLASH_CMD_READ);
	  Flash_Write_Addr(addr);
		
	  for(i=0; i < len; i++)
	    *data++ = SPI_Transfer(0x00);
	
	  SPI_DIS(); 
}

void Flash_Sector_Erase(uint32_t sector_addr)
{
	unsigned char status = 0;
	
	Flash_Write_Enable();
	
	SPI_EN();		
		
	SPI_Transfer(FLASH_SECTOR_ERASE);
	Flash_Write_Addr(sector_addr);
	
	SPI_DIS(); 
	
	do{
			status = Flash_Read_Status_Register();
		}	while(status & 0x01);
}


void Flash_Write_Addr(uint32_t addr) {

  union {
    uint32_t uint32;
    uint8_t  uint8[sizeof(uint32_t)];
  } address;

	int i; 
	address.uint32 = addr;
  
  for (i=2; i>=0; i--) {
    SPI_Transfer(address.uint8[i]);
  }
}


uint32_t Flash_Manf_Device_ID(void){

  union {
    uint32_t id_word;
    uint8_t  id_bytes[sizeof(uint32_t)];
  } man_id;

	int i; 
    
	SPI_EN();		
		
	// Send ID commnad
	SPI_Transfer(FLASH_CMD_READ_ID);
	
  for (i=2; i>=0; i--) {
    man_id.id_bytes[i] = SPI_Transfer(0x00);
  }
	
	SPI_DIS();
	return man_id.id_word;
  	
}


