/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef KEYBOARDLISTENER_H_
#define KEYBOARDLISTENER_H_

#include <set>
#include <boost/serialization/singleton.hpp>

#define KEYBOARD_LISTENER (KeyboardListener::get_mutable_instance())

class KeyboardListener :
    public boost::serialization::singleton<KeyboardListener> {
 public:
  enum ModifierKey {
    ShiftKey,
    CtrlKey,
    AltKey,
  };

  KeyboardListener()
      : pushing_keys_(),
        push_keys_(),
        release_keys_(),
        pushing_mods_(),
        push_mods_(),
        release_mods_() {
  }
  virtual ~KeyboardListener() {
  }

  void EventKeyPush(unsigned char key) {
    pushing_keys_.insert(key);
    push_keys_.insert(key);
  }
  void EventKeyRelease(unsigned char key) {
    pushing_keys_.erase(key);
    release_keys_.insert(key);
  }
  void EventModifierOn(ModifierKey modifier) {
    pushing_mods_.insert(modifier);
    push_mods_.insert(modifier);
  }
  void EventModifierOff(ModifierKey modifier) {
    pushing_mods_.erase(modifier);
    release_mods_.insert(modifier);
  }
  void OnFrameEnd() {
    push_keys_.clear();
    release_keys_.clear();
    push_mods_.clear();
    release_mods_.clear();
  }

  bool CheckKeyPushed(unsigned char key) const {
    return push_keys_.end() != push_keys_.find(key);
  }
  bool CheckKeyReleased(unsigned char key) const {
    return release_keys_.end() != release_keys_.find(key);
  }
  bool CheckKeyPushing(unsigned char key) const {
    return pushing_keys_.end() != pushing_keys_.find(key);
  }
  bool CheckKeyReleasing(unsigned char key) const {
    return !CheckKeyPushing(key);
  }
  bool CheckModifierKeyPushed(ModifierKey mod) const {
    return push_mods_.end() != push_mods_.find(mod);
  }
  bool CheckModifierKeyReleased(ModifierKey mod) const {
    return release_mods_.end() != release_mods_.find(mod);
  }
  bool CheckModifierKeyPushing(ModifierKey mod) const {
    return pushing_mods_.end() != pushing_mods_.find(mod);
  }
  bool CheckModifierKeyReleasing(ModifierKey mod) const {
    return !CheckModifierKeyPushing(mod);
  }

 private:
  std::set<unsigned char> pushing_keys_;
  std::set<unsigned char> push_keys_;
  std::set<unsigned char> release_keys_;
  std::set<ModifierKey> pushing_mods_;
  std::set<ModifierKey> push_mods_;
  std::set<ModifierKey> release_mods_;
};

#endif /* KEYBOARDLISTENER_H_ */
