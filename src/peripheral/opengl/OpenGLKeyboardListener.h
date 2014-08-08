/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef OPENGLKEYBOARDLISTENER_H_
#define OPENGLKEYBOARDLISTENER_H_

#include <vector>
#include <GL/freeglut.h>

#include "peripheral/KeyboardListener.h"

class OpenGLKeyboardListener {
 public:
  typedef void (*KeyPushHookFunc)(unsigned char, int, int);
  typedef void (*SpecialKeyPushHookFunc)(int, int, int);
  typedef void (*KeyReleaseHookFunc)(unsigned char, int, int);
  typedef void (*SpecialKeyReleaseHookFunc)(int, int, int);

  static void KeyboardFunc(unsigned char key, int x, int y) {
    // Call the hook functions
    KEYBOARD_LISTENER.EventKeyPush(key);
    for (auto it = key_push_hooks_.begin(); key_push_hooks_.end() != it; ++it) {
      (*it)(key, x, y);
    }
  }
  static void SpecialFunc(int key, int x, int y) {
    // Close the application with alt + F4
    if ((GLUT_KEY_F4 == key) && (0 != (glutGetModifiers() & GLUT_ACTIVE_ALT))) {
      glutLeaveMainLoop();

    }

    // Call the hook functions
    KEYBOARD_LISTENER.EventKeyPush(static_cast<unsigned char>(0x80 + key));
    for (auto it = special_key_push_hooks_.begin();
        special_key_push_hooks_.end() != it; ++it) {
      (*it)(key, x, y);
    }
  }
  static void KeyboardUpFunc(unsigned char key, int x, int y) {
    // Call the hook functions
    KEYBOARD_LISTENER.EventKeyRelease(key);
    for (auto it = key_release_hooks_.begin(); key_release_hooks_.end() != it;
        ++it) {
      (*it)(key, x, y);
    }
  }
  static void SpecialUpFunc(int key, int x, int y) {
    // Call the hook functions
    KEYBOARD_LISTENER.EventKeyRelease(static_cast<unsigned char>(0x80 + key));
    for (auto it = special_key_release_hooks_.begin();
        special_key_release_hooks_.end() != it; ++it) {
      (*it)(key, x, y);
    }
  }

  static void RegisterAllCallbackFuncs() {
    glutKeyboardFunc(OpenGLKeyboardListener::KeyboardFunc);
    glutSpecialFunc(OpenGLKeyboardListener::SpecialFunc);
    glutKeyboardUpFunc(OpenGLKeyboardListener::KeyboardUpFunc);
    glutSpecialUpFunc(OpenGLKeyboardListener::SpecialUpFunc);
  }

  static void PushKeyPushHook(KeyPushHookFunc hook_func) {
    key_push_hooks_.push_back(hook_func);
  }
  static void PushSpecialKeyPushHook(SpecialKeyPushHookFunc hook_func) {
    special_key_push_hooks_.push_back(hook_func);
  }
  static void PushKeyReleaseHook(KeyReleaseHookFunc hook_func) {
    key_release_hooks_.push_back(hook_func);
  }
  static void PushSpecialKeyReleaseHook(SpecialKeyReleaseHookFunc hook_func) {
    special_key_release_hooks_.push_back(hook_func);
  }

  static bool CheckKeyPushed(unsigned char key) {
    return KEYBOARD_LISTENER.CheckKeyPushed(key);
  }
  static bool CheckKeyReleased(unsigned char key) {
    return KEYBOARD_LISTENER.CheckKeyReleased(key);
  }
  static bool CheckKeyPushing(unsigned char key) {
    return KEYBOARD_LISTENER.CheckKeyPushing(key);
  }
  static bool CheckKeyReleasing(unsigned char key) {
    return KEYBOARD_LISTENER.CheckKeyReleasing(key);
  }
  static bool CheckSpecialKeyPushed(int key) {
    return KEYBOARD_LISTENER.CheckKeyPushed(
        static_cast<unsigned char>(0x80 + key));
  }
  static bool CheckSpecialKeyReleased(int key) {
    return KEYBOARD_LISTENER.CheckKeyReleased(
        static_cast<unsigned char>(0x80 + key));
  }
  static bool CheckSpecialKeyPushing(int key) {
    return KEYBOARD_LISTENER.CheckKeyPushing(
        static_cast<unsigned char>(0x80 + key));
  }
  static bool CheckSpecialKeyReleasing(int key) {
    return KEYBOARD_LISTENER.CheckKeyReleasing(
        static_cast<unsigned char>(0x80 + key));
  }

 private:
  static std::vector<KeyPushHookFunc> key_push_hooks_;
  static std::vector<SpecialKeyPushHookFunc> special_key_push_hooks_;
  static std::vector<KeyReleaseHookFunc> key_release_hooks_;
  static std::vector<SpecialKeyReleaseHookFunc> special_key_release_hooks_;
};

#endif /* OPENGLKEYBOARDLISTENER_H_ */
