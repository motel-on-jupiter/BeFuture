/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#ifndef MESSAGEBOXLOGEMITTER_H_
#define MESSAGEBOXLOGEMITTER_H_

#include "util/logging/emitter/LogEmitter.h"
#include <tchar.h>
#include <boost/serialization/singleton.hpp>

#define MESSAGE_BOX_LOG_EMITTER (MessageBoxLogEmitter::get_mutable_instance())

class MessageBoxLogEmitter : public LogEmitter,
    public boost::serialization::singleton<MessageBoxLogEmitter> {
 public:
  enum {
    kMaxEmittableSize = 1024
  };
  static const TCHAR *kMessageBoxCaption;

  MessageBoxLogEmitter();
  virtual ~MessageBoxLogEmitter();

  virtual int Initialize();
  virtual int Finalize();

  virtual void Emitv(const char *prefix, const char *format, va_list args);
};

#endif /* MESSAGEBOXLOGEMITTER_H_ */
