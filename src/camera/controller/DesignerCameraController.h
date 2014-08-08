/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef DESIGNERCAMERACONROLLER_H_
#define DESIGNERCAMERACONROLLER_H_

#include "camera/controller/CameraController.h"

/*
 *
 */
class DesignerCameraController : public CameraController {
 public:
  DesignerCameraController(Camera &camera, float translate_speed,
                           float rotate_speed, float zoom_speed);
  virtual ~DesignerCameraController();

  virtual int Update() {
    return 0;
  }
  virtual void EventKeyboard(unsigned char key, int x, int y);
  virtual void EventKeyboardUp(unsigned char key, int x, int y);
  virtual void EventMouse(int button, int state, int x, int y);
  virtual void EventMouseWheel(int wheel, int direction, int x, int y);
  virtual void EventMouseMotion(int x, int y);

 private:
  struct Option {
    float translate_speed;
    float rotate_speed;
    float zoom_speed;
  } option_;
  struct MouseStat {
    int x;
    int y;
    int button;
    int state;
  } mouse_stat_;
};

#endif /* DESIGNERCAMERACONROLLER_H_ */
