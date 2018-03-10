#include "inverter.h"

Inverter::Inverter(Stream* conn, uint16_t addr) {
	_conn = conn;
	_addr = addr;
}

void Inverter::send(Frame frm, bool useFrameSrc) {
	if (frm._cmd == CMD_ERR) {
		__debug("error frame, returning");
		return;
	}
	uint8_t buf[MAX_SIZE];
	if (!useFrameSrc) {
		frm._src = _addr;
	}
	uint8_t len = frm.bytes(buf);
	if (len == 0) {
		__debug("error converting Frame to bytes, returning");
		return;
	}
	__debug("sending frame");
	for (int i = 0; i < len; i++) {
		Serial.print(buf[i], HEX);
		Serial.print('-');
	}
	Serial.println();
	_conn -> write(buf, len);
}

Frame Inverter::receive() {
	long start = millis();
	__debug("receiving frame");
	// wait until we have enough data to make a valid frame
	while (_conn -> available() < 9) {
		if (millis() - start > RECV_TIMEOUT) {
			__debug("timed out while receiving frame, returning");
			return Frame(CMD_ERR);
		}
	}
	uint8_t buf[MAX_SIZE];
	// read up to payload
	for (int i = 0; i < 9; i++) {
		buf[i] = _conn -> read();
	}
	__debug("read up to payload, ok");
	uint8_t ploadLen = buf[8];
	// read payload + checksum
	char dbgMsg[50];
	snprintf(dbgMsg, 50, "ploadLen equals 0x%02x", ploadLen);
	__debug(dbgMsg);
	while (_conn -> available() < ploadLen + 2) {
		if (millis() - start > RECV_TIMEOUT) {
			__debug("timed out while receiving frame, returning");
			return Frame(CMD_ERR);
		}
	}
	for (int i = 0; i < ploadLen + 2; i++) {
		buf[9+i] = _conn -> read();
	}
	__debug("read payload, ok, parsing frame");
	return parseFrame(buf, 11 + ploadLen);
}

void Inverter::reset() {
	__debug("sending reset");
	send(Frame(CMD_RST));
}

uint8_t Inverter::discover(uint8_t* buf) {
	__debug("sending discover");
	Frame f(CMD_DSC);
	send(f);
	f = receive();
	if (f._cmd != CMD_DSC_R) {
		char dbgMsg[50];
		snprintf(dbgMsg, 50, "frame cmd equals %02x:%02x", f._cmd >> 8, f._cmd & 0xff);
		__debug(dbgMsg);
		__debug("invalid response frame, returning");
		return 0;
	}
	memcpy(buf, f._payload, f._ploadLen);
	return f._ploadLen;
}

bool Inverter::begin(uint8_t* sn, uint8_t snLen, uint16_t addr) {
	uint8_t buf[50];	
	if (snLen > 50 - 2) {
		__debug("serial number is too long, returning");
		return false;
	}
	for (int i = 0; i < snLen; i++) {
		buf[i] = sn[i];
	}
	buf[snLen] = addr & 0xff;
	buf[snLen + 1] = addr >> 8;
	Frame f(CMD_REG, buf, snLen + 2);
	send(f);
	f = receive();
	return f._cmd == CMD_REG_R;
}

String Inverter::version(uint16_t dst) {
	Frame f(CMD_VER);
	f._dst = dst;
	send(f);
	f = receive();
	if (f._cmd != CMD_VER_R) {
		return "";
	}
	return String((char*) f._payload);	
}
