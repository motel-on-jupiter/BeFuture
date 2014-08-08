/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef OPENGLMOUSELISTENER_H_
#define OPENGLMOUSELISTENER_H_

#include <vector>
#include <GL/freeglut.h>

#include "peripheral/MouseListener.h"

class OpenGLMouseListener {
 public:
  typedef void (*ButtonHookFunc)(int, int, int, int);
  typedef void (*MoveHookFunc)(int, int);
  typedef void (*PassiveMoveHookFunc)(int, int);
  typedef void (*WheelScrollHookFunc)(int, int, int, int);

  static void ButtonFunc(int button, int state, int x, int y) {
    // Call the hook functions
    MouseListener::MouseButton mbutton =
        (GLUT_LEFT_BUTTON == button) ? MouseListener::LeftButton :
        (GLUT_RIGHT_BUTTON == button) ? MouseListener::RightButton :
        (GLUT_MIDDLE_BUTTON == button) ?
            MouseListener::MiddleButton : MouseListener::InvalidMouseButton;
    if (MouseListener::InvalidMouseButton != mbutton) {
      if (GLUT_DOWN == state) {
        MOUSE_LISTENER.EventButtonPush(mbutton);
      } else if (GLUT_UP == state) {
        MOUSE_LISTENER.EventButtonRelease(mbutton);
      }
      MOUSE_LISTENER.UpdateCursorPos(x, y);
    }
    for (auto it = button_hooks_.begin(); button_hooks_.end() != it; ++it) {
      (*it)(button, state, x, y);
    }
  }

  static void MoveFunc(int x, int y) {
    // Call the hook functions
    MOUSE_LISTENER.EventCursorMove(x, y);
    for (auto it = move_hooks_.begin(); it != move_hooks_.end(); ++it) {
      (*it)(x, y);
    }
  }

  static void PassiveMoveFunc(int x, int y) {
    // Call the hook functions
    MOUSE_LISTENER.EventCursorMove(x, y);
    for (auto it = passive_move_hooks_.begin(); passive_move_hooks_.end() != it;
        ++it) {
      (*it)(x, y);
    }
  }
  static void WheelFunc(int wheel, int direction, int x, int y) {
    // Call the hook functions
    MouseListener::WheelScroll wheel_scroll =
        ((0 == wheel) && (0 < direction)) ? MouseListener::UpScroll :
        ((0 == wheel) && (0 > direction)) ? MouseListener::DownScroll :
        ((1 == wheel) && (0 < direction)) ? MouseListener::LeftScroll :
        ((1 == wheel) && (0 > direction)) ?
            MouseListener::RightScroll : MouseListener::InvalidWheelScroll;
    if (MouseListener::InvalidWheelScroll != wheel_scroll) {
      MOUSE_LISTENER.EventWheelScroll(wheel_scroll);
      MOUSE_LISTENER.UpdateCursorPos(x, y);
    }
    for (auto it = wheel_scroll_hooks_.begin(); wheel_scroll_hooks_.end() != it;
        ++it) {
      (*it)(wheel, direction, x, y);
    }
  }

  static void RegisterAllCallbackFuncs() {
    glutMouseFunc(OpenGLMouseListener::ButtonFunc);
    glutMotionFunc(OpenGLMouseListener::MoveFunc);
    glutPassiveMotionFunc(OpenGLMouseListener::PassiveMoveFunc);
    glutMouseWheelFunc(OpenGLMouseListener::WheelFunc);
  }

  static void PushButtonHook(ButtonHookFunc hook_func) {
    button_hooks_.push_back(hook_func);
  }
  static void PushMoveHook(MoveHookFunc hook_func) {
    move_hooks_.push_back(hook_func);
  }
  static void PushPassiveMoveHook(PassiveMoveHookFunc hook_func) {
    passive_move_hooks_.push_back(hook_func);
  }
  static void PushWheelHook(WheelScrollHookFunc hook_func) {
    wheel_scroll_hooks_.push_back(hook_func);
  }

  static bool CheckButtonPushed(MouseListener::MouseButton button) {
    return MOUSE_LISTENER.CheckButtonPushed(button);
  }
  static bool CheckButtonReleased(MouseListener::MouseButton button) {
    return MOUSE_LISTENER.CheckButtonReleased(button);
  }
  static bool CheckButtonPushing(MouseListener::MouseButton button) {
    return MOUSE_LISTENER.CheckButtonPushing(button);
  }
  static bool CheckButtonReleasing(MouseListener::MouseButton button) {
    return MOUSE_LISTENER.CheckButtonReleasing(button);
  }
  static bool CheckWheelScroll(MouseListener::WheelScroll scroll) {
    return MOUSE_LISTENER.CheckWheelScroll(scroll);
  }

 private:
  static std::vector<ButtonHookFunc> button_hooks_;
  static std::vector<MoveHookFunc> move_hooks_;
  static std::vector<PassiveMoveHookFunc> passive_move_hooks_;
  static std::vector<WheelScrollHookFunc> wheel_scroll_hooks_;
};

#endif /* OPENGLMOUSELISTENER_H_ */
