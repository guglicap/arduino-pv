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
   0x4c, 1, "Mode"				Operating Mode 0=Wait 1=Normal 2=Fault 3=Permenant Fault
   0x7e7f, 1, "Error"	

			# The 2 error bytes are bit fields, e.g. ERROR[16] = 0x0100
			 0: ('The GFCI detection circucit is abnormal', 'GFCI ckt fails'),
			 1: ('The DC output sensor is abnormal', 'DC sensor fails'),
			 2: ('The 2.5V reference inside is abnormal', 'Ref 2.5V fails'),
			 3: ('Different measurements between Master and Slave for output DC current', 'DC inj. differs for M-S'),
			 4: ('Different measurements between Master and Slave for GFCI', 'Ground I differs for M-S'),
			 5: ('DC Bus voltage is too low', 'Bus-Low-Fail'),
			 6: ('DC Bus voltage is too High', 'Bus-High-Fail'),
			 7: ('Device Fault', 'Device-Fault'),
			 8: ('Delta GridZ is too high', 'Delta Z high'),
			 9: ('No grid voltage detected', 'No-Utility'),
			10: ('Ground current is too high', 'Ground I high'),
			11: ('DC bus is not correct', 'DC BUS fails'),
			12: ('Master and Slave firmware version is unmatch', 'M-S Version Fail'),
			13: ('Internal temperature is high', 'Temperature high'),
			14: ('AutoTest failed', 'Test Fail'),
			15: ('PV voltage is too high', 'Vpv high'),
			16: ('Fan Lock', 'FanLock-Warning'),
			17: ('The measured AC voltage is out of tolerable range', 'Vac out of range'),
			18: ('Isulation resistance of PV to earth is too low', 'PV insulation low'),
			19: ('The DC injection to grid is too high', 'DC injection high'),
			20: ('Different measurements between Master and Slave for dl, Fac, Uac or Zac', 'Fac,Zac,Vac differs for M-S'),
			21: ('Different measurements between Master and Slave for grid impedance', 'Zac differs for M-S'),
			22: ('Different measurements between Master and Slave for grid frequency', 'Fac differs for M-S'),
			23: ('Different measurements between Master and Slave for grid voltage', 'Vac differs for M-S'),
			24: ('Memory space is full', 'MemFull-Warning'),
			25: ('Test of output AC relay fails', 'AC relay fails'),
			26: ('The slave impedance is out of tolerable range', 'Zac-Slave out of range'),
			27: ('The measured AC impedance is out of range', 'Zac-Master out of range'),
			28: ('The slave frequency is out of tolerable range', 'Fac-Slave out of range'),
			29: ('The master frequency is out of tolerable range', 'Fac-Master out of range'),
			30: ('EEPROM reading or writing error', 'EEPROM fails'),
			31: ('Communication between microcontrollers fails', 'Comm fails between M-S'),
   
   */

class InverterStatus {
	public:
		InverterStatus();
		uint32_t temp,
			 vpv,
			 iac,
			 vac,
			 fac,
			 pac,
			 zac,
			 etoday,
			 etotal,
			 htotal,
			 mode,
			 error,
			 err_gv,
             err_gf,
             err_gz,
             err_temp,
             err_pv1,
             err_gfc1;
		bool parseLayoutElement(uint16_t code, uint16_t value);
};

void interpretData(InverterStatus* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen);
#endif
