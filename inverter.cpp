#include "inverter.h"

Inverter::Inverter(Stream* conn, uint16_t addr) {
	_conn = conn;
	_addr = addr;
}

Inverter::Inverter(uint16_t addr) {
	_conn = &Serial;
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
	__debug(ploadLen);
	while (_conn -> available() < ploadLen + 2) {
		if (millis() - start > RECV_TIMEOUT) {
			__debug("timed out while receiving frame, returning");
			return Frame(CMD_ERR);
		}
	}
	for (int i = 0; i < ploadLen + 2; i++) {
		buf[8+i] = _conn -> read();
	}
	__debug("read payload, ok, parsing frame");
	return parseFrame(buf, 11 + ploadLen);
}

void Inverter::reset() {
	__debug("sending reset");
	send(Frame(CMD_RST));
}

String Inverter::discover() {
	__debug("sending discover");
	send(Frame(CMD_DSC));
	Frame f = receive();
	if (f._cmd != CMD_REG_R) {
		__debug("invalid response frame, returning");
		return "";
	}
	return String((char*) f._payload);
}

bool Inverter::begin(String serial, uint16_t addr) {
	uint8_t buf[MAX_PLOAD_SIZE];
	uint8_t* sn = serial.c_str();
	uint16_t snLen = serial.length(); //length excludes the null terminator
	if (snLen > MAX_PLOAD_SIZE - 2) {
		__debug("serial number is too long, returning");
		return false;
	}
	memcpy(buf, sn, snLen); //so we're only copying the string characters
	buf[snLen] = addr & 0xff;
	buf[snLen + 1] = addr >> 8;
	send(Frame(CMD_REG, buf, sizeof(sn) + 2));
	Frame f = receive();
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
