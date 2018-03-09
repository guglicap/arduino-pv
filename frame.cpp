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

Frame parseFrame(uint8_t data[], uint16_t len) {
	// the frame format is as follows
	// <sync> <src> <dst> <cmd> <len> <payload> <checksum>
	// 2b     2b    2b    2b    1b    len b     2b
	// thus, the frame cannot be shorter than 11 bytes
	if (len < 11) {
		__debug("frame length is less than 11 bytes, returning");
		return Frame(CMD_ERR);
	}
	uint16_t recvChecksum = __b2u16(data[len - 2], data[len - 1]);
	if (checksum(data, len - 2) != recvChecksum) {
		char dbgMsg[256];
		sprintf(dbgMsg, "received checksum and calculated checksum do not match: %x vs %x ,returning", recvChecksum, checksum(data, len - 2));
		__debug(dbgMsg);
		return Frame(CMD_ERR);
	}
	__debug("checksums match, ok");
	uint16_t preamble = __b2u16(data[0], data[1]);
	if (preamble != SYNC) {
		__debug("frame preamble doesn't match SYNC");
		return Frame(CMD_ERR);
	}	
	__debug("frame preamble is SYNC, ok");
	uint16_t src = __b2u16(data[2], data[3]);
	uint16_t dst = __b2u16(data[4], data[5]);
	uint16_t cmd = __b2u16(data[6], data[7]);
	uint8_t ploadLen = data[8];
	if (ploadLen > MAX_SIZE - 11) {
		__debug("length of payload exceeds max size");
		return Frame(CMD_ERR);
	}
	char debugMsg[50];
	sprintf(debugMsg, "payload has length %d", ploadLen);
	__debug(debugMsg);
	uint8_t* payload;
	if (ploadLen > 0) {
		payload = &data[9];
	} else {
		payload = nullptr;
	}
	__debug("assigned payload pointer, ok, returning Frame");
	return Frame(cmd, payload, ploadLen, src, dst);
}

uint16_t checksum(uint8_t data[], int len) {
	uint16_t result = 0;
	for (int i = 0; i < len; i++) {
		result += data[i];
	}
	return result;
}

uint8_t Frame::bytes(uint8_t* buf) {
	buf[0] = SYNC >> 8; // high byte
	buf[1] = SYNC & 0xff; // low byte
	buf[2] = _src >> 8;
	buf[3] = _src & 0xff;
	buf[4] = _dst >> 8;
	buf[5] = _dst & 0xff;
	buf[6] = _cmd >> 8;
	buf[7] = _cmd & 0xff;
	buf[8] = _ploadLen;
	if (_ploadLen > 0 && ! _payload) {
		__debug("specified payload length, but no payload could be found, returning");
		return 0;
	}
	for (int i = 0; i < _ploadLen; i++) {
		buf[8 + i] = _payload[i];
	}
	uint16_t _checksum = checksum(buf, 9 + _ploadLen);
	buf[9 + _ploadLen] = _checksum >> 8;
	buf[10 + _ploadLen] = _checksum & 0xff;
	__debug("converted Frame to bytes, ok, returning bytes");
	return 11 + _ploadLen;
}
