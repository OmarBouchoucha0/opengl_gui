#ifndef RENDER_H
#define RENDER_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "../base/base_types.cpp"
#include "../base/base_math.cpp"
#include "../base/base_arena.cpp"
#include "../base/base_strings.cpp"
#include "../base/base_log.cpp"

#define GPU_BUFFER_SIZE Megabytes(1)
#define NormalizeColorChannel(color) ((float)(color) / 255.0f)

struct Color {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

struct ColorNormolized {
  f32 r;
  f32 g;
  f32 b;
  f32 a;
};

struct Font {
  u32 texture;
  stbtt_bakedchar glyphs[96]; // ASCII 32..126
  f32 size;
};

struct GpuState {
  const char *vertexShaderSource;
  i32 vertexShader;

  const char *fragmentShaderSource;
  i32 fragmentShader;

  u32 shaderProgram;
  u32 textProgram;
  u32 VAO;
  u32 VBO;
  u32 textVAO;
  u32 textVBO;
  u32 EAO;
};

typedef GLFWwindow Window;

global_variable GpuState GPU_STATE = {};
global_variable u32 WINDOW_WIDTH;
global_variable u32 WINDOW_HEIGHT;

internal ColorNormolized normolizeColor(Color color);
internal Window *createWindow(u32 windowWidth, u32 windowHeight,
                              const char *title);
internal void destroyWindow(Window *window);
internal void clearBackground(Color color);
internal void drawLine(V2f start, V2f end, Color color);
internal void drawLineEx(V2f start, V2f end, f32 thickness, Color color);
internal void drawTrinagle(V2f v1, V2f v2, V2f v3, Color color);
internal void drawTrinagleEx(V2f v1, V2f v2, V2f v3, f32 rotation, Color color);
internal void drawTrinagleOutline(V2f v1, V2f v2, V2f v3, f32 thickness,
                                  Color color);
internal void drawRectangle(Rectangle rect, Color color);
internal void drawRectangleEx(Rectangle rect, V2f origin, f32 rotation,
                              Color color);
internal void drawRectangleOutline(Rectangle rect, f32 thickness, Color color);
internal void drawPolygon(V2f center, u32 segments, f32 radius, Color color);
internal void drawCircle(V2f center, f32 radius, Color color);
internal void drawCircleOutline(V2f center, f32 radius, f32 thickness,
                                Color color);
internal Font loadFont(const char *path, f32 pixelHeight);
internal void drawText(V2f pos, const char *text, Font *font, Color color);

#endif // RENDER_H
