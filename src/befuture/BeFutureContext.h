/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef BEFUTURECONTEXT_H_
#define BEFUTURECONTEXT_H_

#include "befuture/BeFutureConfig.h"

class Camera;
class CharacterStorm;
class ColorBar;
class DesignerCameraController;
class FPSCounter;
class SkySphere;

struct BeFutureContext {
  BeFutureConfig config;

  struct _system {
    FPSCounter *fps_counter;
    float boot_sec;
    float app_sec;
    int ideal_fps;
    int current_fps;
    bool pause;
    float running_speed;
    bool debug_draw;
  } system;

  struct _window {
    int id;
    glm::i32vec2 pos;
    glm::i32vec2 size;
  } window;

  struct _rendering {
    Camera *camera;
    DesignerCameraController *camera_controller;
  } rendering;

  struct _sequence {
    SkySphere *sky;
    CharacterStorm *char_storm;
    ColorBar *color_bar;
    int num_characters;
    glm::vec2 character_size;
    float character_lifetime;
    float emission_interval;
    bool emission_active;
  } sequence;

  int LoadFromInifile(const char *ini_path);
  void LoadFromConfig(const BeFutureConfig &config);
  void Unload();
};

#endif /* BEFUTURECONTEXT_H_ */
