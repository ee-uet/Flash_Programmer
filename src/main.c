/****************************************************************** 
* Filename :    main.c                                    
* Date     :    28-12-2021                                
* Author   :    Muhammad Tahir                            
*                                                         
* Description:  Flash memory driver for writing and 
*               reading its contents.                                 
******************************************************************/ 

#include <inttypes.h>
#include "../include/spi.h"
#include "../include/gpio.h"
#include "../include/flash.h"

#define MAX_APP_SIZE               0x200

// Global variables
unsigned char Status = 0;

extern uint8_t Imem_Content [];
extern uint32_t imem_size;

uint8_t Read_Array[MAX_APP_SIZE] = {0};

// Application main function
int main(void){

	uint32_t device_id = 0, count = 0;
	uint8_t  flag = 0;
	
	// Initialize the GPIO module for port F
	GPIO_Init();

  // Initialize the SPI module
  SPI_Init();                 

  // Read flash manfacturer and device ID
	do{
	  device_id = Flash_Manf_Device_ID();
	} while(device_id == 0);
		
	// Enable flach write operation
	Flash_Write_Enable();
	Status = Flash_Read_Status_Register();
		
	// Erase the fash sector before writing
	Flash_Sector_Erase(FLASH_SECTOR_ADDRESS);
	
	// Write the user application executable to the flash
	Flash_Write_Data(FLASH_APP_START_ADDRESS, ((uint8_t *)Imem_Content), imem_size);
	
	// Read the user execuatable from flash for verification purpose
	Flash_Read_Data(FLASH_APP_START_ADDRESS, Read_Array, imem_size);
	
	
	for(count = 0; count < imem_size; count++)
	{ 
		// Verify flash write operation
		if(Imem_Content[count] != Read_Array[count])
		{
			flag = 1;
			break;
		}
		else
			flag = 0;
		
	}
	
	// LED indicator for correct programming of the flash.
	if(flag == 1)
	{
		// Turn on the red LED to signal that flash was not programmed properly
	  GPIO_PORTF_DATA_R = GPIO_LED_RED;
	}
	else
	{
		// Turn on the green LED
	  GPIO_PORTF_DATA_R = GPIO_LED_GREEN;
	}
		
	// Endless loop
	while(1){};
 			
}

