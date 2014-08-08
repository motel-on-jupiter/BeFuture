/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef MACRO_UTIL_H_
#define MACRO_UTIL_H_

#ifndef ASSERT
#define ASSERT(_expr) do { if (_expr) { __debugbreak(); } } while(0)
#endif

#ifndef UNUSED
#define UNUSED(_arg) do { (void)(_arg); } while(0)
#endif

#ifdef _MSC_VER
#include <Windows.h>
#else
#ifndef ARRAYSIZE
#define ARRAYSIZE(_var) (sizeof(_var) / sizeof((_var)[0]))
#endif
#endif

#endif /* MACRO_UTIL_H_ */
