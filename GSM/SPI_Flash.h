#define READ            0x03    // SPI Flash opcode: Read up up to 25MHz
#define READ_FAST       0x0B    // SPI Flash opcode: Read up to 50MHz with 1 dummy byte
#define ERASE_4K        0x20    // SPI Flash opcode: 4KByte sector erase
#define ERASE_32K       0x52    // SPI Flash opcode: 32KByte block erase
#define ERASE_SECTOR    0xD8    // SPI Flash opcode: sector block erase
#define ERASE_ALL       0x60    // SPI Flash opcode: Entire chip erase
#define WRITE           0x02    // SPI Flash opcode: Write one byte
#define WRITE_STREAM    0xAD    // SPI Flash opcode: Write continuous stream of words (AAI mode)
#define RDSR            0x05    // SPI Flash opcode: Read Status Register
#define EWSR            0x50    // SPI Flash opcode: Enable Write Status Register
#define WRSR            0x01    // SPI Flash opcode: Write Status Register
#define WREN            0x06    // SPI Flash opcode: Write Enable
#define WRDI            0x04    // SPI Flash opcode: Write Disable / End AAI mode
#define RDID            0x90    // SPI Flash opcode: Read ID
#define JEDEC_ID        0x9F    // SPI Flash opcode: Read JEDEC ID
#define EBSY            0x70    // SPI Flash opcode: Enable write BUSY status on SO pin
#define DBSY            0x80    // SPI Flash opcode: Disable write BUSY status on SO pin


#define BUSY    0x01    // Mask for Status Register BUSY bit
#define WEL     0x02    // Mask for Status Register BUSY bit
#define BP0     0x04    // Mask for Status Register BUSY bit
#define BP1     0x08    // Mask for Status Register BUSY bit
#define BP2     0x10    // Mask for Status Register BUSY bit
#define BP3     0x20    // Mask for Status Register BUSY bit
#define AAI     0x40    // Mask for Status Register BUSY bit
#define BPL     0x80    // Mask for Status Register BUSY bit


#define SPI_FLASH_PAGE_SIZE		(256ul)		// SST has no page boundary requirements
#define SPI_FLASH_SECTOR_SIZE		(4096ul)
#define SPI_FLASH_SECTOR_MASK		(SPI_FLASH_SECTOR_SIZE - 1)
#define SPI_FLASH_PAGE_MASK		(SPI_FLASH_PAGE_SIZE - 1)



extern void SPIFlash_Init(void);
uint8_t SPIFlash_SendCmd(uint8_t cmd);
extern uint16_t SPIFlash_GetStatus(void);
void SPIFlash_WaitWhileBusy(void);
void ReadUniqueNumber(void);
extern void SPIFlash_ReadArray(uint32_t address, uint8_t* data, uint16_t len);
extern void SPIFlash_WriteArray(uint32_t address, uint8_t* data, uint16_t len);
extern void SPIFlash_EraseSector(uint32_t address);