#include <cmath>
#include <glad/glad.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cstdint>
#include <time.h>
#include "base/base_strings.cpp"
#include "base/base_types.cpp"
#include "base/base_math.h"
#include "base/base_log.cpp"
#include "base/base_arena.cpp"
#include "render/render.cpp"
#include "gui/gui.cpp"

void lockFps(u32 fps, u64 frequency, u64 *startCounter, f32 *dt) {
  u64 endCounter = glfwGetTimerValue();
  f64 elapsedMs = (f64)(endCounter - *startCounter) * 1000.0 / (f64)frequency;
  local_persist f32 targetMilliSecondsPerFrame = 1000.0f / (f32)fps;

  if (elapsedMs < targetMilliSecondsPerFrame) {
    f64 sleepMs = targetMilliSecondsPerFrame - elapsedMs - 1.0;
    if (sleepMs > 0.0) {
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = (long)(sleepMs * 1e6);
      nanosleep(&ts, NULL);
    }
    while (elapsedMs < targetMilliSecondsPerFrame) {
      endCounter = glfwGetTimerValue();
      elapsedMs = (f64)(endCounter - *startCounter) * 1000.0 / (f64)frequency;
    }
  }
  *dt = elapsedMs / 1000.0f;
  *startCounter = endCounter;
}

int main(void) {
  i32 windowWidth = 800;
  i32 windowHeight = 450;
  Window window = createWindow(windowWidth, windowHeight, "open gl");

  Color backgroundColor = {
      0x18,
      0x18,
      0x18,
      0xff,
  };
  Color white = {
      0xff,
      0xff,
      0xff,
      0xff,
  };
  Color red = {
      0xff,
      0x00,
      0x00,
      0xff,
  };
  Color green = {
      0x00,
      0xff,
      0x00,
      0xff,
  };
  Color blue = {
      0x00,
      0x00,
      0xff,
      0xff,
  };
  Color rectColor = {
      0x11,
      0x05,
      0xff,
      0xff,
  };
  Color polyColor = {
      0x11,
      0xff,
      0x05,
      0xff,
  };
  V2f v1 = {
      .x = 200.0f,
      .y = 200.0f,
  };
  V2f v2 = {
      .x = 200.0f,
      .y = 400.0f,
  };
  V2f v3 = {
      .x = 100.0f,
      .y = 300.0f,
  };
  V2f startx = {
      .x = (f32)windowWidth / 2.0f,
      .y = 0,
  };
  V2f endx = {
      .x = (f32)windowWidth / 2.0f,
      .y = (f32)windowHeight,
  };

  Rectangle rect = {
      .x = windowWidth / 5.0f,
      .y = windowHeight / 4.0f,
      .width = 200.0f,
      .height = 100.0f,
  };

  V2f center = {
      .x = windowWidth / 2.0f,
      .y = windowHeight / 2.0f,
  };

  V2f start = {
      .x = 100.0f,
      .y = 100.0f,
  };

  V2f end = {
      .x = 400.0f,
      .y = 100.0f,
  };

  u64 frequency = glfwGetTimerFrequency();
  u64 startCounter = glfwGetTimerValue();
  u32 fps = 60;
  f32 rot = 0.0f;
  f32 dt = 0.0f;
  char infoBuffer[Kilobytes(10)];
  char infoBuffer2[Kilobytes(10)];
  Font font =
      loadFont("/home/omar/Work/opengl/assets/JetBrainsMono-Regular.ttf", 32);

  Color buttonColor = blue;
  Color textColor = backgroundColor;
  i32 i = 0;
  snprintf(infoBuffer, sizeof(infoBuffer), "IDLE");
  while (!windowShouldClose()) {

    clearBackground(backgroundColor);
    rot += 1.0f;

    snprintf(infoBuffer2, sizeof(infoBuffer2), "i : %d", i);
    drawText(center, infoBuffer2, &font, red);
    switch (guiButton(rect, infoBuffer, &font, buttonColor, textColor)) {
    case BUTTON_IDLE: {
      snprintf(infoBuffer, sizeof(infoBuffer), "IDLE");
    } break;
    case BUTTON_HOVERED: {
      snprintf(infoBuffer, sizeof(infoBuffer), "HOVERED");
    } break;
    case BUTTON_DOWN: {
      snprintf(infoBuffer, sizeof(infoBuffer), "DOWN");
    } break;
    case BUTTON_RELEASED: {
      i++;
    } break;
    }
    lockFps(fps, frequency, &startCounter, &dt);
    // snprintf(infoBuffer, sizeof(infoBuffer), "DELTA TIME = %.5fs", dt);
    // traceLog(DEBUG, infoBuffer);

    // snprintf(infoBuffer, sizeof(infoBuffer), "MOUSE POS = {%.5f|%.5f}",
    //          MOUSE_POSITION.x, MOUSE_POSITION.y);
    // traceLog(DEBUG, infoBuffer);
    handleEvents();
    MOUSE_LEFT_BUTTON_STATE = MOUSE_IDLE;
  }

  destroyWindow();
  exit(EXIT_SUCCESS);
}
