/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#include "util/logging/emitter/PipeLogEmitter.h"
#include <cstdio>

PipeLogEmitter::PipeLogEmitter(FILE* pipe)
    : LogEmitter(),
      pipe_(pipe) {
}

PipeLogEmitter::~PipeLogEmitter() {
}

int PipeLogEmitter::Initialize() {
  if (0 > OpenPipe()) {
    return -1;
  }
  return 0;
}

int PipeLogEmitter::Finalize() {
  if (0 > ClosePipe()) {
    return -1;
  }
  return 0;
}

void PipeLogEmitter::Emitv(const char* prefix, const char* format,
                           va_list args) {
  fprintf(pipe_, prefix);
  vfprintf(pipe_, format, args);
  fprintf(pipe_, "\n");
}
