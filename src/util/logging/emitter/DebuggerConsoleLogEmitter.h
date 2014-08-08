/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#ifndef DEBUGGERCONSOLELOGEMITTER_H_
#define DEBUGGERCONSOLELOGEMITTER_H_

#include <boost/serialization/singleton.hpp>
#include "util/logging/emitter/LogEmitter.h"

#define DEBUGGER_CONSOLE_LOG_EMITTER (DebuggerConsoleLogEmitter::get_mutable_instance())

class DebuggerConsoleLogEmitter : public LogEmitter,
    public boost::serialization::singleton<DebuggerConsoleLogEmitter> {
 public:
  enum {
    kMaxEmittableSize = 1024
  };

  DebuggerConsoleLogEmitter();
  virtual ~DebuggerConsoleLogEmitter();

  virtual int Initialize();
  virtual int Finalize();

  virtual void Emitv(const char *prefix, const char *format, va_list args);

 private:
  virtual int EmitMultibyteChars(const char *mbchars);
  virtual int EmitPrefix(const char *prefix);
  virtual int EmitBody(const char *format, va_list args);
};

#endif /* DEBUGGERCONSOLELOGEMITTER_H_ */
