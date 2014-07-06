/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include <cstdio>
#include <ctime>
#include <map>
#include <sstream>

#include <GL/freeglut.h>
#include <GL/glew.h>
#include <SDL_image.h>

#include "befuture/BeFutureConfig.h"
#include "befuture/BeFutureContext.h"
#include "befuture/BeFutureGame.h"
#include "befuture/BeFutureTweakMenu.h"
#include "befuture/actor/CharacterStorm.h"
#include "befuture/actor/ColorBar.h"
#include "camera/camera.h"
#include "camera/controller/DesignerCameraController.h"
#include "peripheral/opengl/OpenGLKeyboardListener.h"
#include "peripheral/opengl/OpenGLMouseListener.h"
#include "util/logging/Logger.h"
#include "util/system/FPSCounter.h"
#include "util/wrapper/glm_wrap.h"
#include "util/macro_util.h"

/**
 * The constants for the setting
 */

// The config settings
static BeFutureContext context;
static BeFutureGame game;
static int prev_time_ms = 0;

/**
 * The callback function called every fixed time
 */
static void OnTimer(int value) {
  UNUSED(value);

  int current_time_ms = glutGet(GLUT_ELAPSED_TIME);
  float current_time_sec = static_cast<float>(current_time_ms) / 1000.0f;
  float sys_frame_time_sec = static_cast<float>(current_time_ms - prev_time_ms)
      / 1000.0f;
  float app_frame_time_sec = sys_frame_time_sec * context.system.running_speed;
  if (context.system.pause) {
    app_frame_time_sec = 0.0f;
  }
  prev_time_ms = current_time_ms;

  // Update the application time in the context
  context.system.boot_sec = current_time_sec;
  context.system.app_sec += app_frame_time_sec;

  // Update the FPS counter
  context.system.fps_counter->Update(current_time_ms);
  context.system.current_fps = context.system.fps_counter->fps();

  // Update the game
  game.Update(context, app_frame_time_sec);

  // Update the camera
  context.rendering.camera_controller->Update();

  // Update the system
  glutSetWindow(context.window.id);
  glutPostRedisplay();
  glutTimerFunc(1000 / context.system.ideal_fps, OnTimer, 0);
}

/**
 * The callback function for the mouse click reaction
 */
static void OnMouseClick(int button, int state, int x, int y) {
  context.rendering.camera_controller->EventMouse(button, state, x, y);

}

/**
 * The callback function for the mouse wheel reaction
 */
static void OnMouseWheel(int wheel, int direction, int x, int y) {
  context.rendering.camera_controller->EventMouseWheel(wheel, direction, x, y);
}

/**
 * The callback function for the mouse moving reaction
 */
static void OnMouseMove(int x, int y) {
  context.rendering.camera_controller->EventMouseMotion(x, y);
}

/**
 * The callback function to display
 */
static void OnDisplay(void) {
  game.Draw(context);
}

/**
 * main (the entry point)
 */
int BeFutureMain(int argc, char *argv[], const char *config_path) {
  int error = 0;

  // Initialize the SDL-image
  int ret = IMG_Init(IMG_INIT_PNG);
  if (0 == ret) {
    LOGGER.Error("Failed to initialize SDL-image (error: %s)", IMG_GetError());
    return -1;
  }

  // Create the game context
  if (0 > context.LoadFromInifile(config_path)) {
    LOGGER.Error("Failed to load the configuration from ini-file (path: %s)",
                 config_path);
    return -1;
  }

  // Initialize the GLUT library and Create the Window
  glutInitWindowPosition(context.window.pos.x, context.window.pos.y);
  glutInitWindowSize(context.window.size.x, context.window.size.y);
  glutInit(&argc, argv);
  context.window.id = glutCreateWindow("SIGNAL - The Motel On Jupiter");
  if (0 >= context.window.id) {
    LOGGER.Error("Failed to create the window (ret: %d)", context.window.id);
    return -1;
  }
  // GLUT_DOUBLE: Enable double buffer
  // GLUT_DEPTH: Enable to use Z buffer
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  // Initialize the GLEW library
  GLenum glew_status = glewInit();
  if (GLEW_OK != glew_status) {
    LOGGER.Error("Failed to initiaize GLEW library (errmsg: %s)",
                 glewGetErrorString(glew_status));
    return -1;
  }

  // Initialize the tweak menu
  if (0 > BEFUTURE_TWEAK_MENU.Initialize(context)) {
    LOGGER.Error("Failed to initialize the tweak menu");
    return -1;
  }

  // Initialize the game
  game.Initialize(context);

  // Set the callback functions
  glutDisplayFunc(OnDisplay);
  glutTimerFunc(1000 / context.system.ideal_fps, OnTimer, 0);
  OpenGLKeyboardListener::RegisterAllCallbackFuncs();
  OpenGLMouseListener::PushButtonHook(OnMouseClick);
  OpenGLMouseListener::PushMoveHook(OnMouseMove);
  OpenGLMouseListener::PushPassiveMoveHook(OnMouseMove);
  OpenGLMouseListener::PushWheelHook(OnMouseWheel);
  OpenGLMouseListener::RegisterAllCallbackFuncs();

  // Execute the main loop
  LOGGER.Info("Start the main loop");
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glutMainLoop();

  // Finalize the game
  game.Finalize(context);

  // Finalize the tweak menu
  if (0 > BEFUTURE_TWEAK_MENU.Finalize()) {
    LOGGER.Error("Failed to finalize the tweak menu");
    error = -1;
  }

  // Finalize the context
  context.Unload();

  return 0;
}
