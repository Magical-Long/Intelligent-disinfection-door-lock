#ifndef __FLASH_H__
#define __FLASH_H__

#define FLASH_ADDR1 0x0800c000
#define FLASH_ADDR2 0x0800c400
#define FLASH_ADDR3 0x0800c800
#define FLASH_ADDR4 0x0800cc00

void FLASH_W(u32 add,u8 dat1,u8 dat2,u8 dat3,u8 dat4);
u16 FLASH_R(u32 add);
void FLASH_Clear(u32 add);

#endif

