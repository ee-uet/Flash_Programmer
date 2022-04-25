/****************************************************************** 
 * Filename :    spi.c                                    
 * Date     :    28-12-2021                                
 * Author   :    Muhammad Tahir                            
 *                                                         
 * Description:  Flash memory driver for writing and 
 *               reading its contents.                                 
 ******************************************************************/ 
 
#include <inttypes.h>
#include "../include/spi.h"


// SPI intialization and GPIO alternate function configuration
void SPI_Init(void)
{		
  RCGC_GPIO_R |=  CLOCK_GPIOD;  // Enable the clock for port D 
	RCGC_SPI_R |= CLOCK_SPI1;     // Enable the clock for SPI 1
	GPIO_PORTD_DEN_R |= 0x0F;     // DEN for port D pins 0-3
	GPIO_PORTD_DIR_R |= 0x02;
  
  // Configure Port D pins 0-3 for SPI 1 
  GPIO_PORTD_AFSEL_R |= 0x0000000D;
  GPIO_PORTD_PCTL_R |= 0x00002202;
	  
  // Disable SPI and perform control configuration
  SPI_CONTROL1_R &= ~(SPI_ENABLE);

  // Freescale frame format, mode 3, 8 bit data & 1 MHz SPI clock
  SPI_CONTROL0_R |= 0x00C7;   
  SPI_CLK_PRESCALE_R = 0x0001;    // Prescale system clock by 2
  SPI_CLK_CONFIG_R = 0x00;        // Clock source is system clock

  SPI_CONTROL1_R |= SPI_ENABLE + SPI_EOT;   // Enable SPI master module
}

// This function transmits one data element and reads the 
// received byte  
uint8_t SPI_Transfer(unsigned char Data)
{ 
	uint8_t value = 0;
   
	SPI_DATA_R = (Data & 0xFF);
	while(SPI_STATUS_R & SPI_BUSY_FLAG);	
	
	value = (SPI_DATA_R & 0xFF);
  return value;
}

