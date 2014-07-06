/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "BeFutureContext.h"

#include <GL/freeglut.h>

#include "camera/controller/DesignerCameraController.h"
#include "camera/Camera.h"
#include "util/system/FPSCounter.h"
#include "util/macro_util.h"

int BeFutureContext::LoadFromInifile(const char *ini_path) {
  BeFutureConfig config;
  if (0 > config.LoadFromInifile(ini_path)) {
    return -1;
  }
  LoadFromConfig(config);
  return 0;
}

void BeFutureContext::LoadFromConfig(const BeFutureConfig &config) {
  memcpy(&(this->config), &config, sizeof(config));

  system.fps_counter = new FPSCounter(config.system.fps_update_interval);
  ASSERT(nullptr == system.fps_counter);

  system.boot_sec = 0.0f;
  system.app_sec = 0.0f;
  system.ideal_fps = config.system.fps;
  system.current_fps = 0;
  system.running_speed = config.system.running_speed;
  system.pause = false;
  system.debug_draw = false;

  window.id = 0;
  window.pos = config.window.default_pos;
  window.size = config.window.default_size;

  rendering.camera = new Camera(config.camera.default_pos,
                                config.camera.default_at,
                                glm::normalize(config.camera.default_up),
                                config.camera.default_pers,
                                config.camera.default_vport);
  ASSERT(nullptr == rendering.camera);

  rendering.camera_controller = new DesignerCameraController(
      *(rendering.camera), config.ds_camera.translate_speed,
      config.ds_camera.rotate_speed, config.ds_camera.zoom_speed);
  ASSERT(nullptr == rendering.camera_controller);

  sequence.num_characters = 0;
  sequence.character_size = config.character_storm.character_size;
  sequence.character_lifetime = config.character_storm.character_lifetime;
  sequence.emission_interval = config.character_storm.emission_interval;
  sequence.emission_active = true;
}

void BeFutureContext::Unload() {
  delete rendering.camera_controller;
  delete rendering.camera;
  delete system.fps_counter;
}
