/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#ifndef BEFUTURECONFIG_H_
#define BEFUTURECONFIG_H_

#include "camera/Camera.h"

struct BeFutureConfig {
  // for system
  struct _system {
    int fps;
    int fps_update_interval;
    float running_speed;

    _system()
        : fps(0),
          fps_update_interval(0),
          running_speed(1.0f) {
    }
  } system;

  // for window
  struct _window {
    glm::i32vec2 default_pos;
    glm::i32vec2 default_size;

    _window()
        : default_pos(0),
          default_size(0) {
    }
  } window;

  // for camera
  struct _camera {
    glm::vec3 default_pos;
    glm::vec3 default_at;
    glm::vec3 default_up;
    Camera::Perspective default_pers;
    Camera::Viewport default_vport;

    _camera()
        : default_pos(0.0f),
          default_at(0.0f),
          default_up(0.0f),
          default_pers(0.0f, 0.0f, 0.0f, 0.0f),
          default_vport(glm::i32vec2(0), glm::i32vec2(0)) {
    }
  } camera;

  // for camera control
  struct _ds_camera {
    GLfloat translate_speed;
    GLfloat rotate_speed;
    GLfloat zoom_speed;

    _ds_camera()
        : translate_speed(0.0f),
          rotate_speed(0.0f),
          zoom_speed(0.0f) {
    }
  } ds_camera;

  // for tweak menu
  struct _tweak_menu {
    glm::vec2 default_pos;
    glm::vec2 default_size;
    glm::i32vec3 color;

    _tweak_menu()
        : default_pos(0.0f),
          default_size(0.0f),
          color(0) {
    }
  } tweak_menu;

  // for character storm
  struct _character_storm {
    int num_characters;
    glm::vec2 character_size;
    float character_lifetime;
    float emission_interval;

    _character_storm()
        : num_characters(0),
          character_size(0),
          character_lifetime(0.0f),
          emission_interval(0.0f) {
    }
  } character_storm;

  BeFutureConfig()
      : system(),
        window(),
        camera(),
        ds_camera(),
        tweak_menu(),
        character_storm() {
  }

  int LoadFromInifile(const char *ini_path);
};

#endif /* BEFUTURECONFIG_H_ */
