#ifndef MOD_LOGGER_H
#define MOD_LOGGER_H

#include <android/log.h>

#define LOG_TAG "SAMP-AML-Menu"

class CLogger {
public:
    void Info(const char* fmt, ...) {
        va_list ap; va_start(ap, fmt);
        __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, fmt, ap);
        va_end(ap);
    }
    void Error(const char* fmt, ...) {
        va_list ap; va_start(ap, fmt);
        __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, ap);
        va_end(ap);
    }
    void Warn(const char* fmt, ...) {
        va_list ap; va_start(ap, fmt);
        __android_log_vprint(ANDROID_LOG_WARN, LOG_TAG, fmt, ap);
        va_end(ap);
    }
    void Debug(const char* fmt, ...) {
        va_list ap; va_start(ap, fmt);
        __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ap);
        va_end(ap);
    }
};

static CLogger g_Logger;
#define logger (&g_Logger)

#endif
