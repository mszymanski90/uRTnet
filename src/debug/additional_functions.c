#include "debug/additional_functions.h"

void DisplayHexNumber(uint8_t number, uint32_t ulCol, uint32_t ulRow)
{
    int8_t pucBuf[8];

    usprintf(pucBuf, "%2x", number);

    RIT128x96x4StringDraw(pucBuf, ulCol, ulRow, 15);
}

void DisplayDecNumber(uint32_t number, uint32_t ulCol, uint32_t ulRow)
{
    int8_t pucBuf[16];

    usprintf(pucBuf, "%2u", number);

    RIT128x96x4StringDraw(pucBuf, ulCol, ulRow, 15);
}

void DisplayULLNumber(uint64_t number, uint32_t ulCol, uint32_t ulRow)
{
    int8_t pucBuf[21];

    usprintf(pucBuf, "%21u", number);

    RIT128x96x4StringDraw(pucBuf, ulCol, ulRow, 15);
}

void DisplayMACAddress(MAC_addr_t *data, uint8_t ulCol, uint8_t ulRow)
{
	int16_t i;
	int8_t buf[4];

	for(i=0; i<6; i++)
	{
		usprintf(buf, "%2x ", data->addr[i]);
		RIT128x96x4StringDraw(buf, ulCol + 16*i, ulRow, 15);
	}
}
