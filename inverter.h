#ifndef CMS2000_h
#define CMS2000_h

#include "Arduino.h"
#include "stdint.h"
#include "SoftwareSerial.h"
#include "frame.h"

class Inverter 
{
	public:
		Inverter(SoftwareSerial& conn, uint16_t addr = ADDR_HOST);
		explicit Inverter(uint16_t addr = ADDR_HOST);
		Frame receive();
		void send(Frame* frm);
		void reset();
		String discover();
		bool begin(String serial, uint16_t addr = ADDR_DEV);
		String version(uint16_t dst = ADDR_DEV);
	private:
		Print& _output;
		Stream& _input;
		uint16_t _addr;
};
#endif
