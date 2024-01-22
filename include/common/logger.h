//
// Created by yanjingsheng on 2024/1/22.
//

#pragma once

#include <fmt/core.h>
#include <ctime>
#include <string>

namespace concurrent {
#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

// https://blog.galowicz.de/2016/02/20/short_file_macro/
using cstr = const char *;

static constexpr auto PastLastSlash(cstr a, cstr b) -> cstr {
  return *a == '\0' ? b : *a == '/' ? PastLastSlash(a + 1, a + 1) : PastLastSlash(a + 1, b);
}

static constexpr auto PastLastSlash(cstr a) -> cstr { return PastLastSlash(a, a); }

#define __SHORT_FILE__                            \
  ({                                              \
    constexpr cstr sf__{PastLastSlash(__FILE__)}; \
    sf__;                                         \
  })

// Log levels.
#define LOG_LEVEL_OFF 1000
#define LOG_LEVEL_ERROR 500
#define LOG_LEVEL_WARN 400
#define LOG_LEVEL_INFO 300
#define LOG_LEVEL_DEBUG 200
#define LOG_LEVEL_TRACE 100
#define LOG_LEVEL_ALL 0

#define LOG_LOG_TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define LOG_OUTPUT_STREAM stdout

// Compile Option
#ifndef LOG_LEVEL

// #pragma message("Warning: LOG_LEVEL compile option was not explicitly
// given.")
#ifndef NDEBUG
#define LOG_LEVEL LOG_LEVEL_DEBUG
// #pragma message("LOG_LEVEL_DEBUG is used instead as DEBUG option is on.")
#else
// #pragma message("LOG_LEVEL_WARN is used instead as DEBUG option is off.")
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif
// #pragma message("Give LOG_LEVEL compile option to overwrite the default
// level.")
#endif

// For compilers which do not support __FUNCTION__
#if !defined(__FUNCTION__) && !defined(__GNUC__)
#define __FUNCTION__ ""
#endif

void OutputLogHeader(const char *file, int line, const char *func, int level);

// Two convenient macros for debugging
// 1. Logging macros.
// 2. LOG_XXX_ENABLED macros. Use these to "eliminate" all the debug blocks from
// release binary.
#ifdef LOG_ERROR_ENABLED
#undef LOG_ERROR_ENABLED
#endif
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR_ENABLED
// #pragma message("LOG_ERROR was enabled.")
#define LOG_ERROR(...)                                                      \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_ERROR); \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                \
  fprintf(LOG_OUTPUT_STREAM, "\n");                                         \
  ::fflush(stdout)
#define LOG_FORMAT_ERROR(...)                                               \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_ERROR); \
  ::fprintf(LOG_OUTPUT_STREAM, "%s\n", fmt::format(__VA_ARGS__).c_str());   \
  ::fflush(stdout)
#else
#define LOG_ERROR(...) ((void)0)
#endif

#ifdef LOG_WARN_ENABLED
#undef LOG_WARN_ENABLED
#endif
#if LOG_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN_ENABLED
// #pragma message("LOG_WARN was enabled.")
#define LOG_WARN(...)                                                      \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_WARN); \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                               \
  fprintf(LOG_OUTPUT_STREAM, "\n");                                        \
  ::fflush(stdout)
#define LOG_FORMAT_WARN(...)                                               \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_WARN); \
  ::fprintf(LOG_OUTPUT_STREAM, "%s\n", fmt::format(__VA_ARGS__).c_str());  \
  ::fflush(stdout)
#else
#define LOG_WARN(...) ((void)0)
#endif

#ifdef LOG_INFO_ENABLED
#undef LOG_INFO_ENABLED
#endif
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO_ENABLED
// #pragma message("LOG_INFO was enabled.")
#define LOG_INFO(...)                                                      \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_INFO); \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                               \
  fprintf(LOG_OUTPUT_STREAM, "\n");                                        \
  ::fflush(stdout)
#define LOG_FORMAT_INFO(...)                                               \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_INFO); \
  ::fprintf(LOG_OUTPUT_STREAM, "%s\n", fmt::format(__VA_ARGS__).c_str());  \
  ::fflush(stdout)
#else
#define LOG_INFO(...) ((void)0)
#endif

#ifdef LOG_DEBUG_ENABLED
#undef LOG_DEBUG_ENABLED
#endif
#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG_ENABLED
// #pragma message("LOG_DEBUG was enabled.")
#define LOG_DEBUG(...)                                                      \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_DEBUG); \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                \
  fprintf(LOG_OUTPUT_STREAM, "\n");                                         \
  ::fflush(stdout)
#define LOG_FORMAT_DEBUG(...)                                               \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_DEBUG); \
  ::fprintf(LOG_OUTPUT_STREAM, "%s\n", fmt::format(__VA_ARGS__).c_str());   \
  ::fflush(stdout)
#else
#define LOG_DEBUG(...) ((void)0)
#endif

#ifdef LOG_TRACE_ENABLED
#undef LOG_TRACE_ENABLED
#endif
#if LOG_LEVEL <= LOG_LEVEL_TRACE
#define LOG_TRACE_ENABLED
// #pragma message("LOG_TRACE was enabled.")
#define LOG_TRACE(...)                                                      \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_TRACE); \
  ::fprintf(LOG_OUTPUT_STREAM, __VA_ARGS__);                                \
  fprintf(LOG_OUTPUT_STREAM, "\n");                                         \
  ::fflush(stdout)
#define LOG_FORMAT_TRACE(...)                                               \
  OutputLogHeader(__SHORT_FILE__, __LINE__, __FUNCTION__, LOG_LEVEL_TRACE); \
  ::fprintf(LOG_OUTPUT_STREAM, "%s\n", fmt::format(__VA_ARGS__).c_str());   \
  ::fflush(stdout)
#else
#define LOG_TRACE(...) ((void)0)
#endif

// Output log message header in this format: [type] [file:line:function] time -
// ex: [ERROR] [somefile.cpp:123:doSome()] 2008/07/06 10:00:00 -
inline void OutputLogHeader(const char *file, int line, const char *func, int level) {
  time_t t = ::time(nullptr);
  tm *curTime = localtime(&t);  // NOLINT
  char time_str[32];
  ::strftime(time_str, 32, LOG_LOG_TIME_FORMAT, curTime);
  const char *type;
  switch (level) {
    case LOG_LEVEL_ERROR:
      type = LIGHT_RED "ERROR";
      break;
    case LOG_LEVEL_WARN:
      type = YELLOW "WARN ";
      break;
    case LOG_LEVEL_INFO:
      type = LIGHT_GREEN "INFO ";
      break;
    case LOG_LEVEL_DEBUG:
      type = WHITE "DEBUG";
      break;
    case LOG_LEVEL_TRACE:
      type = LIGHT_GRAY "TRACE";
      break;
    default:
      type = "UNKWN";
  }
  // PAVLO: DO NOT CHANGE THIS
  ::fprintf(LOG_OUTPUT_STREAM, "%s [%s:%d:%s] %s - ", time_str, file, line, func, type);
}

}  // namespace concurrent