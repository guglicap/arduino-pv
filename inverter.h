#ifndef SUNEZY_h
#define SUNEZY_h

#define RECV_TIMEOUT 5000 //ms

#include "Arduino.h"
#include "debug.h"
#include "stdint.h"
#include "SoftwareSerial.h"
#include "frame.h"
#include "status.h"

class Inverter 
{
	public:
		explicit Inverter(Stream& conn = Serial, uint16_t addr = ADDR_HOST);
		Frame receive();
		void send(Frame frm, bool useFrameSrc = false);
		void reset();
		String discover(); // returns serial number of connected inverter
		bool begin(String& sn, uint16_t addr = ADDR_DEV);
		String version(uint16_t dst = ADDR_DEV);
		String statLayout(uint16_t dst = ADDR_DEV);
		String paramLayout(uint16_t dst = ADDR_DEV);
		bool status(InverterStatus& status, String& layout, uint16_t dst = ADDR_DEV);
	private:
        char _buf[MAX_SIZE];
		Stream& _conn;
		uint16_t _addr;
};
#endif
