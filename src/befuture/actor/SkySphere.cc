/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "befuture/actor/SkySphere.h"

#include <glm/gtc/type_ptr.hpp>
#include <SDL_image.h>
#include "util/logging/Logger.h"

SkySphere::SkySphere()
    : sphere_(nullptr),
      texture_(nullptr),
      tex_name_(0) {
}

SkySphere::~SkySphere() {
}

int SkySphere::Initialize() {
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  // Set up the sphere object
  sphere_ = gluNewQuadric();
  if (nullptr == sphere_) {
    LOGGER.Error("Failed to create the quadric model");
    return -1;
  }
  gluQuadricDrawStyle(sphere_, GLU_FILL);
  gluQuadricTexture(sphere_, GL_TRUE);
  gluQuadricNormals(sphere_, GLU_SMOOTH);
  gluQuadricOrientation(sphere_, GLU_OUTSIDE);

  // Load the texture data from file
  static const char *kTexturePath = "share/texture/sky.png";
  texture_ = IMG_Load(kTexturePath);
  if (nullptr == texture_) {
    LOGGER.Error("Failed to load texture (path: %s)", kTexturePath);
    return -1;
  }

  // Set up the texture
  glGenTextures(1, &tex_name_);
  glBindTexture(GL_TEXTURE_2D, tex_name_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_->w, texture_->h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, texture_->pixels);

  return 0;
}

int SkySphere::Finalize() {
  if (nullptr != texture_) {
    SDL_FreeSurface(texture_);
    texture_ = nullptr;
  }
  if (nullptr != sphere_) {
    gluDeleteQuadric(sphere_);
    sphere_ = nullptr;
  }
  return 0;
}

void SkySphere::Render() {
  // Prepare the rendering
  glEnable (GL_CULL_FACE);
  glCullFace (GL_FRONT);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  // Set the texture
  glActiveTexture (GL_TEXTURE0);
  glEnable (GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex_name_);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);

  // Set the vertices
  gluSphere(sphere_, 1200.0f, 16, 16);

  // Restore the flags
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
}
