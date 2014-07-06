/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include <boost/noncopyable.hpp>

class Camera;

class CameraController : private boost::noncopyable {
 public:
  CameraController(Camera &camera)
      : camera_(camera) {
  }
  virtual ~CameraController() {
  }

  virtual int Update() = 0;
  virtual void EventKeyboard(unsigned char key, int x, int y) = 0;
  virtual void EventKeyboardUp(unsigned char key, int x, int y) = 0;
  virtual void EventMouse(int button, int state, int x, int y) = 0;
  virtual void EventMouseWheel(int wheel, int direction, int x, int y) = 0;
  virtual void EventMouseMotion(int x, int y) = 0;

  Camera &camera() {
    return camera_;
  }
  const Camera &const_camera() const {
    return camera_;
  }

 private:
  Camera &camera_;
};

#endif /* CAMERACONTROLLER_H_ */
