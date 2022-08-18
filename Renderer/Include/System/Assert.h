#pragma once

// Triggered when assertion fails
void FailedAssert(const char* file, int line, const char* expression);

#if defined(_DEBUG)
	#if defined(WIN32)
		#include <intrin.h>
		#define debugBreak() __debugbreak() // Stops at block of code! Win7/10/11
	#else
		#define debugBreak() // does nothing, need a solution for this
	#endif
	
	#undef assert // force my assert
	#define assert(x) if(x) { } else { FailedAssert(__FILE__,__LINE__, #x); debugBreak();} // Assert should exit?
	#define verify(x) ((x)? true : {FailedAssert(__FILE__,__LINE__, #x)) // No Debug Break
#else

	#define debugBreak() // Does nothing.
	#undef assert
	// Does nothing in release for performance reasons
	#define assert(x) {}

	// Validates but doesnt terminate, used too output a fail message
	#define verify(x) ((x)? true :  FailedAssert(__FILE__, __LINE__, #x))
#endif