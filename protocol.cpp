#include "protocol.h"

uint8_t interpretData(StatusElem* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {	
	for (int i = 0; i < layoutLen && (2*i) + 1 < dataLen; i++) {
		uint8_t x = 2*i;
		StatusElem* elem = &status[i];
		for (int j = 0; j < sizeof(layoutElems) / sizeof(layoutElem); j++) {
			uint16_t constCode = layoutElems[j].code;
			uint16_t layoutCode;
			if (constCode > 0xff) {
				if (layoutLen < i+1) {
					break;
				}
				layoutCode = data[i] << 8 | data[i+1];
				i++;
			} else {
				layoutCode = data[x];
			}
			if (constCode != layoutCode) {
				continue;
			}
			*elem = StatusElem{ layoutElems[j].name, data[x] << 8 | data[x+1] };
			break;
		}
	}
	return layoutLen;
}
