#include "frame.h"

#ifndef __b2u16
#define __b2u16(a, b) a<<8 | b
#endif

Frame::Frame(uint16_t cmd, uint8_t* payload, uint8_t ploadLen, uint16_t src, uint16_t dst) {
	_cmd = cmd;
	_ploadLen = ploadLen;
	_payload = payload;
	_src = src;
	_dst = dst;
}

Frame parseFrame(uint8_t data[]) {
	// the frame format is as follows
	// <sync> <src> <dst> <cmd> <len> <payload> <checksum>
	// 2b     2b    2b    2b    1b    len b     2b
	// thus, the frame cannot be shorter than 11 bytes
	unsigned int frameLen = sizeof(data);
	if (frameLen < 11) {
		return Frame(CMD_ERR);
	}
	// the received one is big-endian, we want ours to be little endian
	uint16_t recvChecksum = __b2u16(data[frameLen - 1], data[frameLen - 2]);
	if (checksum(data, frameLen - 2) != recvChecksum) {
		return Frame(CMD_ERR);
	}
	uint16_t preamble = __b2u16(data[1], data[0]);
	if (preamble != SYNC) {
		return Frame(CMD_ERR);
	}	
	uint16_t src = __b2u16(data[3], data[2]);
	uint16_t dst = __b2u16(data[5], data[4]);
	uint16_t cmd = __b2u16(data[7], data[6]);
	uint8_t payloadLen = data[8];
	if (payloadLen > MAX_PLOAD_SIZE) {
		return Frame(CMD_ERR);
	}
	uint8_t payload[MAX_PLOAD_SIZE];
	for (int i = 0; i < payloadLen; i++) {
		if (i >= frameLen - 2) {
			break;
		}
		payload[i] = data[i];
	}
	return Frame(cmd, payloadLen, payload, src, dst);
}

uint16_t checksum(uint8_t data[], int len) {
	uint16_t result = 0;
	for (int i = 0; i < len; i++) {
		result += data[i];
	}
	return result;
}

uint8_t* Frame::bytes() {
	uint8_t bytes[MAX_SIZE];
	bytes[0] = SYNC & 0xff; // low byte
	bytes[1] = SYNC >> 8; //high byte
	bytes[2] = _src & 0xff;
	bytes[3] = _src >> 8;
	bytes[4] = _dst & 0xff;
	bytes[5] = _dst >> 8;
	bytes[6] = _cmd & 0xff;
	bytes[7] = _cmd >> 8;
	bytes[8] = _ploadLen;
	if (_ploadLen > 0 && ! _payload) {
		return nullptr;
	}
	for (int i = 0; i < _ploadLen; i++) {
		bytes[8 + i] = _payload[i];
	}
	uint16_t _checksum = checksum(bytes, 9 + _ploadLen);
	bytes[9 + _ploadLen] = _checksum & 0xff;
	bytes[10 + _ploadLen] = _checksum >> 8;
	return bytes;
}
