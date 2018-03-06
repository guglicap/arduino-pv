#include "inverter.h"

Inverter::Inverter(int rxPin, int txPin, uint16_t addr = ADDR_HOST) {
	SoftwareSerial conn(rxPin, txPin);
	_conn = conn;
	_addr = addr;
}

void Inverter::send(Frame* frm) {
	if (!frm) {
		return;
	}
	uint8_t* bytes = frm -> bytes();
	_conn.write(bytes, sizeof(bytes));
	return;
}

Frame Inverter::receive() {
	if (_conn.available() < 11) {
		return Frame(CMD_ERR); //not enough data to make a valid frame
	}
	uint8_t buf[MAX_SIZE];
	// read up to payload
	for (int i = 0; i < 9; i++) {
		buf[i] = _conn.read();
	}
	uint8_t ploadLen = buf[8];
	// read payload + checksum
	for (int i = 0; i < ploadLen + 2; i++) {
		buf[8+i] = _conn.read();
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
