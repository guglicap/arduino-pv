#ifndef SUNEZY_PROTOCOL
#define SUNEZY_PROTOCOL

#include "stdint.h"
#include "string.h"

struct layoutElem {
	uint16_t code;
	uint8_t divisor;
	char* name;
};

const layoutElem layoutElems[] = {
	{0x00, 10, "Temp-inv"},		
	{0x01, 10, "Vpv1"},		
	{0x02, 10, "Vpv2"},		
	{0x03, 10, "Vpv3"},		
	{0x04, 10, "Ipv1"},		
	{0x05, 10, "Ipv2"},		
	{0x06, 10, "Ipv3"},		
	{0x40, 10, "Vpv"},		
	{0x41, 10, "Iac"},		
	{0x42, 10, "Vac"},		
	{0x43, 100, "Fac"},		
	{0x44, 1, "Pac"},			
	{0x45, 1, "Zac"},			
	{0x4748, 10, "E-Total"},		
	{0x494a, 1, "h-Total"},			
	{0x4c, 1, "Mode"},			
	{0x7e7f, 1, "Error"},			
};

struct StatusElem {
	char* name;
	uint16_t value;
};

uint8_t interpretData(StatusElem* status, char* layout, uint8_t* data, uint8_t dataLen);
#endif
