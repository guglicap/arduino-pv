#include "protocol.h"

uint8_t interpretData(StatusElem* status, char* layout, uint8_t* data, uint8_t dataLen) {
	uint8_t layoutLen = strlen(layout);
	for (int i = 0; i < layoutLen; i++) {
		uint8_t x = 2*i;
		if (dataLen <= x + 1) {
			break;
		}
		for (int j = 0; j < sizeof(layoutElems) / sizeof(layoutElem); j++) {
			if (layoutElems[j].code != (data[x] << 8 | data[x+1])) {
				continue;
			}
			status[i] = StatusElem{ layoutElems[j].name, data[x] << 8 | data[x+1] };
			break;
		}
	}
	return layoutLen;
}
