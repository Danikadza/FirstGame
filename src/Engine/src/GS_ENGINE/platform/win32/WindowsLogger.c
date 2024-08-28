#ifdef _WIN32
#include <GS_ENGINE/Logger.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct GS_Logger
{
    char *name;
    char *buffer;
    unsigned int bufferSize;
    unsigned int filled;
    bool immedeate;
    bool colored;
    FILE *out;
};

const int TColors[] = {44, 30, 34, 33,
                       31, 41, 0};

GS_API void GS_LoggerInitialize()
{
    gsEngineLogger = GS_LoggerCreate("Engine", stdout, true, false, 512);
    gsGameLogger = GS_LoggerCreate("Game", stdout, true, false, 512);
}

GS_API void GS_LoggerDeinitialize()
{
    GS_LoggerDestroy(&gsEngineLogger);
    GS_LoggerDestroy(&gsGameLogger);
}

GS_API GS_Logger *GS_LoggerCreate(const char *name, FILE *out, bool immedeate,
                                  bool colored, unsigned int bufferSize)
{
    if (!out)
        return NULL;
    if (!bufferSize)
        return NULL;
    GS_Logger *ret = malloc(sizeof(GS_Logger));
    if (!ret)
        return NULL;
    ret->name = NULL;
    if (strlen(name))
    {
        ret->name = malloc(strlen(name) + 1);
        strcpy(ret->name, name);
        ret->name[strlen(name)] = '\0';
    }
    ret->out = out;
    ret->immedeate = immedeate;
    ret->colored = colored;
    ret->bufferSize = bufferSize;
    ret->buffer = malloc(bufferSize);
    ret->filled = 0;
    return ret;
}

GS_API void GS_LoggerLog(GS_Logger *logger, GS_LogLevel level,
                         const char *format, ...)
{
    if (!logger)
        return;
    if (!format)
        return;
    if (level < 0 || level > GS_FatalLevel)
        return;
    va_list args;
    va_start(args, format);
    char *tmp = malloc(logger->bufferSize);
    memset(tmp, 0, logger->bufferSize);
    char *tmp1 = malloc(logger->bufferSize);
    memset(tmp1, 0, logger->bufferSize);
    HANDLE hConsole;
    if (!tmp)
    {
        va_end(args);
        return;
    }
    if (logger->colored && (logger->out == stdout || logger->out == stderr))
    {
        if (logger->out == stdout)
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        else if (logger->out == stderr)
            hConsole = GetStdHandle(STD_ERROR_HANDLE);
        SetConsoleTextAttribute(hConsole, TColors[level]);
    }
    if (logger->name)
    {
        strcat(tmp1, "[");
        strcat(tmp1, logger->name);
        strcat(tmp1, "]: ");
    }
    vsnprintf(tmp, logger->bufferSize, format, args);
    if (logger->filled + strlen(tmp) + strlen(tmp1) + 1 >=
        logger->bufferSize)
        GS_LoggerFlush(logger);
    strcpy(&logger->buffer[logger->filled], tmp1);
    logger->filled += strlen(tmp1);
    strcpy(&logger->buffer[logger->filled], tmp);
    logger->filled += strlen(tmp);
    strcpy(&logger->buffer[logger->filled], "\n");
    logger->filled += strlen("\n");
    if (logger->immedeate)
        GS_LoggerFlush(logger);
    free(tmp);
    free(tmp1);
    SetConsoleTextAttribute(hConsole, TColors[6]);
}

GS_API void GS_LoggerAssert(GS_Logger *logger, bool expression,
                            const char *message)
{
    if (!logger)
        return;
    if (!expression)
    {
        if (!message)
            GS_LoggerLog(logger, GS_FatalLevel, "%s", "");
        GS_LoggerLog(logger, GS_FatalLevel, "%s", message);
    }
}

GS_API void GS_LoggerFlush(GS_Logger *logger)
{
    if (!logger)
        return;
    if (!logger->filled)
        return;
    HANDLE hConsole;
    fprintf(logger->out, "%s", logger->buffer);
    memset(logger->buffer, 0, logger->bufferSize);
    logger->filled = 0;
}

GS_API void GS_LoggerDestroy(GS_Logger **p_logger)
{
    if (!p_logger)
        return;
    if (!*p_logger)
        return;
    if ((*p_logger)->name)
        free((*p_logger)->name);
    free((*p_logger)->buffer);
    free(*p_logger);
    *p_logger = NULL;
}

GS_Logger *gsEngineLogger = NULL;
GS_Logger *gsGameLogger = NULL;
#endif
