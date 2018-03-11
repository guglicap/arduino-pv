#include "inverter.h"

Inverter::Inverter(Stream* conn, uint16_t addr) {
	_conn = conn;
	_addr = addr;
}

void Inverter::send(Frame frm, bool useFrameSrc) {
	if (frm._cmd == CMD_ERR) {
#if SUNEZY_DEBUG
		__debug(F("error frame, returning"));
#endif
		return;
	}
	uint8_t buf[MAX_SIZE];
	if (!useFrameSrc) {
		frm._src = _addr;
	}
	uint8_t len = frm.bytes(buf);
	if (len == 0) {
#if SUNEZY_DEBUG
		__debug(F("error converting Frame to bytes, returning"));
#endif
		return;
	}
#if SUNEZY_DEBUG
	__debug(F("sending frame"));
#endif
	_conn -> write(buf, len);
}

Frame Inverter::receive() {
	long start = millis();
#if SUNEZY_DEBUG
	__debug(F("receiving frame"));
#endif
	uint8_t buf[MAX_SIZE];
	uint8_t i = 0;
	while (millis() - start < RECV_TIMEOUT) {
		if (_conn -> available() > 0) {
			buf[i++] = _conn -> read();
		}
		if (i > 8 && i == buf[8] + 11) {
#if SUNEZY_DEBUG
			__debug(F("received frame ok, parsing..."));
#endif
			return parseFrame(buf, buf[8] + 11);
		}
	}
#if SUNEZY_DEBUG
	__debug(F("timed out while receiving frame"));
#endif
	return Frame(CMD_ERR);		
}

void Inverter::reset() {
#if SUNEZY_DEBUG
	__debug(F("sending reset"));
#endif
	send(Frame(CMD_RST));
}

uint8_t Inverter::discover(char* buf) {
#if SUNEZY_DEBUG
	__debug(F("sending discover"));
#endif
	Frame f(CMD_DSC);
	send(f);
	f = receive();
	if (f._cmd != CMD_DSC_R) {
#if SUNEZY_DEBUG
		char dbgMsg[50];
		snprintf(dbgMsg, 50, "frame cmd equals %02x:%02x", f._cmd >> 8, f._cmd & 0xff);
		__debug(dbgMsg);
		__debug(F("invalid response frame, returning"));
#endif
		return 0;
	}
	memcpy(buf, f._payload, f._ploadLen);
	return f._ploadLen;
}

bool Inverter::begin(char* sn, uint8_t snLen, uint16_t addr) {
	uint8_t buf[50];	
	if (snLen > 50 - 2) {
#if SUNEZY_DEBUG
		__debug(F("serial number is too long, returning"));
#endif
		return false;
	}
	for (int i = 0; i < snLen; i++) {
		buf[i] = sn[i];
	}
	buf[snLen] = addr >> 8;
	buf[snLen + 1] = addr & 0xff;
	Frame f(CMD_REG, buf, snLen + 2);
	send(f);
	f = receive();
	return f._cmd == CMD_REG_R;
}

uint8_t Inverter::version(char* buf, uint16_t dst) {
	Frame f(CMD_VER);
	f._dst = dst;
	send(f);
	f = receive();
	if (f._cmd != CMD_VER_R) {
		return 0;
	}
	memcpy(buf, f._payload, f._ploadLen);
	return f._ploadLen;	
}
