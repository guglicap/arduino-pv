#ifndef FRAME_H
#define FRAME_H

#include "stdint.h"
#include "debug.h"

#define	MAX_SIZE 		256
#define MAX_PLOAD_SIZE		MAX_SIZE - 11
#define	SYNC 			0xaaaa		// 2 sync bytes preamble "1010101010101010"
#define	ADDR_DEFAULT 	0x0000		// Broadcast address
#define	ADDR_HOST 		0x0100		// Default host address
#define	ADDR_DEV 		0x0001		// Default device address
#define	CMD_DSC 	0x0000		// Computer discovers devices
#define	CMD_DSC_R 	0x0080		// Inverter advertises its serial number
#define	CMD_REG 	0x0001		// Address registration
#define	CMD_REG_R 	0x0081		// Acknowledge the assigned address
#define	CMD_RMV 	0x0002		// Disconnect
#define	CMD_RMV_R 	0x0082		// Disconnect ACK
#define	CMD_RCT 	0x0003		// Reconnect all devices
#define	CMD_RST 	0x0004		// Reset communications
#define	CMD_STL 	0x0100		// Status frame structure request
#define	CMD_STL_R 	0x0180		// Status frame structure reply
#define	CMD_PRL 	0x0101		// Parameter frame structure request
#define	CMD_PRL_R 	0x0181		// Parameter frame structure reply
#define	CMD_STA 	0x0102		// Status request
#define	CMD_STA_R 	0x0182		// Status reply
#define	CMD_VER 	0x0103		// Version string request
#define	CMD_VER_R 	0x0183		// Version string reply
#define	CMD_PRM 	0x0104		// Parameter request
#define	CMD_PRM_R 	0x0184		// Parameters reply
#define	CMD_SP0 	0x0200		// Set Vpv-Start
#define	CMD_SP0_R 	0x0280		// Set Vpv-Start ACK
#define	CMD_SP1 	0x0201		// Set T-Start
#define	CMD_SP1_R 	0x0281		// Set T-Start ACK
#define	CMD_SP2 	0x0204		// Set Vac-Min
#define	CMD_SP2_R 	0x0284		// Set Vac-Min ACK
#define	CMD_SP3 	0x0205		// Set Vac-Max
#define	CMD_SP3_R 	0x0285		// Set Vac-Max ACK
#define	CMD_SP4 	0x0206		// Set Fac-Max
#define	CMD_SP4_R 	0x0286		// Set Fac-Max ACK
#define	CMD_SP5 	0x0207		// Set Fac-Min
#define	CMD_SP5_R 	0x0287		// Set Fac-Min ACK
#define	CMD_SP6 	0x0208		// Set DZac-Max
#define	CMD_SP6_R 	0x0288		// Set DZac-Max ACK
#define	CMD_SP7 	0x0209		// Set DZac
#define	CMD_SP7_R 	0x0289		// Set DZac ACK
#define	CMD_ZRO 	0x0300		// Reset inverter E-Total and h-Total
#define CMD_ERR		0xFFAA		// Used to signal an error in frame processing
#define	CMD_ZRO_R 	0x0380		// Reset inverter E-Total and h-Total ACK


class Frame
{
	public:
		explicit Frame(uint16_t cmd, uint8_t* payload = nullptr, uint8_t ploadLen = 0, uint16_t src = ADDR_DEFAULT, uint16_t dst = ADDR_DEFAULT);
		uint8_t bytes(uint8_t* buf);
		uint8_t* _payload;
		uint16_t _cmd;
		uint16_t _src,_dst;
		uint8_t _ploadLen;
};

Frame parseFrame(uint8_t data[], uint16_t len);
uint16_t checksum(uint8_t data[], int len);
#endif
