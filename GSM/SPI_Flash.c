#include "spi.h"
#include "gpio.h"
#include "SPI_Flash.h"
#include "stm32f4xx_hal.h"


extern SPI_HandleTypeDef hspi1;

void SPIFlash_Init(void)
{
  SPIFlash_SendCmd(WRDI);
  SPIFlash_SendCmd(EWSR);
  
  SPI_Flash_CS_Set(0);
  uint8_t cmd;
  cmd = WRSR; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = 0x00; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  SPI_Flash_CS_Set(1);  
  ReadUniqueNumber();
  
  /*
  uint8_t testArrayWrite[10]={0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F};
  uint8_t testArrayRead[50]={0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
  SPIFlash_EraseSector(0x00000000);

  SPIFlash_WriteArray(0x00000000, testArrayWrite, 10);
  SPIFlash_ReadArray(0x00000000, testArrayRead, 15);
  SPIFlash_GetStatus();
  */
}


uint8_t SPIFlash_SendCmd(uint8_t cmd)
{
  uint8_t readByte;
  SPI_Flash_CS_Set(0);
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  //HAL_SPI_Receive(&hspi1, &readByte, 1, 1);
  SPI_Flash_CS_Set(1);
  return readByte;
}

    volatile uint8_t ttest[2];

uint16_t SPIFlash_GetStatus(void)
{
  SPI_Flash_CS_Set(0);
  uint8_t cmd;
  uint8_t status[2] = {0x11,0x11};
  uint16_t result;

  
  
  cmd = RDSR; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  HAL_SPI_Receive(&hspi1, &status[0], 1, 1);
  HAL_SPI_Receive(&hspi1, &status[1], 1, 1);
  result = status[0];result<=8;
  result |= status[1];

  SPI_Flash_CS_Set(1);
  return result;
}


void ReadUniqueNumber(void)
{
  SPI_Flash_CS_Set(0);
  uint8_t cmd;
  uint8_t readByte[8];
  cmd = 0x4B; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = 0x00; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = 0x00; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = 0x00; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = 0x00; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  for(int i=0; i < 8; i++)
  {
    HAL_SPI_Receive(&hspi1, &readByte[i], 1, 1);
  }
  SPI_Flash_CS_Set(1);
  cmd = cmd;
}

  
void SPIFlash_WaitWhileBusy(void)
{
  SPI_Flash_CS_Set(0);
  uint8_t cmd;
  uint8_t readByte;
  cmd = RDSR; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  do{
    HAL_SPI_Receive(&hspi1, &readByte, 1, 1);
  }while(readByte & BUSY);
  SPI_Flash_CS_Set(1);
}

void SPIFlash_ReadArray(uint32_t address, uint8_t* data, uint16_t len)
{
  SPI_Flash_CS_Set(0);
  uint8_t cmd;
  cmd = READ; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[2]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[1]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[0]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);

  while(len--)
  {
    HAL_SPI_Receive(&hspi1, data++, 1, 1);
  }
  SPI_Flash_CS_Set(1);
}


void SPIFlash_WriteArray(uint32_t address, uint8_t* data, uint16_t len)
{
  uint16_t Len = len;
  uint32_t Address = address;
  
  while(Len)
  {
    SPIFlash_SendCmd(WREN);
    SPI_Flash_CS_Set(0);
    uint8_t cmd;
    cmd = WRITE; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
    cmd = ((uint8_t*)address)[2]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
    cmd = ((uint8_t*)address)[1]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
    cmd = ((uint8_t*)address)[0]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
    do
    {
      HAL_SPI_Transmit(&hspi1, data++, 1, 1);
      Len--;
      Address++;
    } while(Len && (Address & SPI_FLASH_PAGE_MASK));

    SPI_Flash_CS_Set(1);
    
    SPIFlash_WaitWhileBusy();
  }
}


void SPIFlash_EraseSector(uint32_t address)
{
  uint8_t cmd;
  
  SPIFlash_SendCmd(WREN);
  
  SPI_Flash_CS_Set(0);
  cmd = ERASE_4K; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[2]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[1]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  cmd = ((uint8_t*)address)[0]; HAL_SPI_Transmit(&hspi1, &cmd, 1, 1);
  SPI_Flash_CS_Set(1);

  SPIFlash_WaitWhileBusy();

}

