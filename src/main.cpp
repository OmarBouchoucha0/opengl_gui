#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cstdint>

#define NormalizeColorChannel(color) ((float)(color) / 255.0f)

#define global_variable static
#define local_persist static
#define internal static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

struct Color {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

internal void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

internal void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

internal void clearBackground(Color color) {
  glClearColor(NormalizeColorChannel(color.r), NormalizeColorChannel(color.g),
               NormalizeColorChannel(color.b), NormalizeColorChannel(color.a));
  glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  i32 windowWidth = 800;
  i32 windowHeight = 600;
  GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight,
                                        "graphics library", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glViewport(0, 0, windowWidth, windowHeight);

  Color backgroundColor = {
      .r = 100,
      .g = 100,
      .b = 100,
      .a = 255,
  };

  while (!glfwWindowShouldClose(window)) {

    clearBackground(backgroundColor);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
