#pragma once
#ifndef GS_LOGGER_H
#define GS_LOGGER_H

#include "Define.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum
{
    GS_TraceLevel,
    GS_InfoLevel,
    GS_DebugLevel,
    GS_WarnLevel,
    GS_ErrorLevel,
    GS_FatalLevel,
} GS_LogLevel;

typedef struct GS_Logger GS_Logger;

GS_API void GS_LoggerInitialize();
GS_API void GS_LoggerDeinitialize();

GS_API GS_Logger *GS_LoggerCreate(const char *name, FILE *out, bool immedeate,
                                  bool colored, unsigned int bufferSize);
GS_API void GS_LoggerLog(GS_Logger *logger, GS_LogLevel level,
                         const char *format, ...);
GS_API void GS_LoggerAssert(GS_Logger *logger, bool expression,
                            const char *message);
GS_API void GS_LoggerFlush(GS_Logger *logger);
GS_API void GS_LoggerDestroy(GS_Logger **p_logger);

extern GS_Logger *gsEngineLogger;
extern GS_Logger *gsGameLogger;

#if defined(GS_DEBUG)
#define GS_ENGINE_LOG(level, format, ...)                                      \
    GS_LoggerLog(gsEngineLogger, level, format, __VA_ARGS__)
#define GS_GAME_LOG(level, format, ...)                                        \
    GS_LoggerLog(gsGameLogger, level, format, __VA_ARGS__)
#define GS_ENGINE_ASSERT(expression, message)                                  \
    GS_LoggerAssert(gsEngineLogger, expression, message)
#define GS_GAME_ASSERT(expression, message)                                    \
    GS_LoggerAssert(gsGameLogger, expression, message)
#else
#define GS_ENGINE_LOG(level, format, ...)
#define GS_GAME_LOG(level, format, ...)
#define GS_ENGINE_ASSERT(expression, message)
#define GS_GAME_ASSERT(expression, message)
#endif

#endif
