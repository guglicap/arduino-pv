#include "status.h"
#include "debug.h"
InverterStatus::InverterStatus() {
	temp = 0;
	vpv = 0;
	iac = 0;
	vac = 0;
	fac = 0;
	pac = 0;
	zac = 0;
	etoday = 0;
	etotal = 0;
	htotal = 0;
	mode = 0;
	error = 0;	
}

void interpretData(InverterStatus* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {
	char* le = layout + layoutLen;
	uint8_t* de = data + dataLen;
	while (layout < le && data < de) {				
		uint16_t value = *(data) << 8 | *(data+1);
		uint16_t code = *layout;
		bool b = status->parseLayoutElement(code, value);
		if (!b) {
			code = code << 8 | *(layout+1);
			b = status->parseLayoutElement(code, value);
		}
#if SUNEZY_DEBUG
		if (!b) {
			Serial.print("@");
			Serial.print(__func__);
			Serial.print("\t");
			Serial.print("unknown code: 0x");
			Serial.print(*layout, HEX);
			Serial.print(" or 0x");
			Serial.print(code, HEX);
			Serial.print(" value: ");
			Serial.println(value);
		}
#endif
		if (!b) {
			code = 0; // only skip one char if the code is unknown 
		}
		layout += (code < 0xff ? 1 : 2);
		data += 2;
	}
}

bool InverterStatus::parseLayoutElement(uint16_t code, uint16_t value) {
	switch (code)
	{
		// put 2-byte codes first
		case 0x4748:
			etotal = value;
			break;

		case 0x494a:
			htotal = value;
			break;

		case 0x7e7f:
			error = value;
			break;

		case 0x00:
			temp = value;
			break;
			
		case 0x0d:
			etoday = value;
			break;

		case 0x40:
			vpv = value;
			break;

		case 0x41:
			iac = value;
			break;

		case 0x42:
			vac = value;
			break;

		case 0x43:
			fac = value;
			break;

		case 0x44:
			pac = value;
			break;

		case 0x45:
			zac = value;
			break;

		case 0x4c:
			mode = value;
			break;

		default:
			return false;
	}
	return true;
}
