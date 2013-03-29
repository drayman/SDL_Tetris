#include <stdio.h>
#include <stdarg.h>

#include "log.h"

void desktop_log_vprint(log_priority priority, const char *tag,  const char *fmt, va_list pa)
{

#ifdef LOG_SEVERITY
	if (LOG_SEVERITY <= priority) {
#endif


	switch(priority)
	{
		case LOG_UNKNOWN:
		case LOG_DEFAULT:
		case LOG_VERBOSE:
		case LOG_DEBUG:
			printf("[%s] Debug: ", tag);
			break;
		case LOG_INFO:
            printf("[%s] Info: ", tag);
			break;
		case LOG_WARN:
			printf("[%s] Warning: ", tag);
			break;
		case LOG_ERROR:
			printf("[%s] Error: ", tag);
			break;
		case LOG_FATAL:
			printf("[%s] Fatal: ", tag);
			break;
		default:
			break;
	}

	vprintf(fmt, pa);
	printf("\n");

#ifdef LOG_SEVERITY
	}
#endif

}

void desktop_log_print(log_priority priority, const char *tag,  const char *fmt, ...)
{
    va_list pa; va_start(pa, fmt);
    desktop_log_vprint(priority, tag, fmt, pa);
    va_end(pa);
}
