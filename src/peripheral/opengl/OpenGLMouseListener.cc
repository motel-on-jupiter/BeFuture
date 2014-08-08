/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "peripheral/opengl/OpenGLMouseListener.h"

std::vector<OpenGLMouseListener::ButtonHookFunc> OpenGLMouseListener::button_hooks_;
std::vector<OpenGLMouseListener::MoveHookFunc> OpenGLMouseListener::move_hooks_;
std::vector<OpenGLMouseListener::PassiveMoveHookFunc> OpenGLMouseListener::passive_move_hooks_;
std::vector<OpenGLMouseListener::WheelScrollHookFunc> OpenGLMouseListener::wheel_scroll_hooks_;
