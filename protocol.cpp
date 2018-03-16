#include "protocol.h"

uint8_t interpretData(StatusElem* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {	
	uint8_t elemsNum = sizeof(layoutElems) / sizeof(layoutElem);
	uint8_t x = 0;
	while (layout < layout + layoutLen && data < data + dataLen) {
		uint8_t divisor = 0;
		char* name;
		for (int i = 0; i < elemsNum; i++) {
			layoutElem elem = layoutElems[i];
			if (! *layout == elem.code >> 8) {
				continue;
			}
			if (elem.code > 0xff && elem.code != *(layout + 1) & 0xff) {
				continue;
			}
			divisor = elem.divisor;
			name = elem.name;
			layout++;
			if (elem.code > 0xff) {
				layout++;
			}
		}
		if (divisor == 0) {
			layout++;
			continue;
		}
		uint16_t value = *(data++) << 8 | *(data++);
		status[x] = StatusElem{value, name};
		x++;
	}
	return x;
}
