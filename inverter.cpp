#include "inverter.h"

Inverter::Inverter(SoftwareSerial& conn, uint16_t addr) {
	_output = conn;
	_input = conn;
	_addr = addr;
}

Inverter::Inverter(uint16_t addr) {
	_output = Serial;
	_input = Serial;
	_addr = addr;
}

void Inverter::send(Frame* frm) {
	if (!frm) {
		return;
	}
	uint8_t* bytes = frm -> bytes();
	_output.write(bytes, sizeof(bytes));
	return;
}

Frame Inverter::receive() {
	long start = millis();
	// wait until we have enough data to make a valid frame
	while (_input.available() < 11) {
		if (millis() - start > RECV_TIMEOUT) {
			Frame f(CMD_ERR);
			f._payload = "timed out while receiving frame";
			return f;
		}
	}
	uint8_t buf[MAX_SIZE];
	// read up to payload
	for (int i = 0; i < 9; i++) {
		buf[i] = _input.read();
	}
	uint8_t ploadLen = buf[8];
	// read payload + checksum
	for (int i = 0; i < ploadLen + 2; i++) {
		buf[8+i] = _input.read();
	}
	return parseFrame(buf);
}

void Inverter::reset() {
	send(&Frame(CMD_RST));
}

String Inverter::discover() {
	send(&Frame(CMD_DSC));
	Frame f = receive();
	if (f._cmd != CMD_REG_R) {
		return "";
	}
	return String((char*) f._payload);
}

bool Inverter::begin(String serial, uint16_t addr) {
	uint8_t buf[MAX_PLOAD_SIZE];
	uint8_t* sn = serial.c_str();
	memcpy(buf, sn, sizeof(sn) - 1);
	buf[sizeof(sn) - 1] = addr & 0xff; // leaving null terminator out
	buf[sizeof(sn)] = addr >> 8;
	send(&Frame(CMD_REG, buf, sizeof(sn) + 2));
	Frame f = receive();
	return f._cmd == CMD_REG_R;
}

String Inverter::version(uint16_t dst) {
	send(&Frame(CMD_VER, dst = dst));
	Frame f = receive();
	if (f._cmd != CMD_VER_R) {
		return "";
	}
	return String((char*) f._payload);	
}
