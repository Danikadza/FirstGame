#pragma once
#ifndef GS_DEFINE_H
#define GS_DEFINE_H

#if defined(GS_WIN32) || defined(WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(GS_LINUX) || defined(__linux__)
#define EXPORT __attribute__((visibility("default")))
#define IMPORT 
#else
#error "PLATFORM NOT SUPPORTED"
#endif

#if GS_ENGINE_COMPILE
#define GS_API EXPORT
#else
#define GS_API IMPORT
#endif

#endif
