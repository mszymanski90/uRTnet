/*
 *
 *  inc/hal/ethernet.h - Hardware abstraction layer- ethernet module
 *
 *  Copyright (C) 2012  	Maciej Szymañski <mszymanski90@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "inc/hal/ethernet.h"

extern uRTnet					uRTnet_module;
MAC_addr_t 						uC_MAC_Address;

void ethernet_init(void)
{
	uint32_t 			ul_temp;
	uint32_t			ul_user1;
	uint32_t			ul_user2;


	// Enable and Reset the Ethernet Controller.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	// Enable Port F for Ethernet LEDs.
	//  LED0        Bit 3   Output
	//  LED1        Bit 2   Output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

	// Intialize the Ethernet Controller and disable all Ethernet Controller
	// interrupt sources.
	EthernetIntDisable(ETH_BASE, (ETH_INT_PHY | ETH_INT_MDIO | ETH_INT_RXER |
						   ETH_INT_RXOF | ETH_INT_TX | ETH_INT_TXER | ETH_INT_RX));
	ul_temp = EthernetIntStatus(ETH_BASE, false);
	EthernetIntClear(ETH_BASE, ul_temp);

	// Initialize the Ethernet Controller for operation.
	EthernetInitExpClk(ETH_BASE, SysCtlClockGet());

	EthernetConfigSet(ETH_BASE, (ETH_CFG_TX_DPLXEN | ETH_CFG_TX_CRCEN |
								 ETH_CFG_TX_PADEN));

	// Wait for the link to become active.
	ul_temp = EthernetPHYRead(ETH_BASE, PHY_MR1);
	while((ul_temp & 0x0004) == 0)
	{
		ul_temp = EthernetPHYRead(ETH_BASE, PHY_MR1);
	}

	// Enable the Ethernet Controller.
	EthernetEnable(ETH_BASE);

	// Enable the Ethernet interrupt.
	IntEnable(INT_ETH);

	// Enable the Ethernet RX Packet interrupt source.
	EthernetIntEnable(ETH_BASE, ETH_INT_RX);


	FlashUserGet(&ul_user1, &ul_user2);
	if((ul_user1 == 0xffffffff) || (ul_user2 == 0xffffffff))
	{
		//
		// We should never get here.  This is an error if the MAC address has
		// not been programmed into the device.  Exit the program.
		//
		while(1)
		{
		}
	}

	// Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
	// address needed to program the hardware registers, then program the MAC
	// address into the Ethernet Controller registers.
	uC_MAC_Address.addr[0] = ((ul_user1 >>  0) & 0xff);
	uC_MAC_Address.addr[1] = ((ul_user1 >>  8) & 0xff);
	uC_MAC_Address.addr[2] = ((ul_user1 >> 16) & 0xff);
	uC_MAC_Address.addr[3] = ((ul_user2 >>  0) & 0xff);
	uC_MAC_Address.addr[4] = ((ul_user2 >>  8) & 0xff);
	uC_MAC_Address.addr[5] = ((ul_user2 >> 16) & 0xff);

	// Program the hardware with it's MAC address (for filtering).
	EthernetMACAddrSet(ETH_BASE, uC_MAC_Address.addr);
}

uint32_t ethernet_receive_packet(uint8_t* buffer)
{
	return EthernetPacketGetNonBlocking(ETH_BASE, buffer, CONF_eth_buffer_LENGTH);
}

void ethernet_send_packet(uint8_t* tx_buffer, int32_t data_size)
{
	EthernetPacketPutNonBlocking(ETH_BASE, tx_buffer, data_size);
}

void ethernet_get_mac_addr(MAC_addr_t* target)
{
	target->addr[0] = uC_MAC_Address.addr[0];
	target->addr[1] = uC_MAC_Address.addr[1];
	target->addr[2] = uC_MAC_Address.addr[2];
	target->addr[3] = uC_MAC_Address.addr[3];
	target->addr[4] = uC_MAC_Address.addr[4];
	target->addr[5] = uC_MAC_Address.addr[5];
}

void ethernet_int_handler(void)
{
    uint32_t temp;

    // clear interrupt
    temp = EthernetIntStatus(ETH_BASE, false);
    EthernetIntClear(ETH_BASE, temp);

    // if new frame arrived
    if(temp & ETH_INT_RX)
    {
    	rtmac_on_rx_frame(&uRTnet_module);
    }
}
