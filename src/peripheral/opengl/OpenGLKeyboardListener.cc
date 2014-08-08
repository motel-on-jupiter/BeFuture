/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "peripheral/opengl/OpenGLKeyboardListener.h"

std::vector<OpenGLKeyboardListener::KeyPushHookFunc> OpenGLKeyboardListener::key_push_hooks_;
std::vector<OpenGLKeyboardListener::SpecialKeyPushHookFunc> OpenGLKeyboardListener::special_key_push_hooks_;
std::vector<OpenGLKeyboardListener::KeyReleaseHookFunc> OpenGLKeyboardListener::key_release_hooks_;
std::vector<OpenGLKeyboardListener::SpecialKeyReleaseHookFunc> OpenGLKeyboardListener::special_key_release_hooks_;
