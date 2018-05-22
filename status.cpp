#include "status.h"

void interpretData(InverterStatus* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {		
	while (layout < layout + layoutLen && data < data + dataLen) {				
		uint16_t value = *(data) << 8 | *(data+1);	
		uint16_t code = *layout;
		bool b = status->parseLayoutElement(code, value);
		if (!b) {
			code = code << 8 | *(layout+1);
			b = status->parseLayoutElement(code, value);
		}
		layout += code > 0xff ? 1 : 2;
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
