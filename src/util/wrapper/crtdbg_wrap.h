/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CRTDBG_WRAP_H_
#define CRTDBG_WRAP_H_

#ifdef _MSC_VER

#ifdef _DEBUG

// include antecedent to overwriting new
#include <stdlib.h>
#include <cstdlib>
#include <new>
#include <memory>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#endif

#endif /* CRTDBG_WRAP_H_ */
