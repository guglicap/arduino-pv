#include "protocol.h"

uint8_t interpretData(StatusElem* status, char* layout, uint8_t layoutLen, uint8_t* data, uint8_t dataLen) {	
	uint8_t numElems = sizeof(layoutElems) / sizeof(layoutElem);
	uint8_t x = 0; // x holds the current element in status
	for (int i = 0; i < layoutLen && 2 * i + 1 < dataLen; i++) {
		uint8_t divisor = 0; // going to be assigned to the right divisor by the next for loop
		char* name;
		for (int j = 0; j < numElems; j++) {
			layoutElem elem = layoutElems[j];
			uint16_t layoutCode;
			if (elem.code > 0xff && layoutLen > i + 1) { // layout codes can be 2 bytes long
				layoutCode = layout[i] << 8 | layout[i+1];
			} else {
				layoutCode = layout[i];
			}
			if (layoutCode == elem.code) {
				divisor = elem.divisor;
				name = elem.name;
				break;
			}
		}
		if (divisor == 0) { // no match
			continue;
		}
		uint8_t value = data[2 * i] << 8 | data[2 * i + 1];
		value = value / divisor;
		status[x] = StatusElem{name, value};
		x++;
	}
	return x;
}
