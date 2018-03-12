#ifndef SUNEZY_h
#define SUNEZY_h

#define RECV_TIMEOUT 5000 //ms

#include "Arduino.h"
#include "debug.h"
#include "stdint.h"
#include "SoftwareSerial.h"
#include "frame.h"
#include "protocol.h"

class Inverter 
{
	public:
		explicit Inverter(Stream* conn = &Serial, uint16_t addr = ADDR_HOST);
		Frame receive();
		void send(Frame frm, bool useFrameSrc = false);
		void reset();
		char* discover(char* buf);
		bool begin(char* sn, uint16_t addr = ADDR_DEV);
		char* version(char* ver, uint16_t dst = ADDR_DEV);
		char* statLayout(char* buf, uint16_t dst = ADDR_DEV);
		char* paramLayout(char* buf, uint16_t dst = ADDR_DEV);
		uint8_t status(StatusElem* status, char* layout, uint16_t dst = ADDR_DEV);
	private:
		Stream* _conn;
		uint16_t _addr;
};
#endif
