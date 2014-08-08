/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "befuture/actor/CharacterStorm.h"

#include <SDL_image.h>

#include "util/wrapper/spark_wrap.h"
#include "util/logging/Logger.h"

CharacterStorm::CharacterStorm()
    : texture_(nullptr),
      texname_(0),
      renderer_(),
      system_(),
      group_(),
      emitter_() {
}

CharacterStorm::~CharacterStorm() {
  Finalize();
}

int CharacterStorm::Initialize(int num_chars, float char_lifetime,
                               float emission_interval, glm::vec2 char_size) {
  // Load the texture data from file
  static const char *kTexturePath = "share/texture/characters.bmp";
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

  SPK::System::useRealStep();
  system_ = SPK::System::create(true);

  group_ = system_->createGroup(num_chars);
  group_->setLifeTime(char_lifetime, char_lifetime);
  group_->setRadius(0.6f);
  renderer_ = SPK::GL::GLQuadRenderer::create(char_size.x, char_size.y);
  renderer_->setTexturingMode(SPK::TEXTURE_MODE_2D);
  renderer_->setTexture(texname_);
  renderer_->setAtlasDimensions(5, 2);
  renderer_->setBlendMode(SPK::BLEND_MODE_ADD);
  renderer_->enableRenderingOption(SPK::RENDERING_OPTION_DEPTH_WRITE, false);
  renderer_->enableRenderingOption(SPK::RENDERING_OPTION_ALPHA_TEST, true);
  renderer_->setAlphaTestThreshold(0.0f);
  renderer_->setShared(true);
  group_->setRenderer(renderer_);
  group_->setParamInterpolator(SPK::PARAM_ROTATION_SPEED,
                               SPK::FloatRandomInitializer::create(0.0f, 2.0f));
  group_->setParamInterpolator(
      SPK::PARAM_ANGLE,
      SPK::FloatRandomInitializer::create(0.0f, 2.0f * glm::pi<float>()));
  group_->setParamInterpolator(
      SPK::PARAM_TEXTURE_INDEX,
      SPK::FloatRandomInitializer::create(
          0.0f, static_cast<float>(kNumberOfTexture)));
  group_->addModifier(SPK::Rotator::create());

  emitter_ = SPK::SphericEmitter::create(
      SPK::Vector3D(0.0f, 1.0f, 0.0f), 0.0f, glm::radians(8.0f),
      SPK::Point::create(SPK::Vector3D(0.0f, -100.0f, 0.0f)), true, -1,
      1.0f / emission_interval, 5.0f, 5.0f);
  group_->addEmitter(emitter_);
  SPK::Ref<SPK::Vortex> vortex = SPK::Vortex::create(
      SPK::Vector3D(0.0f, 0.0f, 0.0f), SPK::Vector3D(0.0f, 1.0f, 0.0f), 4.0f,
      -2.0f);
  vortex->setRotationSpeedAngular(true);
  vortex->setAttractionSpeedLinear(false);
  group_->addModifier(vortex);

  for (int i = 0; i < 1000; ++i) {
    system_->updateParticles(1000.0f / 60.0f * 0.2f);
  }

  return 0;
}

int CharacterStorm::Finalize() {
  if (nullptr != texture_) {
    SDL_FreeSurface(texture_);
    texture_ = nullptr;
  }
  return 0;
}

void CharacterStorm::Update(float elapsed_time) {
  system_->updateParticles(elapsed_time);
}

void CharacterStorm::Render() {
  SPK::GL::GLRenderer::saveGLStates();
  glEnable (GL_POLYGON_SMOOTH);
  glEnable (GL_DEPTH_TEST);
  glDisable (GL_CULL_FACE);
  system_->renderParticles();
  SPK::GL::GLRenderer::restoreGLStates();
}

int CharacterStorm::GetNumCharacters() const {
  return group_->getNbParticles();
}

void CharacterStorm::SetMaxCharacters(int max) {
  emitter_->setTank(max);
}

int CharacterStorm::GetMaxCharacters() const {
  return emitter_->getCurrentTank();
}

int CharacterStorm::ClearCharacters() {
  for (size_t i = 0; i < group_->getNbParticles(); ++i) {
    group_->getParticle(i).kill();
  }
  return 0;
}

int CharacterStorm::SetEmissionInterval(float interval) {
  emitter_->setFlow(1.0f / interval);
  return 0;
}

int CharacterStorm::InactivateEmission(bool inactive) {
  emitter_->setActive(!inactive);
  return 0;
}

bool CharacterStorm::IsEmissionActive() const {
  return emitter_->isActive();
}

void CharacterStorm::SetCharacterSize(glm::vec2 size) {
  renderer_->setScale(size.x, size.y);
}

glm::vec2 CharacterStorm::GetCharacterSize() const {
  return glm::vec2(renderer_->getScaleX(), renderer_->getScaleY());
}

void CharacterStorm::SetCharacterLifetime(float time) {
  group_->setLifeTime(time, time);
}

float CharacterStorm::GetCharacterLifetime() const {
  return group_->getMinLifeTime();
}
