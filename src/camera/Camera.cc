/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "camera/Camera.h"

Camera::Camera(const glm::vec3& default_pos, const glm::vec3& default_at,
               const glm::vec3& default_up, const Perspective &default_pers,
               const Viewport &default_vport)
    : pos_(default_pos),
      at_(default_at),
      up_(glm::normalize(default_up)),
      pers_(default_pers),
      vport_(default_vport) {
}

Camera::~Camera() {
}
