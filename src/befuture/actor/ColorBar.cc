/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "befuture/actor/ColorBar.h"

#include <SDL_image.h>

#include "util/logging/Logger.h"

ColorBar::ColorBar()
    : texture_(nullptr),
      texname_(0) {
}

ColorBar::~ColorBar() {
  Finalize();
}

int ColorBar::Initialize(glm::vec2 pos, glm::vec2 scale) {
  // Load the texture data from file
  static const char *kTexturePath = "share\\texture\\colorbar.bmp";
  texture_ = IMG_Load(kTexturePath);
  if (nullptr == texture_) {
    LOGGER.Error("Failed to load texture (path: %s)", kTexturePath);
    return -1;
  }

  // Set up the texture
  glGenTextures(1, &texname_);
  glBindTexture(GL_TEXTURE_2D, texname_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_->w, texture_->h, 0, GL_RGB,
               GL_UNSIGNED_BYTE, texture_->pixels);

  pos_ = pos;
  scale_ = scale;

  return 0;
}

int ColorBar::Finalize() {
  if (nullptr != texture_) {
    SDL_FreeSurface(texture_);
    texture_ = nullptr;
  }
  return 0;
}

void ColorBar::Render(glm::vec2 windowSize) {
  glUseProgram(0);
  glDisable (GL_LIGHTING);
  glEnable (GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texname_);

  glDisable (GL_DEPTH_TEST);
  glDisable (GL_CULL_FACE);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  glBegin (GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(pos_.x / windowSize.x * 2.0f - 1.0f,
             pos_.y / windowSize.y * 2.0f - 1.0f, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(pos_.x / windowSize.x * 2.0f - 1.0f,
             (pos_.y + texture_->h * scale_.y) / windowSize.y * 2.0f - 1.0f,
             0.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f((pos_.x + texture_->w * scale_.x) / windowSize.x * 2.0f - 1.0f,
             (pos_.y + texture_->h * scale_.y) / windowSize.y * 2.0f - 1.0f,
             0.0f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f((pos_.x + texture_->w * scale_.x) / windowSize.x * 2.0f - 1.0f,
             pos_.y / windowSize.y * 2.0f - 1.0f, 0.0f);
  glEnd();
}

