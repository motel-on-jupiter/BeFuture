/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#include "util/logging/emitter/MessageBoxLogEmitter.h"

#include <cstdarg>
#include <cstdlib>
#include <cerrno>
#include <tchar.h>
#include <Windows.h>

const TCHAR *MessageBoxLogEmitter::kMessageBoxCaption = _T(
    "MessageBoxLogEmitter");

MessageBoxLogEmitter::MessageBoxLogEmitter()
    : LogEmitter(),
      boost::serialization::singleton<MessageBoxLogEmitter>() {
}

MessageBoxLogEmitter::~MessageBoxLogEmitter() {
}

int MessageBoxLogEmitter::Initialize() {
  return 0;
}

int MessageBoxLogEmitter::Finalize() {
  return 0;
}

void MessageBoxLogEmitter::Emitv(const char *prefix, const char *format,
                                 va_list args) {
  TCHAR buf[kMaxEmittableSize] = { 0 };
  size_t len = 0;
  errno_t error = mbstowcs_s(&len, buf, (size_t) kMaxEmittableSize, prefix,
                             (size_t) kMaxEmittableSize);
  if (ERANGE == error) {
    OutputDebugString(
        _T("[LOG_EMITTER ERROR]: message prefix prefix is too long to copy to the buffer\n"));
  } else if (EINVAL == error) {
    OutputDebugString(
        _T("[LOG_EMITTER ERROR]: mbstowcs_s() called with invald arguments to copy the message prefix to the buffer\n"));
    return;
  }
  if (len >= kMaxEmittableSize) {
    MessageBox(
        nullptr,
        _T("[EMITTER ERROR]: message prefix prefix is too long to copy the buffer\n"),
        kMessageBoxCaption, MB_OK);
  } else {
    len = vsnprintf_s((char *) (buf + len), kMaxEmittableSize - len, _TRUNCATE,
                      format, args);
    if (len >= kMaxEmittableSize - len) {
      MessageBox(
          nullptr,
          _T("[EMITTER ERROR]: message is too long to copy the buffer\n"),
          kMessageBoxCaption, MB_OK);
    }
  }
  MessageBox(nullptr, buf, kMessageBoxCaption, MB_OK);
}
