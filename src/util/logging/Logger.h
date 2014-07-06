/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <vector>
#include <boost/serialization/singleton.hpp>

class LogEmitter;

#define LOGGER (Logger::get_mutable_instance())

class Logger : public boost::serialization::singleton<Logger> {
 public:
  enum Level {
    kNoneLevel,
    kDebug,
    kInfo,
    kNotice,
    kWarn,
    kError,
    kCrit,
    kNumLogLevels,
  };

  Logger();
  virtual ~Logger();

  // メッセージ出力
  void Emit(const char *format, ...);
  void Emitv(const char *format, va_list args);
  void Emit(Level level, const char *format, ...);
  void Emitv(Level level, const char *format, va_list args);

  void Debug(const char *format, ...);
  void Debugv(const char *format, va_list args);
  void Info(const char *format, ...);
  void Infov(const char *format, va_list args);
  void Notice(const char *format, ...);
  void Noticev(const char *format, va_list args);
  void Warn(const char *format, ...);
  void Warnv(const char *format, va_list args);
  void Error(const char *format, ...);
  void Errorv(const char *format, va_list args);
  void Crit(const char *format, ...);
  void Critv(const char *format, va_list args);

  void PushEmitter(LogEmitter *emitter) {
    if (NULL != emitter) {
      emitters_.push_back(emitter);
    }
  }
  void ClearEmitters() {
    emitters_.clear();
  }

 private:
  static const char *kLogPrefixes[kNumLogLevels];

  std::vector<LogEmitter *> emitters_;
};

#endif /* LOGGER_H_ */
