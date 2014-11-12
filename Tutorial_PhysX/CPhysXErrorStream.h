#ifndef _CPHYSXERRORSTREAM_H_
#define _CPHYSXERRORSTREAM_H_

#if defined(WIN32) || defined(WIN64)
	#define NOMINMAX
	#include <windows.h>
#endif

#include "ILogger.h"
#include "NxUserOutputStream.h"

//-----------------------------------------------------------------------------
class ErrorStream : public NxUserOutputStream
{
public:
	ErrorStream(irr::ILogger* logger) : Logger(logger)
	{
	}
	void reportError(NxErrorCode e, const char* message, const char* file, int line)
	{

		//sprintf(Temp, "%s (%d) :", file, line);
		//print(Temp);
		switch (e)
		{
			case NXE_INVALID_PARAMETER:
				sprintf(Temp, "PhysX Invalid Parameter: %s\n", message);
				break;
			case NXE_INVALID_OPERATION:
				sprintf(Temp, "PhysX Invalid Operation: %s\n", message);
				break;
			case NXE_OUT_OF_MEMORY:
				sprintf(Temp, "PhysX Out Of Memory: %s\n", message);
				break;
			case NXE_DB_INFO:
				sprintf(Temp, "PhysX Info: %s\n", message);
				break;
			case NXE_DB_WARNING:
				sprintf(Temp, "PhysX Warning: %s\n", message);
				break;
			default:
				sprintf(Temp, "PhysX Unknown Error: %s\n", message);
		}
		print(Temp);
	}

	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
	{
		sprintf(Temp, "PhysX Access Violation : %s (%s line %d)\n", message, file, line);
		print(Temp);
		#if defined(WIN32) || defined(WIN64)
		switch (MessageBox(0, message, "PhysX AssertViolation, See Console For Details.", MB_ABORTRETRYIGNORE))
		{
			case IDRETRY:
				return NX_AR_CONTINUE;
			case IDIGNORE:
				return NX_AR_IGNORE;
			case IDABORT:
			default:
				return NX_AR_BREAKPOINT;
		}
		#elif defined(LINUX)
		assert(0);
		#elif defined(_XBOX)
		return NX_AR_BREAKPOINT;
		#elif defined(__CELLOS_LV2__)
		return NX_AR_BREAKPOINT;
		#endif
	}

	void print(const char* message)
	{
		//printf(message);
		Logger->log(message);
	}
private:
	irr::ILogger* Logger;
	irr::c8 Temp[512];
};
//-----------------------------------------------------------------------------

#endif
