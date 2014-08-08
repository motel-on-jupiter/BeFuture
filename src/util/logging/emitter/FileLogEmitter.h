/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#ifndef FILELOGEMITTER_H_
#define FILELOGEMITTER_H_

#include <cstdio>
#include <boost/serialization/singleton.hpp>

#include "util/logging/emitter/PipeLogEmitter.h"

#define STDOUT_LOG_EMITTER (StdoutLogEmitter::get_mutable_instance())
#define STDERR_LOG_EMITTER (StderrLogEmitter::get_mutable_instance())
#define FILE_LOG_EMITTER(_filepath) (FileLogEmitter<_filepath>::get_mutable_instance())

class StdoutLogEmitter : public PipeLogEmitter,
    public boost::serialization::singleton<StdoutLogEmitter> {
 public:
  StdoutLogEmitter()
      : PipeLogEmitter(stdout) {
  }
  virtual ~StdoutLogEmitter() {
  }

 protected:
  virtual int OpenPipe() {
    return 0;
  }
  virtual int ClosePipe() {
    return 0;
  }
};

class StderrLogEmitter : public PipeLogEmitter,
    public boost::serialization::singleton<StderrLogEmitter> {
 public:
  StderrLogEmitter()
      : PipeLogEmitter(stderr) {
  }
  virtual ~StderrLogEmitter() {
  }

 protected:
  virtual int OpenPipe() {
    return 0;
  }
  virtual int ClosePipe() {
    return 0;
  }
};

template<const char *filepath>
class FileLogEmitter : public PipeLogEmitter,
    public boost::serialization::singleton<FileLogEmitter<filepath>> {
 public:
  FileLogEmitter()
      : PipeLogEmitter(nullptr) {
  }
  virtual ~FileLogEmitter() {
  }

 protected:
  virtual int OpenPipe() {
    pipe_ = fopen(filepath, "a");
    if (nullptr == pipe_) {
      return -1;
    }
    return 0;
  }
  virtual int ClosePipe() {
    if (nullptr != pipe_) {
      if (EOF == fclose(pipe_)) {
        return -1;
      }
    }
    return 0;
  }
};

#endif /* FILELOGEMITTER_H_ */
