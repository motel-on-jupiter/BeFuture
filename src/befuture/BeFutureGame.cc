/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "BeFutureGame.h"

#include <GL/freeglut.h>
#include <GL/glew.h>
#include <SDL_image.h>

#include "befuture/actor/CharacterStorm.h"
#include "befuture/actor/ColorBar.h"
#include "befuture/actor/SkySphere.h"
#include "befuture/BeFutureContext.h"
#include "befuture/BeFutureTweakMenu.h"
#include "util/graphics/shader_utils.h"
#include "util/logging/Logger.h"

BeFutureGame::BeFutureGame()
    : initialized_(false),
      framebuf_(0),
      renderbuf_(0),
      shader_(0) {
}

BeFutureGame::~BeFutureGame() {
}

int BeFutureGame::Initialize(BeFutureContext &context) {
  // Load the configuration from file
  LOGGER.Info("Set up the application");

  // Initialize the sky
  context.sequence.sky = new SkySphere();
  if (nullptr == context.sequence.sky) {
    LOGGER.Error("Failed to allocate the sky instance");
    return -1;
  }
  int ret = context.sequence.sky->Initialize();
  if (0 > ret) {
    LOGGER.Error("Failed to initialize the sky (%d)", ret);
    return -1;
  }

  // Initialize the character storm
  context.sequence.char_storm = new CharacterStorm();
  if (nullptr == context.sequence.char_storm) {
    LOGGER.Error("Failed to allocate the character storm instance");
    return -1;
  }
  ret = context.sequence.char_storm->Initialize(
      context.config.character_storm.num_characters,
      context.config.character_storm.character_lifetime,
      context.config.character_storm.emission_interval,
      context.config.character_storm.character_size);
  if (0 > ret) {
    LOGGER.Error("Failed to initialize the character storm (%d)", ret);
    return -1;
  }

  // Initialize the color bar
  context.sequence.color_bar = new ColorBar();
  if (nullptr == context.sequence.color_bar) {
    LOGGER.Error("Failed to allocate the color bar instance");
    return -1;
  }
  ret = context.sequence.color_bar->Initialize(glm::vec2(550.0f, 590.0f),
                                               glm::vec2(1.2f, 0.25f));
  if (0 > ret) {
    LOGGER.Error("Failed to initialize the color bar (%d)", ret);
    return -1;
  }

  // Create the frame buffer (for multi-pass rendering)
  glGenFramebuffers(1, &framebuf_);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf_);

  glGenRenderbuffers(1, &renderbuf_);
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuf_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        context.window.size.x, context.window.size.y);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
  GL_RENDERBUFFER,
                            renderbuf_);

  // Create a RGBA color texture
  glGenTextures(1, &colortexs_[0]);
  glBindTexture(GL_TEXTURE_2D, colortexs_[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, context.window.size.x,
               context.window.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colortexs_[0],
                       0);
  glGenTextures(1, &colortexs_[1]);
  glBindTexture(GL_TEXTURE_2D, colortexs_[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, context.window.size.x,
               context.window.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, colortexs_[1],
                       0);

  GLenum fbstatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  if (fbstatus != GL_FRAMEBUFFER_COMPLETE) {
    LOGGER.Error("Failed to check frame buffer all right");
    return -1;
  }

  for (int i = 0; i < ARRAYSIZE(colortexs_); ++i) {
    glBindTexture(GL_TEXTURE_2D, colortexs_[i]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  // Compile the vertex shader program
  shader_ = create_shader_program("src/shader/v_blured_character_storm.glsl",
                                  "src/shader/f_blured_character_storm.glsl");
  if (0 == shader_) {
    LOGGER.Error("Failed to create shader program (ret: %d)");
    return -1;
  }

  initialized_ = true;

  return 0;
}

void BeFutureGame::Finalize(BeFutureContext &context) {
  if (!initialized_) {
    return;
  }

  LOGGER.Info("Clean up the application");

  delete_shader_program(shader_);
  shader_ = 0;

  glDeleteTextures(ARRAYSIZE(colortexs_), colortexs_);
  memset(colortexs_, 0, sizeof(colortexs_));
  glDeleteRenderbuffers(1, &renderbuf_);
  renderbuf_ = 0;
  glDeleteFramebuffers(1, &framebuf_);
  framebuf_ = 0;

  // Finalize and clean up the actors
  if (nullptr != context.sequence.char_storm) {
    context.sequence.char_storm->Finalize();
    delete context.sequence.char_storm;
    context.sequence.char_storm = nullptr;
  }
  if (nullptr != context.sequence.color_bar) {
    context.sequence.color_bar->Finalize();
    delete context.sequence.color_bar;
    context.sequence.color_bar = nullptr;
  }
  if (nullptr != context.sequence.sky) {
    context.sequence.sky->Finalize();
    delete context.sequence.sky;
    context.sequence.sky = nullptr;
  }

  return;
}

void BeFutureGame::Update(BeFutureContext &context, float elapsed_time) {
  if (!initialized_) {
    return;
  }

  if (elapsed_time > FLT_EPSILON) {
    context.sequence.char_storm->Update(elapsed_time);
  }

  context.sequence.char_storm->SetEmissionInterval(
      context.sequence.emission_interval);
  context.sequence.char_storm->InactivateEmission(
      !context.sequence.emission_active);
  context.sequence.char_storm->SetCharacterSize(
      context.sequence.character_size);
  context.sequence.char_storm->SetCharacterLifetime(
      context.sequence.character_lifetime);

  context.sequence.num_characters = context.sequence.char_storm
      ->GetNumCharacters();
  context.sequence.character_lifetime = context.sequence.char_storm
      ->GetCharacterLifetime();
  context.sequence.emission_active = context.sequence.char_storm
      ->IsEmissionActive();
}

int BeFutureGame::Draw(BeFutureContext &context) {
  if (!initialized_) {
    return 1;
  }

  int error = 0;

  glShadeModel(GL_SMOOTH);
  glViewport(context.rendering.camera->vport().pos.x,
             context.rendering.camera->vport().pos.y,
             context.rendering.camera->vport().size.x,
             context.rendering.camera->vport().size.y);

  glMatrixMode(GL_PROJECTION);
  glm::mat4 proj = context.rendering.camera->CalcProjMtx();
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(proj));

  glMatrixMode(GL_MODELVIEW);
  glm::mat4 view = context.rendering.camera->CalcViewMtx();
  glLoadIdentity();
  glLoadMatrixf(glm::value_ptr(view));

  static const GLfloat quad_vertices[4][3] = { { -1.0f, -1.0f, 0.0f }, { -1.0f,
      1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, };
  static const GLfloat tex_coords[4][3] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, {
      1.0f, 1.0f }, { 1.0f, 0.0f }, };

  if (!context.system.pause) {
    // Render the character storm to texture 0
    // (char_storm -> texture0)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf_);
    GLenum attachment0[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachment0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    context.sequence.char_storm->Render();

    // Render the character storm effected blur to texture 1
    // (texture0 * 0.5 + texture1 * 0.5 -> texture1)
    glUseProgram(shader_);
    glUniform1i(glGetUniformLocation(shader_, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shader_, "texture1"), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colortexs_[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colortexs_[1]);
    glActiveTexture(GL_TEXTURE0);
    GLenum attachment1[] = { GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(1, attachment1);

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; ++i) {
      glTexCoord2fv(tex_coords[i]);
      glVertex3fv(quad_vertices[i]);
    }
    glEnd();

    glUseProgram(0);
  }

  // Render to front buffer
  // (texture1 + texture2 + color_bar -> front)
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(
      glm::value_ptr(
          glm::lookAt(glm::vec3(-37.84f, 1341.94f, -170.35f),
                      glm::vec3(97.90f, 117.54f, 57.14f),
                      glm::vec3(0.70f, 0.66f, 0.25f))));
  context.sequence.sky->Render();

  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, colortexs_[1]);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glTexCoord2fv(tex_coords[i]);
    glVertex3fv(quad_vertices[i]);
  }
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);

  glUseProgram(0);
  context.sequence.color_bar->Render(context.window.size);

  // Draw tweak bars
  if (0 > BEFUTURE_TWEAK_MENU.Render()) {
    LOGGER.Error("Failed to render the tweak menu");
    error = -1;
  }

  glutSwapBuffers();

  return error;
}
