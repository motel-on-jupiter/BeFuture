/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#include "camera/controller/DesignerCameraController.h"

#include <GL/freeglut.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "camera/Camera.h"
#include "camera/controller/CameraController.h"

DesignerCameraController::DesignerCameraController(Camera &camera,
                                                   float translate_speed,
                                                   float rotate_speed,
                                                   float zoom_speed)
    : CameraController(camera),
      option_(),
      mouse_stat_() {
  option_.translate_speed = translate_speed;
  option_.rotate_speed = rotate_speed;
  option_.zoom_speed = zoom_speed;
  mouse_stat_.x = 0;
  mouse_stat_.y = 0;
  mouse_stat_.button = GLUT_LEFT;
  mouse_stat_.state = GLUT_UP;
}

DesignerCameraController::~DesignerCameraController() {
}

void DesignerCameraController::EventKeyboard(unsigned char key, int x, int y) {
  UNUSED(key);
  UNUSED(x);
  UNUSED(y);
}

void DesignerCameraController::EventKeyboardUp(unsigned char key, int x,
                                               int y) {
  UNUSED(key);
  UNUSED(x);
  UNUSED(y);
}

void DesignerCameraController::EventMouse(int button, int state, int x, int y) {
  mouse_stat_.x = x;
  mouse_stat_.y = y;
  mouse_stat_.button = button;
  mouse_stat_.state = state;
}

void DesignerCameraController::EventMouseWheel(int wheel, int direction, int x,
                                               int y) {
  UNUSED(x);
  UNUSED(y);

  if (0 == wheel) {
    glm::vec3 posToAt = camera().at() - camera().pos();
    glm::vec3 forward = glm::normalize(posToAt);
    if (0 < direction) {
      // on scroll up
      if (glm::length(posToAt) >= option_.zoom_speed) {
        camera().set_pos(camera().pos() + forward * option_.zoom_speed);
      }
    } else {
      // on scroll down
      camera().set_pos(camera().pos() + forward * option_.zoom_speed * -1.0f);
    }
  }
}

void DesignerCameraController::EventMouseMotion(int x, int y) {
  if (GLUT_DOWN == mouse_stat_.state) {
    glm::vec3 pos = camera().pos();
    glm::vec3 at = camera().at();
    glm::vec3 up = camera().up();
    glm::vec3 right = glm::normalize(glm::cross(at - pos, up));
    float deltaX = static_cast<float>(x - mouse_stat_.x);
    float deltaY = static_cast<float>(y - mouse_stat_.y);

    if (GLUT_RIGHT_BUTTON == mouse_stat_.button) {
      glm::vec3 local = pos - at;
      glm::vec3 rotated = glm::rotate(local, deltaY * option_.rotate_speed,
                                      right);
      rotated = glm::rotate(rotated, deltaX * option_.rotate_speed, up);
      camera().set_pos(rotated + at);
      camera().set_up(glm::rotate(up, deltaY * option_.rotate_speed, right));
    } else if (GLUT_MIDDLE_BUTTON == mouse_stat_.button) {
      glm::vec3 move = up * deltaY * option_.translate_speed;
      move += right * deltaX * option_.translate_speed * -1.0f;
      camera().set_pos(pos + move);
      camera().set_at(at + move);
    }
  }

  mouse_stat_.x = x;
  mouse_stat_.y = y;
}
