/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#include "befuture/BeFutureMain.h"
#include "util/logging/emitter/DebuggerConsoleLogEmitter.h"
#include "util/logging/emitter/FileLogEmitter.h"
#include "util/logging/Logger.h"
#include "util/wrapper/crtdbg_wrap.h"

#define kDebugAlloc (-1)

int main(int argc, char **argv) {
#ifdef _MSC_VER
#ifdef _DEBUG
  // Validate the detection for memory leak
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetBreakAlloc(kDebugAlloc);
#endif
#endif

  // Set up logger
  LOGGER.PushEmitter(&DEBUGGER_CONSOLE_LOG_EMITTER);
  LOGGER.PushEmitter(&STDERR_LOG_EMITTER);

  // Load the configuration on the external file
  if (2 != argc) {
    LOGGER.Error("Usage: %s <configfile>", argv[0]);
    return -1;
  }

  // Start the main function for BeFuture
  return BeFutureMain(argc, argv, argv[1]);
}

