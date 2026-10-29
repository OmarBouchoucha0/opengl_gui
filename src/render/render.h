#ifndef RENDER_H
#define RENDER_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <string.h>

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
  f32 ascent;
  f32 descent;
  f32 lineHeight;
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

typedef GLFWwindow *Window;

struct Input {
  b32 keyDown[GLFW_KEY_LAST + 1];
  b32 keyPrev[GLFW_KEY_LAST + 1];
  b32 mouseDown[8];
  b32 mousePrev[8];
  V2f mousePos;
  V2f mouseDelta;
  f32 scrollDelta;
};

global_variable GpuState GPU_STATE;
global_variable Window WINDOW;
global_variable u32 WINDOW_WIDTH;
global_variable u32 WINDOW_HEIGHT;
global_variable V2f MOUSE_POSITION;
global_variable Input INPUT;

internal ColorNormolized normolizeColor(Color color);
internal Window createWindow(u32 windowWidth, u32 windowHeight,
                             const char *title);
internal b32 windowShouldClose();
internal void destroyWindow();
internal void handleEvents();
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
internal void drawRectangleRounded(Rectangle rect, V2f origin, f32 rotation,
                                   float roundness, Color color);
internal void drawPolygon(V2f center, u32 segments, f32 radius, Color color);
internal void drawCircle(V2f center, f32 radius, Color color);
internal void drawCircleOutline(V2f center, f32 radius, f32 thickness,
                                Color color);
internal Font loadFont(const char *path, f32 pixelHeight);
internal void drawText(V2f pos, const char *text, Font *font, Color color);
internal f32 getTextWidth(const char *text, Font *font);
internal V2f getMousePosition();
internal b32 isMouseButtonDown(i32 key);
internal b32 isMouseButtonPressed(i32 key);
internal b32 isMouseButtonReleased(i32 key);
internal b32 isKeyButtonDown(i32 key);
internal b32 isKeyButtonPressed(i32 key);
internal b32 isKeyButtonReleased(i32 key);

#endif // RENDER_H
