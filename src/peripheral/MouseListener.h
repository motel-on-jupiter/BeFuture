/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef MOUSELISTENER_H_
#define MOUSELISTENER_H_

#include <bitset>
#include <set>

#include <boost/serialization/singleton.hpp>

#define MOUSE_LISTENER (MouseListener::get_mutable_instance())

class MouseListener : public boost::serialization::singleton<MouseListener> {
 public:
  enum MouseButton {
    LeftButton,
    RightButton,
    MiddleButton,
    InvalidMouseButton,
  };
  enum WheelScroll {
    UpScroll,
    DownScroll,
    LeftScroll,
    RightScroll,
    NumOfWheelScroll,
    InvalidWheelScroll,
  };

  MouseListener()
      : cursor_pos_(0),
        cursor_move_(0),
        pushing_buttons_(),
        push_buttons_(),
        release_buttons_(),
        wheel_scrolls_() {
  }
  virtual ~MouseListener() {
  }

  void EventButtonPush(MouseButton button) {
    pushing_buttons_.insert(button);
    push_buttons_.insert(button);
  }
  void EventButtonRelease(MouseButton button) {
    pushing_buttons_.erase(button);
    release_buttons_.insert(button);
  }
  void EventCursorMove(int x, int y) {
    cursor_move_.x = x;
    cursor_move_.y = y;
  }
  void EventWheelScroll(MouseListener::WheelScroll scroll) {
    wheel_scrolls_.set(scroll);
  }
  void UpdateCursorPos(int x, int y) {
    cursor_pos_.x = x;
    cursor_pos_.y = y;
  }
  void OnFrameEnd() {
    push_buttons_.clear();
    release_buttons_.clear();
    wheel_scrolls_.reset();
  }

  bool CheckButtonPushed(MouseButton button) const {
    return push_buttons_.end() != push_buttons_.find(button);
  }
  bool CheckButtonReleased(MouseButton button) const {
    return release_buttons_.end() != release_buttons_.find(button);
  }
  bool CheckButtonPushing(MouseButton button) const {
    return pushing_buttons_.end() != pushing_buttons_.find(button);
  }
  bool CheckButtonReleasing(MouseButton button) const {
    return !CheckButtonPushing(button);
  }
  bool CheckWheelScroll(WheelScroll scroll) const {
    return wheel_scrolls_.test(scroll);
  }

 private:
  glm::i32vec2 cursor_pos_;
  glm::i32vec2 cursor_move_;
  std::set<MouseButton> pushing_buttons_;
  std::set<MouseButton> push_buttons_;
  std::set<MouseButton> release_buttons_;
  std::bitset<NumOfWheelScroll> wheel_scrolls_;
};

#endif /* MOUSELISTENER_H_ */
