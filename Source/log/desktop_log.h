#ifndef DESKTOP_LOG_H
#define DESKTOP_LOG_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Send a formatted string to the log, used like printf(fmt,...)
 */
void desktop_log_print(log_priority priority, const char *tag,  const char *fmt, ... );

/*
 * A variant of __desktop_log_print() that takes a va_list to list
 * additional parameters.
 */
void desktop_log_vprint(log_priority priority, const char *tag,  const char *fmt, va_list pa );

#ifdef __cplusplus
}
#endif

#endif // DESKTOP_LOG_H
