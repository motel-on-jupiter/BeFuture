/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef LOGEMITTER_H_
#define LOGEMITTER_H_

class LogEmitter {
 public:
  LogEmitter();
  virtual ~LogEmitter();

  virtual void Emit(const char *prefix, const char *format, ...);
  virtual void Emitv(const char *prefix, const char *format, va_list args) = 0;
};

#endif /* LOGEMITTER_H_ */
