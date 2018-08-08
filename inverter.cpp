#include "inverter.h"

Inverter::Inverter(Stream& conn, uint16_t addr) {
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
	if (!useFrameSrc) {
		frm._src = _addr;
	}
	uint8_t len = frm.bytes(_buf);
	if (len == 0) {
#if SUNEZY_DEBUG
		__debug(F("error converting Frame to bytes, returning"));
#endif
		return;
	}
#if SUNEZY_DEBUG
	__debug(F("sending frame"));
#endif
	_conn.write(_buf, len);
}

Frame Inverter::receive() {
	long start = millis();
#if SUNEZY_DEBUG
	__debug(F("receiving frame"));
#endif
	uint8_t i = 0;
	while (millis() - start < RECV_TIMEOUT) {
		yield();
		if (_conn.available() > 0) {
			_buf[i++] = _conn.read();
		}
		if (i > 8 && i == _buf[8] + 11) {
#if SUNEZY_DEBUG
			__debug(F("received frame ok, parsing..."));
#endif
			return parseFrame(_buf, _buf[8] + 11);
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

String Inverter::discover() {
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
		return "";
	}
	memcpy(_buf, f._payload, f._ploadLen);
	_buf[f._ploadLen] = '\0';
	return String(_buf);
}

bool Inverter::begin(String& sn, uint16_t addr) {
    uint16_t len = sn.length();
	memcpy(_buf, sn.c_str(), len);
	_buf[len] = addr >> 8;
	_buf[len + 1] = addr & 0xff;
	Frame f(CMD_REG, _buf, len + 2);
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
	memcpy(_buf, f._payload, f._ploadLen);
	_buf[f._ploadLen] = '\0';
	return String(_buf);
}

String Inverter::statLayout(uint16_t dst) {
	Frame f(CMD_STL);
	f._dst = dst;
	send(f);
	f = receive();
	if (f._cmd != CMD_STL_R) {
		return "";
	}
	memcpy(_buf, f._payload, f._ploadLen);
	return String(_buf);
}

String Inverter::paramLayout(uint16_t dst) {
	Frame f(CMD_PRL);
	f._dst = dst;
	send(f);
	f = receive();
	if (f._cmd != CMD_PRL_R) {
		return "";
	}
	memcpy(_buf, f._payload, f._ploadLen);
	return String(_buf);
}

bool Inverter::status(InverterStatus& status, String& layout, uint16_t dst) {
	Frame f(CMD_STA);
	f._dst = dst;
	send(f);
	f = receive();
	if (f._cmd != CMD_STA_R) {
		return false;
	}
	interpretData(status, layout, layoutLen, f._payload, f._ploadLen);
	return true;
}
