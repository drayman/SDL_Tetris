#ifndef LOG_H
#define LOG_H

/*
 * Desktop log priority values, in ascending priority order.
 */
typedef enum log_priority {
    LOG_UNKNOWN = 0,
    LOG_DEFAULT,
    LOG_VERBOSE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_SILENT
} log_priority;

#define LOG_SEVERITY LOG_INFO
#define LOG_TAG "Tetris"

#if defined __ANDROID__

#include <android/log.h>
#define LOGD(...) __android_log_print(LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(LOG_ERROR, LOG_TAG, __VA_ARGS__)

#elif defined __IPHONEOS__

#error NEED IMPLEMENTATION!
#include "alma_log_file.h"
#define LOGD(...) __iphone_log_print(LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __iphone_log_print(LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) __iphone_log_print(LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __iphome_log_print(LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else

#include "desktop_log.h"
#define LOGD(...) desktop_log_print(LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) desktop_log_print(LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGW(...) desktop_log_print(LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) desktop_log_print(LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif

#endif //LOG_H
