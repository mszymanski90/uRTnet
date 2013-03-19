/*
 * rtnet_config.h
 *
 *  Created on: 13-09-2012
 *      Author: Maciej
 */

#ifndef RTNET_CONFIG_H_
#define RTNET_CONFIG_H_

/* RTnet settings */

#define CONF_masters_MAC_0 0x00;
#define CONF_masters_MAC_1 0x1b;
#define CONF_masters_MAC_2 0x11;
#define CONF_masters_MAC_3 0x63;
#define CONF_masters_MAC_4 0x37;
#define CONF_masters_MAC_5 0xa3;

#define CONF_eth_buffer_LENGTH 	1600

#define CONF_max_slots_tab_size	 100
#define CONF_cycle_length		5000
#define CONF_slot_offset		 200

#define CONF_reply_cycle_offset	   1
#define CONF_reply_slot_offset     1

#define CONF_max_initialization_cycles 3
#define CONF_max_calibration_cycles 100

/* End of RTnet settings */
/* do NOT modify next section */

#define CONST_slot_offset_ns CONF_slot_offset*1000;

#endif /* RTNET_CONFIG_H_ */
