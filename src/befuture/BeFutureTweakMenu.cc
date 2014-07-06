/**
 * Copyright (C) 2014 The Motel On Jupiter
 */

#include "befuture/BeFutureTweakMenu.h"

#include <AntTweakBar.h>

#include "befuture/actor/CharacterStorm.h"
#include "befuture/BeFutureContext.h"
#include "camera/Camera.h"
#include "peripheral/opengl/OpenGLKeyboardListener.h"
#include "peripheral/opengl/OpenGLMouseListener.h"
#include "util/macro_util.h"
#include "util/logging/Logger.h"

static void TW_CALL OnAppTerminate(void *clientData) {
  UNUSED(clientData);

  glutLeaveMainLoop();
}

static void TW_CALL OnCharacterStormClearCharacters(void *clientData) {
  static_cast<BeFutureContext *>(clientData)->sequence.char_storm
      ->ClearCharacters();
}

int BeFutureTweakMenu::Initialize(BeFutureContext &context) {
  // Initialize for the AntTweakBar library
  if (0 == TwInit(TW_OPENGL, nullptr)) {
    LOGGER.Error("Failed to initialize AntTweakBar");
    return -1;
  }
  twbar_ = TwNewBar("TweakMenu");

  // Create a tweak bar
  TwDefine(
      "TweakMenu position='710 10' size='200 640' color='41 126 231' iconified=true");

  // Add the items for application
  TwAddVarRO(twbar_, "Application_BootSeconds", TW_TYPE_FLOAT,
             &(context.system.boot_sec),
             "label='BootTime' group='Application'");
  TwAddVarRO(twbar_, "Application_ApplicationSeconds", TW_TYPE_FLOAT,
             &(context.system.app_sec),
             "label='ApplicationTime' group='Application'");
  TwAddVarRO(twbar_, "Application_CurrentFPS", TW_TYPE_INT32,
             &(context.system.current_fps),
             "label='CurrentFPS' group='Application'");
  TwAddVarRW(twbar_, "Application_IdealFPS", TW_TYPE_INT32,
             &(context.system.ideal_fps),
             "label='IdealFPS' group='Application'");
  TwAddVarRW(twbar_, "Application_Pause", TW_TYPE_BOOLCPP,
             &(context.system.pause),
             "label='Pause' group='Application' key=p true='on' false='off'");
  TwAddVarRW(twbar_, "Application_RunningSpeed", TW_TYPE_FLOAT,
             &(context.system.running_speed),
             "label='RunningSpeed' group='Application'");
  TwAddVarRW(
      twbar_,
      "Application_DebugEntities",
      TW_TYPE_BOOLCPP,
      &(context.system.debug_draw),
      "label='DebugObjects' group='Application' true='render-on' false='render-off'");
  TwAddButton(twbar_, "Application_Terminate", OnAppTerminate, nullptr,
              "label='Terminate' group='Application' key=q");

  // Add the items for camera
  TwAddVarRW(twbar_, "CameraGeneral_Pos", TW_TYPE_DIR3F,
             context.rendering.camera->pos_ptr(),
             "label='Pos' group='CameraGeneral'");
  TwAddVarRW(twbar_, "CameraGeneral_At", TW_TYPE_DIR3F,
             context.rendering.camera->at_ptr(),
             "label='At' group='CameraGeneral'");
  TwAddVarRW(twbar_, "CameraGeneral_Up", TW_TYPE_DIR3F,
             context.rendering.camera->up_ptr(),
             "label='Up' group='CameraGeneral'");
  TwAddVarRW(twbar_, "CameraPerspective_Aspect", TW_TYPE_FLOAT,
             &(context.rendering.camera->mutable_pers().aspect),
             "label='Aspect' group='CameraPerspective'");
  TwAddVarRW(twbar_, "CameraPerspective_Fovy", TW_TYPE_FLOAT,
             &(context.rendering.camera->mutable_pers().fovy),
             "label='Fovy' group='CameraPerspective'");
  TwAddVarRW(twbar_, "CameraPerspective_ZFar", TW_TYPE_FLOAT,
             &(context.rendering.camera->mutable_pers().zfar),
             "label='ZFar' group='CameraPerspective'");
  TwAddVarRW(twbar_, "CameraPerspective_ZNear", TW_TYPE_FLOAT,
             &(context.rendering.camera->mutable_pers().znear),
             "label='ZNear' group='CameraPerspective'");
  TwAddVarRW(twbar_, "CameraViewport_SizeX", TW_TYPE_UINT32,
             &(context.rendering.camera->mutable_vport().size.x),
             "label='SizeX' group='CameraViewport'");
  TwAddVarRW(twbar_, "CameraViewport_SizeY", TW_TYPE_UINT32,
             &(context.rendering.camera->mutable_vport().size.y),
             "label='SizeY' group='CameraViewport'");

  // Add the item for character storm
  TwAddVarRO(twbar_, "CharacterStorm_CurrentCharacters", TW_TYPE_INT32,
             &(context.sequence.num_characters),
             "label='CurrentCharacters' group='CharacterStorm'");
  TwAddButton(twbar_, "CharacterStorm_ClearCharacters",
              OnCharacterStormClearCharacters, &context,
              "label='ClearCharacters' group='CharacterStorm'");
  TwAddVarRW(twbar_, "CharacterStorm_EmissionInterval", TW_TYPE_FLOAT,
             &(context.sequence.emission_interval),
             "label='EmissionInterval' group='CharacterStorm'");
  TwAddVarRW(twbar_, "CharacterStorm_EmissionActive", TW_TYPE_BOOLCPP,
             &(context.sequence.emission_active),
             "label='EmissionActive' group='CharacterStorm'");
  TwAddVarRW(twbar_, "CharacterStorm_CharacterSizeX", TW_TYPE_FLOAT,
             &(context.sequence.character_size.x),
             "label='CharacterSizeX' group='CharacterStorm'");
  TwAddVarRW(twbar_, "CharacterStorm_CharacterSizeY", TW_TYPE_FLOAT,
             &(context.sequence.character_size.y),
             "label='CharacterSizeY' group='CharacterStorm'");
  TwAddVarRW(twbar_, "CharacterStorm_CharacterLifetime", TW_TYPE_FLOAT,
             &(context.sequence.character_lifetime),
             "label='CharacterLifetime' group='CharacterStorm'");

  // Register the hook functions
  OpenGLKeyboardListener::PushKeyPushHook(
      (OpenGLKeyboardListener::KeyPushHookFunc) TwEventKeyboardGLUT);
  OpenGLKeyboardListener::PushSpecialKeyPushHook(
      (OpenGLKeyboardListener::SpecialKeyPushHookFunc) TwEventSpecialGLUT);
  OpenGLMouseListener::PushButtonHook(
      (OpenGLMouseListener::ButtonHookFunc) TwEventMouseButtonGLUT);
  OpenGLMouseListener::PushMoveHook(
      (OpenGLMouseListener::MoveHookFunc) TwEventMouseMotionGLUT);

  // Register the function to get the status of the modifier keys
  TwGLUTModifiersFunc(glutGetModifiers);

  // Tell the window size to the tweak menu
  if (0 == TwWindowSize(context.window.size.x, context.window.size.y)) {
    LOGGER.Error("Failed to set the window size (error: %s)", TwGetLastError());
  }

  return 0;
}

int BeFutureTweakMenu::Finalize() {
  if (nullptr == twbar_) {
    return 1;
  }

  if (0 == TwDeleteAllBars()) {
    LOGGER.Error("Failed to delete all bars");
    return -1;
  }
  twbar_ = nullptr;

  if (0 == TwTerminate()) {
    LOGGER.Error("Failed to terminate AntTweakBar");
    return -1;
  }
  return 0;
}

int BeFutureTweakMenu::UpdateWindowSize(int width, int height) {
  if (0 == TwWindowSize(width, height)) {
    LOGGER.Error("Failed to request for AntTweakBar to resize the window");
    return -1;
  }
  return 0;
}

int BeFutureTweakMenu::Render() {
  if (0 == TwDraw()) {
    LOGGER.Error("Failed to request for AntTweakBar to draw");
    return -1;
  }
  return 0;
}
