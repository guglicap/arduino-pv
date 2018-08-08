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
	err_gv = 0;
    err_gf = 0;
    err_gz = 0;
    err_temp = 0;
    err_pv1 = 0;
    err_gfc1 = 0;
}

void interpretData(InverterStatus* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {
  char* le = layout + layoutLen;
  uint8_t* de = data + dataLen;
  while (layout < le && data < de) {
    uint16_t value = *data << 8 | *(data + 1);
    uint16_t code = *layout;
    bool b = status->parseLayoutElement(code, value);
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
    layout += 1;
    data += 2;
  }
}

bool InverterStatus::parseLayoutElement(uint16_t code, uint16_t value) {
	switch (code)
	{
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

		case 0x47: //ETOTALH
			etotal = value << 8;
			break;

		case 0x48: //ETOTALL
			etotal += value;
			break;

		case 0x49: //HTOTALH
			htotal = value << 8;
			break;

		case 0x4a: //HTOTALL
			htotal += value;
			break;

		case 0x4c:
			mode = value;
			break;

		case 0x78:
			err_gv = value;
			break;

		case 0x79:
			err_gf = value;
			break;

		case 0x7a:
			err_gz = value;
			break;

		case 0x7b:
			err_temp = value;
			break;

		case 0x7c:
			err_pv1 = value;
			break;

		case 0x7d:
			err_gfc1 = value;
			break;

		case 0x7e:
			error = value;
			break;

		default:
			return false;
	}
	return true;
}
