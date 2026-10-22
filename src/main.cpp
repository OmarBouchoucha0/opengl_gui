#include <cmath>
#include <glad/glad.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cstdint>
#include <time.h>
#include "base/base_types.cpp"
#include "base/base_math.h"
#include "base/base_log.cpp"
#include "base/base_arena.cpp"
#include "render/render.cpp"

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
  Window *window = createWindow(windowWidth, windowHeight, "open gl");

  Color backgroundColor = {
      0x18,
      0x18,
      0x18,
      0xff,
  };
  Color lineColor = {
      0xff,
      0xff,
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
      .x = windowWidth / 3.0f,
      .y = windowHeight / 3.0f,
      .width = 50.0f,
      .height = 50.0f,
  };

  V2f center = {
      .x = windowWidth / 2.0f,
      .y = windowHeight / 2.0f,
  };

  u64 frequency = glfwGetTimerFrequency();
  u64 startCounter = glfwGetTimerValue();
  u32 fps = 60;
  f32 rot = 0.0f;
  f32 dt = 0.0f;
  char infoBuffer[Kilobytes(10)];

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
    clearBackground(backgroundColor);
    rot += 1.0f;

    // drawLine(startx, endx, lineColor);
    // drawLine(starty, endy, lineColor);
    // drawLineEx(startx, endx, 3.0f, lineColor);
    // drawRectangle(rect, rectColor);
    // drawRectangleOutline(rect, 10.0f, rectColor);
    // drawTrinagle(v1, v2, v3, rectColor);
    drawRectangleEx(rect, {windowWidth / 2.0f, windowHeight / 2.0f}, rot,
                    rectColor);

    // drawTrinagleOutline(v1, v2, v3, 5.0f, rectColor);
    // drawPolygon(center, 8, 50, polyColor);
    // drawCircle(center, 50.0f, rectColor);
    // drawCircleOutline(center, 50.0f, rectColor);
    //
    lockFps(fps, frequency, &startCounter, &dt);
    // snprintf(infoBuffer, sizeof(infoBuffer), "DELTA TIME = %.5fs", dt);
    // traceLog(DEBUG, infoBuffer);
  }

  destroyWindow(window);
  exit(EXIT_SUCCESS);
}
