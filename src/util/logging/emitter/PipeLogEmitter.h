/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef PIPELOGEMITTER_H_
#define PIPELOGEMITTER_H_

#include "util/logging/emitter/LogEmitter.h"
#include <cstdio>

class PipeLogEmitter : public LogEmitter {
 public:
  PipeLogEmitter(FILE *pipe);
  virtual ~PipeLogEmitter();

  virtual int Initialize();
  virtual int Finalize();

  virtual void Emitv(const char *prefix, const char *format, va_list args);

 protected:
  virtual int OpenPipe() = 0;
  virtual int ClosePipe() = 0;

 private:
  FILE *pipe_;
};

#endif /* PIPELOGEMITTER_H_ */
