/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#include "util/logging/Logger.h"

#include <cstdarg>
#include <cstdio>
#include <boost/static_assert.hpp>

#include "util/logging/emitter/LogEmitter.h"

class LogEmitter;

const char *Logger::kLogPrefixes[] = { "",         //kNoneLevel
    "[DEBUG]",  //kDebug
    "[INFO]",   //kInfo
    "[NOTICE]",  //kNotice
    "[WARN]",   //kWarn
    "[ERROR]",  //kError
    "[CRIT]",   //kCrit,
    };

Logger::Logger()
    : boost::serialization::singleton<Logger>(),
      emitters_() {
}

Logger::~Logger() {
}

void Logger::Emit(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kNoneLevel, format, args);
  va_end(args);
}

void Logger::Emitv(const char *format, va_list args) {
  Emitv(kNoneLevel, format, args);
}

void Logger::Emit(Level level, const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(level, format, args);
  va_end(args);
}

void Logger::Emitv(Level level, const char *format, va_list args) {
  if (level >= kNumLogLevels) {
    fprintf(stdout,
            "Failed for logger to emit the message due to invalid level: %d",
            level);
    return;
  }

  auto it = emitters_.begin();
  for (; emitters_.end() != it; ++it) {
    (*it)->Emitv(kLogPrefixes[level], format, args);
  }
}

void Logger::Debug(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kDebug, format, args);
  va_end(args);
}

void Logger::Debugv(const char *format, va_list args) {
  Emitv(kDebug, format, args);
}

void Logger::Info(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emit(kInfo, format, args);
  va_end(args);
}

void Logger::Infov(const char *format, va_list args) {
  Emitv(kInfo, format, args);
}

void Logger::Notice(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kNotice, format, args);
  va_end(args);
}

void Logger::Noticev(const char *format, va_list args) {
  Emitv(kNotice, format, args);
}

void Logger::Warn(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kWarn, format, args);
  va_end(args);
}

void Logger::Warnv(const char *format, va_list args) {
  Emitv(kWarn, format, args);
}

void Logger::Error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kError, format, args);
  va_end(args);
}

void Logger::Errorv(const char *format, va_list args) {
  Emitv(kError, format, args);
}

void Logger::Crit(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kCrit, format, args);
  va_end(args);
}

void Logger::Critv(const char *format, va_list args) {
  Emitv(kCrit, format, args);
}

