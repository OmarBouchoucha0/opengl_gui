#include <cmath>
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define ARENA_IMPLEMENTATION
#include "arena.h"

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cstdint>

#define NormalizeColorChannel(color) ((float)(color) / 255.0f)

#define global_variable static
#define local_persist static
#define internal static

#define GPU_BUFFER_SIZE 4096
#define PI 3.14159265359f

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

struct V2 {
  i32 x;
  i32 y;
};

struct V2f {
  f32 x;
  f32 y;
};

struct Rectangle {
  f32 x;
  f32 y;
  f32 width;
  f32 height;
};

struct GpuBuffers {
  u32 VAO;
  u32 VBO;
  u32 EAO;
};

typedef GLFWwindow Window;

global_variable const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

global_variable const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";

internal void errorCallback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

internal void init() {
  glfwSetErrorCallback(errorCallback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

internal void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

internal Window *createWindow(u32 windowWidth, u32 windowHeight,
                              const char *title) {
  Window *window = glfwCreateWindow(windowWidth, windowHeight,
                                    "graphics library", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, windowWidth, windowHeight);
  return window;
}

internal void destroyWindow(Window *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

internal void loadShaders(u32 vertexShader, u32 fragmentShader,
                          u32 *shaderProgram) {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  *shaderProgram = glCreateProgram();
  glAttachShader(*shaderProgram, vertexShader);
  glAttachShader(*shaderProgram, fragmentShader);
  glLinkProgram(*shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

internal void initGpu(GpuBuffers *gpuBuffers) {
  glGenVertexArrays(1, &gpuBuffers->VAO);
  glGenBuffers(1, &gpuBuffers->VBO);
  glGenBuffers(1, &gpuBuffers->EAO);

  glBindVertexArray(gpuBuffers->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, gpuBuffers->VBO);
  glBufferData(GL_ARRAY_BUFFER, GPU_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuBuffers->EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, GPU_BUFFER_SIZE, NULL, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

internal void clearBackground(Color color) {
  glClearColor(NormalizeColorChannel(color.r), NormalizeColorChannel(color.g),
               NormalizeColorChannel(color.b), NormalizeColorChannel(color.a));
  glClear(GL_COLOR_BUFFER_BIT);
}

internal void drawLine(V2f start, V2f end, u32 shaderProgram, u32 *VAO,
                       u32 *VBO) {
  f32 vertices[] = {start.x, start.y, 0.0, end.x, end.y, 0.0f};

  glUseProgram(shaderProgram);
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glLineWidth(1.0f);
  glDrawArrays(GL_LINES, 0, 2);
}

internal void drawTrinagle(V2f v1, V2f v2, V2f v3, u32 shaderProgram, u32 *VAO,
                           u32 *VBO) {
  f32 vertices[] = {
      v1.x, v1.y, 0.0f, v2.x, v2.y, 0.0f, v3.x, v3.y, 0.0f,
  };

  glUseProgram(shaderProgram);
  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

internal void drawRectangle(Rectangle rect, u32 shaderProgram,
                            GpuBuffers *gpubuffers) {
  float vertices[] = {
      rect.x,
      rect.y,
      0.0f,
      rect.x + rect.width,
      rect.y,
      0.0f,
      rect.x + rect.width,
      rect.y + rect.height,
      0.0f,
      rect.x,
      rect.y + rect.height,
      0.0f,
  };

  u32 indices[] = {
      0, 1, 2, 0, 2, 3,
  };

  glUseProgram(shaderProgram);
  glBindVertexArray(gpubuffers->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, gpubuffers->VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpubuffers->EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

internal void drawPolygon(Arena *frameArena, V2f center, u32 segments,
                          f32 radius, u32 shaderProgram,
                          GpuBuffers *gpubuffers) {
  u32 vertexCount = segments + 2;
  f32 *vertices = (f32 *)arenaAlloc(frameArena, vertexCount * 3 * sizeof(f32));
  vertices[0] = center.x;
  vertices[1] = center.y;
  vertices[2] = 0.0f;

  for (i32 i = 0; i <= segments; i++) {
    f32 theta = 2.0f * PI * f32(i) / f32(segments);
    f32 x = radius * cosf(theta);
    f32 y = radius * sinf(theta);

    u32 verticesIndex = (i + 1) * 3;
    vertices[verticesIndex] = x;
    vertices[verticesIndex + 1] = y;
    vertices[verticesIndex + 2] = 0.0f;
  }

  glUseProgram(shaderProgram);
  glBindVertexArray(gpubuffers->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, gpubuffers->VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(f32), vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

int main(void) {

  init();
  i32 windowWidth = 800;
  i32 windowHeight = 600;
  Window *window = createWindow(windowWidth, windowHeight, "open gl");

  u32 vertexShader;
  u32 fragmentShader;
  u32 shaderProgram;
  loadShaders(vertexShader, fragmentShader, &shaderProgram);
  GpuBuffers gpubuffers;
  initGpu(&gpubuffers);
  u8 frameBuffer[4096];
  Arena frameArena = {};
  frameArena = arenaInit(frameBuffer, 4096);

  Color backgroundColor = {
      .r = 0x11,
      .g = 0x11,
      .b = 0x11,
      .a = 0xff,
  };
  V2f center = {
      .x = 0.0f,
      .y = 0.0f,
  };

  V2f startx = {
      .x = 1.0f,
      .y = 0.0f,
  };

  V2f endx = {
      .x = -1.0f,
      .y = 0.0f,
  };

  V2f starty = {
      .x = 0.0f,
      .y = 1.0f,
  };

  V2f endy = {
      .x = 0.0f,
      .y = -1.0f,
  };

  V2f v1 = {
      .x = -0.5f,
      .y = -0.5f,
  };
  V2f v2 = {
      .x = 0.5f,
      .y = -0.5f,
  };
  V2f v3 = {
      .x = 0.5f,
      .y = 0.5f,
  };
  Rectangle rect = {
      .x = 0.0f,
      .y = 0.0f,
      .width = 1.0f,
      .height = 1.0f,
  };

  while (!glfwWindowShouldClose(window)) {
    arenaReset(&frameArena);
    clearBackground(backgroundColor);

    // drawLine(startx, endx, shaderProgram, &VBO, &VAO);
    // drawLine(starty, endy, shaderProgram, &VBO, &VAO);
    // drawTrinagle(v1, v2, v3, shaderProgram, &VAO, &VBO);
    // drawRectangle(rect, shaderProgram, &gpubuffers);
    drawPolygon(&frameArena, center, 6, 0.5f, shaderProgram, &gpubuffers);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  destroyWindow(window);
  exit(EXIT_SUCCESS);
}
