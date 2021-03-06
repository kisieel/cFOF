#include "main.h"

/* 

Based on PM0062.
** DBL WORD-  64 bit
** WORD-      32 bit
** HALF WORD- 16 bit
** BYTE-      8 bit 

** Addresses are expressed in bytes

FAST WRITE- assuming value is already equal to zero. It takes
the 1 tprog to program the memory. If the value isn't equal to 
zero Flash interface clears it and then writes to it. It takes
2 tprog to program the memory. FTDW bit in PECR is responsible
for fast/normal operations.

*/

#define EEPROM_BASE           ((uint32_t)0x08080000) /*!< EEPROM base address in the alias region */

uint8_t EEPROM_SygAddress[] = {
	EEPROM_Syg1Address,
	EEPROM_Syg2Address,
	EEPROM_Syg3Address,
	EEPROM_Syg4Address
};

// Private functions
void     EEPROM_SystemBackup(uint8_t syg);

void     EEPROM_64_erase(uint32_t ADDRESS);
void     EEPROM_32_erase(uint32_t ADDRESS);

void     EEPROM_64_write(uint32_t ADDRESS, uint64_t DATA);
void     EEPROM_32_write(uint32_t ADDRESS, uint32_t DATA);
void     EEPROM_16_write(uint32_t ADDRESS, uint16_t DATA);
void     EEPROM_8_write(uint32_t ADDRESS, uint8_t DATA);

void     EEPROM_32_fast_write(uint32_t ADDRESS, uint32_t DATA);
void     EEPROM_16_fast_write(uint32_t ADDRESS, uint16_t DATA);
void     EEPROM_8_fast_write(uint32_t ADDRESS, uint8_t DATA);

uint64_t EEPROM_64_read(uint32_t ADDRESS);
uint32_t EEPROM_32_read(uint32_t ADDRESS);
uint16_t EEPROM_16_read(uint32_t ADDRESS);
uint8_t  EEPROM_8_read(uint32_t ADDRESS);

void     EEPROM_unlock(void);
void     EEPROM_lock(void);
void     EEPROM_option_unlock(void);
void     EEPROM_option_lock(void);
// End of private functions

void EEPROM_SystemBackup(uint8_t syg)
{
	EEPROM_32_write(EEPROM_SygAddress[syg], System[syg].ActAnimation  << EEPROM_1_ActAnimationPosition
	                                      | System[syg].ActColor      << EEPROM_1_ActColorPosition
	                                      | System[syg].ActBrightness << EEPROM_1_ActBrightnessPosition
	                                      | System[syg].ActAlarmTone  << EEPROM_1_ActAlarmTonePosition
	                                      | System[syg].ActAlarmVol   << EEPROM_1_ActAlarmVolPosition
	                                      | System[syg].ActAlarmTempo << EEPROM_1_ActAlarmTempoPosition
	                                      | System[syg].ActMusic      << EEPROM_1_ActMusicPosition
	                                      | System[syg].ActAddress    << EEPROM_1_ActAddressPosition);
}

void EEPROM_unlock(void)
{
	FLASH->PEKEYR = (uint32_t)0x89ABCDEF;
	FLASH->PEKEYR = (uint32_t)0x02030405;
	
	while(FLASH->PECR & FLASH_PECR_PELOCK);
}

void EEPROM_lock(void)
{
	FLASH->PECR |= FLASH_PECR_PELOCK;
}

void EEPROM_option_unlock(void)
{
	EEPROM_unlock();
	FLASH->OPTKEYR = (uint32_t)0xFBEAD9C8;
	FLASH->OPTKEYR = (uint32_t)0x24252627;
}

void EEPROM_option_lock(void)
{
	FLASH->PECR |= FLASH_PECR_OPTLOCK;
}

void EEPROM_64_erase(uint32_t ADDRESS)
{
	// Page 19
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR |= FLASH_PECR_ERASE;
	FLASH->PECR |= FLASH_PECR_DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	
	*ptr = (uint32_t)0x00000000;
	ptr += 4;
	while(FLASH->SR & FLASH_SR_BSY);
	*ptr = (uint32_t)0x00000000;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_32_erase(uint32_t ADDRESS)
{
	// Page 18
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	
	*ptr = (uint32_t)0x00000000;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_64_write(uint32_t ADDRESS, uint64_t DATA)
{
	// Page 21
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR |= FLASH_PECR_FPRG;
	FLASH->PECR |= FLASH_PECR_DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	
	*ptr = (uint32_t)(DATA & 0x00000000FFFFFFFF);
	ptr += 4;
	while(FLASH->SR & FLASH_SR_BSY);
	*ptr = (uint32_t)((DATA & 0xFFFFFFFF00000000) >> 32);
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_32_write(uint32_t ADDRESS, uint32_t DATA)
{
	// Page 22
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR |= FLASH_PECR_FTDW;
	
	*ptr = (uint32_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_16_write(uint32_t ADDRESS, uint16_t DATA)
{
	// Page 23
	uint16_t *ptr;
	ptr = (uint16_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR |= FLASH_PECR_FTDW;
	
	*ptr = (uint16_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_8_write(uint32_t ADDRESS, uint8_t DATA)
{
	// Page 24
	uint8_t *ptr;
	ptr = (uint8_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR |= FLASH_PECR_FTDW;
	
	*ptr = (uint8_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_32_fast_write(uint32_t ADDRESS, uint32_t DATA)
{
	// Page 22
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR &= ~(FLASH_PECR_FTDW);
	
	*ptr = (uint32_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_16_fast_write(uint32_t ADDRESS, uint16_t DATA)
{
	// Page 23
	uint16_t *ptr;
	ptr = (uint16_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR &= ~(FLASH_PECR_FTDW);
	
	*ptr = (uint16_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

void EEPROM_8_fast_write(uint32_t ADDRESS, uint8_t DATA)
{
	// Page 24
	uint8_t *ptr;
	ptr = (uint8_t *)(EEPROM_BASE + ADDRESS);
	
	__disable_irq();
	EEPROM_unlock();
	FLASH->PECR &= ~(FLASH_PECR_FTDW);
	
	*ptr = (uint8_t)DATA;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->PECR |= FLASH_PECR_PELOCK;
	__enable_irq();
}

uint64_t EEPROM_64_read(uint32_t ADDRESS)
{
	uint64_t *ptr;
	ptr = (uint64_t *)(EEPROM_BASE + ADDRESS);
	
	return *ptr;
}

uint32_t EEPROM_32_read(uint32_t ADDRESS)
{
	uint32_t *ptr;
	ptr = (uint32_t *)(EEPROM_BASE + ADDRESS);
	
	return *ptr;
}

uint16_t EEPROM_16_read(uint32_t ADDRESS)
{
	uint16_t *ptr;
	ptr = (uint16_t *)(EEPROM_BASE + ADDRESS);
	
	return *ptr;
}

uint8_t EEPROM_8_read(uint32_t ADDRESS)
{
	uint8_t *ptr;
	ptr = (uint8_t *)(EEPROM_BASE + ADDRESS);
	
	return *ptr;
}