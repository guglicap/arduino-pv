#ifndef SUNEZY_INVERTER_STATUS
#define SUNEZY_INVERTER_STATUS
#include "stdint.h"

/*
   code, divisor, name

   0x00, 10, "Temp-inv" 		Internal Temperature - deg C
   0x0d, 10, "etoday"			Accumulated Energy Today - KiloWattHours
   0x01, 10, "Vpv1" :ignored 
   0x02, 10, "Vpv2" :ignored
   0x03, 10, "Vpv3" :ignored
   0x04, 10, "Ipv1" :ignored
   0x05, 10, "Ipv2" :ignored
   0x06, 10, "Ipv3" :ignored
   0x40, 10, "Vpv"				Panel Voltage - Volt
   0x41, 10, "Iac"				Grid Current - Ampere
   0x42, 10, "Vac"				Grid Voltage - Volt
   0x43, 100, "Fac"				Grid Frequency - Hertz
   0x44, 1, "Pac"				Output Power - Watt
   0x45, 1, "Zac"				Grid Impedance - milliOhms
   0x4748, 10, "E-Total"		Accumulated Energy - kiloWattHours
   0x494a, 1, "h-Total"			Working Hours - Hours
   0x4c, 1, "Mode"				Operating Mode
   0x7e7f, 1, "Error"
   */

class InverterStatus {
	public:
		InverterStatus();
		uint16_t temp,
			 vpv,
			 ipv,
			 iac,
			 vac,
			 fac,
			 pac,
			 zac,
			 etoday,
			 etotal,
			 htotal,
			 mode,
			 error;
		bool parseLayoutElement(uint16_t code, uint16_t value);
};

void interpretData(InverterStatus* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen);
#endif
