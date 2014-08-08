/**
 * Copyright (C) 2014 The Motel on Jupiter
 */

#include "befuture/BeFutureConfig.h"

#include <cstring>

#include "util/config/reader/GLINIReader.h"
#include "util/logging/Logger.h"
#include "util/macro_util.h"

int BeFutureConfig::LoadFromInifile(const char *ini_path) {
  GLINIReader ini(ini_path);
  int ret = ini.ParseError();
  if (-1 == ret) {
    LOGGER.Error("Config-file open error (path: %s)", ini_path);
    return -1;
  } else if (0 != ret) {
    LOGGER.Error("Config-file parse error (path: %s, line: %d)", ini_path, ret);
    return -1;
  }

  // Load the configuration settings
  system.fps = ini.GetInteger("GAMELOOP", "FPS", 0);
  system.fps_update_interval = ini.GetInteger("GAMELOOP", "FPS_UPDATE_INTERVAL",
                                              0);
  system.running_speed = ini.GetFloating("GAMELOOP", "RUNNING_SPEED", 0.0f);
  window.default_pos = ini.GetIntVector2("WINDOW", "DEFAULT_POS",
                                         glm::i32vec2());
  window.default_size = ini.GetIntVector2("WINDOW", "DEFAULT_SIZE",
                                          glm::i32vec2());
  camera.default_pos = ini.GetVector3("CAMERA", "DEFAULT_POS", glm::vec3());
  camera.default_at = ini.GetVector3("CAMERA", "DEFAULT_AT", glm::vec3());
  camera.default_up = ini.GetVector3("CAMERA", "DEFAULT_UP", glm::vec3());
  camera.default_pers = Camera::Perspective(
      ini.GetFloating("CAMERA", "PERS_FOVY", 0.0f),
      static_cast<float>(window.default_size.x)
          / static_cast<float>(window.default_size.y),
      ini.GetFloating("CAMERA", "PERS_NEAR", 0.0f),
      ini.GetFloating("CAMERA", "PERS_FAR", 0.0f));
  camera.default_vport = Camera::Viewport(glm::i32vec2(0), window.default_size);
  ds_camera.translate_speed = ini.GetFloating("DS_CAMERA_CTRL",
                                              "TRANSLATE_SPEED", 0.0f);
  ds_camera.rotate_speed = ini.GetFloating("DS_CAMERA_CTRL", "ROTATE_SPEED",
                                           0.0f);
  ds_camera.zoom_speed = ini.GetFloating("DS_CAMERA_CTRL", "ZOOM_SPEED", 0.0f);
  tweak_menu.default_pos = ini.GetVector2("TWEAK_MENU", "DEFAULT_POS",
                                          glm::vec2());
  tweak_menu.default_size = ini.GetVector2("TWEAK_MENU", "DEFAULT_SIZE",
                                           glm::vec2());
  tweak_menu.color = ini.GetVector3("TWEAK_MENU", "MENU_COLOR", glm::vec3());
  character_storm.num_characters = ini.GetInteger("ACTOR__CHARACTER_STORM",
                                                  "NUM_CHARACTERS", 0);
  character_storm.character_size = ini.GetVector2("ACTOR__CHARACTER_STORM",
                                                  "CHARACTER_SIZE",
                                                  glm::vec2());
  character_storm.character_lifetime = ini.GetFloating("ACTOR__CHARACTER_STORM",
                                                       "CHARACTER_LIFETIME",
                                                       0.0f);
  character_storm.emission_interval = ini.GetFloating("ACTOR__CHARACTER_STORM",
                                                      "EMISSION_INTERVAL",
                                                      2.0f);

  return 0;
}
