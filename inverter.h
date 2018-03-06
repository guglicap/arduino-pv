#ifndef CMS2000_h
#define CMS2000_h

#include "Arduino.h"
#include "stdint.h"
#include "SoftwareSerial.h"
#include "frame.h"

class Inverter 
{
	public:
		Inverter(int rxPin, int txPin, uint16_t addr);
		Frame receive();
		void send(Frame* frm);
		void reset();
		String discover();
		bool begin(String serial, uint16_t addr = ADDR_DEV);
		String version(uint16_t dst = ADDR_DEV);
	private:
		SoftwareSerial& _conn;
		uint16_t _addr;
};
#endif
