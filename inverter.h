#ifndef SUNEZY_h
#define SUNEZY_h

#define RECV_TIMEOUT 5000 //ms

#include "Arduino.h"
#include "debug.h"
#include "stdint.h"
#include "SoftwareSerial.h"
#include "frame.h"

class Inverter 
{
	public:
		Inverter(Stream* conn, uint16_t addr = ADDR_HOST);
		explicit Inverter(uint16_t addr = ADDR_HOST);
		Frame receive();
		void send(Frame frm);
		void reset();
		String discover();
		bool begin(String serial, uint16_t addr = ADDR_DEV);
		String version(uint16_t dst = ADDR_DEV);
	private:
		Stream* _conn;
		uint16_t _addr;
};
#endif
