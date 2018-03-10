#ifndef SUNEZY_DEBUG_H
#include "Arduino.h"
#define SUNEZY_DEBUG_H
#define SUNEZY_DEBUG_OUT Serial
#define SUNEZY_DEBUG false
#define __debug(msg) 	SUNEZY_DEBUG_OUT.print("@"); \
	SUNEZY_DEBUG_OUT.print(__func__); \
	SUNEZY_DEBUG_OUT.print("    "); \
	SUNEZY_DEBUG_OUT.println(msg); 
#endif
