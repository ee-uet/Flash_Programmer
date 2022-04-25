#include <inttypes.h>

// Flash related defines
#define FLASH_SECTOR_ADDRESS         0x00
#define FLASH_SECTOR_ERASE           0x20
#define FLASH_WRITE_STATUS_REG       0x01
#define FLASH_READ_STATUS_REG        0x05
#define FLASH_CMD_READ               0x03
#define FLASH_CMD_WRITE_PAGE         0x02
#define FLASH_CMD_WRITE_ENABLE       0x06
#define FLASH_CMD_READ_ID            0x9F

#define FLASH_PAGE_SIZE              0x100
#define FLASH_APP_START_ADDRESS      0x00
#define FLASH_STATUS_REG_VALUE       0x42

// Function prototypes for Flash 
uint8_t Flash_Read_Status_Register(void);
void Flash_Write_Status_Register(void);
void Flash_Write_Enable(void);
void Flash_Write_Data(uint32_t addr, uint8_t *data, uint16_t len);
void Flash_Write_Page(uint32_t addr, uint8_t *data, uint16_t len);
void Flash_Write_Addr(uint32_t addr);
void Flash_Read_Data(uint32_t addr, uint8_t *data, uint16_t len);

void Flash_Sector_Erase(uint32_t sector_addr);
uint32_t Flash_Manf_Device_ID(void);
