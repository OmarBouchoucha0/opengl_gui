#pragma once
#include "render.h"

internal ColorNormolized normolizeColor(Color color) {
  f32 r = NormalizeColorChannel(color.r);
  f32 g = NormalizeColorChannel(color.g);
  f32 b = NormalizeColorChannel(color.b);
  f32 a = NormalizeColorChannel(color.a);
  return (ColorNormolized){r, g, b, a};
}

internal void errorCallback(int error, const char *description) {
  traceLog(ERROR, (char *)description);
}

internal void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

internal void initGpu() {
  // NOTE: (omar) load shaders
  GPU_STATE.vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "layout (location = 1) in vec4 aColor;\n"
      "out vec4 color;"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "   color = aColor;\n"
      "}\0";

  GPU_STATE.fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 fragColor;\n"
                                   "in vec4 color;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   fragColor = color;\n"
                                   "}\n\0";

  GPU_STATE.vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GPU_STATE.vertexShader, 1, &GPU_STATE.vertexShaderSource,
                 NULL);
  glCompileShader(GPU_STATE.vertexShader);

  GPU_STATE.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GPU_STATE.fragmentShader, 1, &GPU_STATE.fragmentShaderSource,
                 NULL);
  glCompileShader(GPU_STATE.fragmentShader);

  GPU_STATE.shaderProgram = glCreateProgram();
  glAttachShader(GPU_STATE.shaderProgram, GPU_STATE.vertexShader);
  glAttachShader(GPU_STATE.shaderProgram, GPU_STATE.fragmentShader);
  glLinkProgram(GPU_STATE.shaderProgram);

  glDeleteShader(GPU_STATE.vertexShader);
  glDeleteShader(GPU_STATE.fragmentShader);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_MULTISAMPLE);

  // NOTE: (omar) generate buffers
  glGenVertexArrays(1, &GPU_STATE.VAO);
  glGenBuffers(1, &GPU_STATE.VBO);
  glGenBuffers(1, &GPU_STATE.EAO);

  glBindVertexArray(GPU_STATE.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferData(GL_ARRAY_BUFFER, GPU_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

  // position (location 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // color (location 1)
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glUseProgram(GPU_STATE.shaderProgram);
}

internal Window *createWindow(u32 windowWidth, u32 windowHeight,
                              const char *title) {
  glfwSetErrorCallback(errorCallback);
  // NOTE: (omar) forcing x11 just to mimic raylib window behaviour since thats
  // what i prefer
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  WINDOW_WIDTH = windowWidth;
  WINDOW_HEIGHT = windowHeight;
  Window *window = glfwCreateWindow(windowWidth, windowHeight,
                                    "graphics library", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // NOTE: (omar) centering window
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  i32 monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);
  glfwSetWindowPos(window, monitorX + (mode->width - (i32)windowWidth) / 2,
                   monitorY + (mode->height - (i32)windowHeight) / 2);
  glfwSetKeyCallback(window, keyCallback);
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, windowWidth, windowHeight);
  initGpu();
  return window;
}

internal void destroyWindow(Window *window) {
  glDeleteVertexArrays(1, &GPU_STATE.VAO);
  glDeleteBuffers(1, &GPU_STATE.VBO);
  glDeleteBuffers(1, &GPU_STATE.EAO);
  glDeleteProgram(GPU_STATE.shaderProgram);
  glfwDestroyWindow(window);
  glfwTerminate();
}

internal void clearBackground(Color color) {
  glClearColor(NormalizeColorChannel(color.r), NormalizeColorChannel(color.g),
               NormalizeColorChannel(color.b), NormalizeColorChannel(color.a));
  glClear(GL_COLOR_BUFFER_BIT);
}

// TODO: (omar) use Orthographic Projection instead
internal V2f fromNDCtoScreenSpace(V2f NDC) {
  V2f ScreenSpace = {};
  ScreenSpace.x = (NDC.x + 1.0f) / 2.0f * WINDOW_WIDTH;
  ScreenSpace.y = (1.0f - NDC.y) / 2.0f * WINDOW_HEIGHT;
  return ScreenSpace;
}

internal V2f fromScreenSpacetoNDC(V2f ScreenSpace) {
  V2f NDC = {};
  NDC.x = (ScreenSpace.x / WINDOW_WIDTH) * 2.0f - 1.0f;
  NDC.y = 1.0f - (ScreenSpace.y / WINDOW_HEIGHT) * 2.0f;
  return NDC;
}

internal void drawLine(V2f start, V2f end, Color color) {
  V2f NDCStart = fromScreenSpacetoNDC(start);
  V2f NDCEnd = fromScreenSpacetoNDC(end);

  f32 r = NormalizeColorChannel(color.r);
  f32 g = NormalizeColorChannel(color.g);
  f32 b = NormalizeColorChannel(color.b);
  f32 a = NormalizeColorChannel(color.a);

  f32 vertices[] = {
      NDCStart.x, NDCStart.y, 0.0f, r, g, b, a,
      NDCEnd.x,   NDCEnd.y,   0.0f, r, g, b, a,
  };

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glLineWidth(1.0f);
  glDrawArrays(GL_LINES, 0, 2);
}

internal void drawLineEx(V2f start, V2f end, f32 thickness, Color color) {
  f32 angle = RadianToDegree(angleBetweenTwoPoints(start, end));
  Rectangle rect = {};
  rect.x = start.x + thickness / 2.0f;
  rect.y = start.y + thickness / 2.0f;
  rect.width = distanceBetweenTwoPoints(start, end);
  rect.height = thickness;
  drawRectangleEx(rect, {rect.x, rect.y}, angle, color);
}

internal void drawTrinagle(V2f v1, V2f v2, V2f v3, Color color) {

  V2f v1NDC = fromScreenSpacetoNDC(v1);
  V2f v2NDC = fromScreenSpacetoNDC(v2);
  V2f v3NDC = fromScreenSpacetoNDC(v3);

  f32 r = NormalizeColorChannel(color.r);
  f32 g = NormalizeColorChannel(color.g);
  f32 b = NormalizeColorChannel(color.b);
  f32 a = NormalizeColorChannel(color.a);

  f32 vertices[] = {
      v1NDC.x, v1NDC.y, 0.0f, r, g, b, a, // v1
      v2NDC.x, v2NDC.y, 0.0f, r, g, b, a, // v2
      v3NDC.x, v3NDC.y, 0.0f, r, g, b, a, // v3
  };

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

// TODO: (omar) fix the inntersection of the lines
internal void drawTrinagleOutline(V2f v1, V2f v2, V2f v3, f32 thickness,
                                  Color color) {
  drawLineEx(v1, v2, thickness, color);
  drawLineEx(v2, v3, thickness, color);
  drawLineEx(v3, v1, thickness, color);
}

internal void drawRectangle(Rectangle rect, Color color) {
  V2f topLeftNDC = fromScreenSpacetoNDC((V2f){rect.x, rect.y});
  V2f bottomRightNDC =
      fromScreenSpacetoNDC((V2f){rect.x + rect.width, rect.y + rect.height});

  f32 r = NormalizeColorChannel(color.r);
  f32 g = NormalizeColorChannel(color.g);
  f32 b = NormalizeColorChannel(color.b);
  f32 a = NormalizeColorChannel(color.a);

  f32 vertices[] = {
      topLeftNDC.x,     topLeftNDC.y,     0.0f, r, g, b, a,
      bottomRightNDC.x, topLeftNDC.y,     0.0f, r, g, b, a,
      bottomRightNDC.x, bottomRightNDC.y, 0.0f, r, g, b, a,
      topLeftNDC.x,     bottomRightNDC.y, 0.0f, r, g, b, a,
  };
  u32 indices[] = {
      0, 1, 2, 0, 2, 3,
  };

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GPU_STATE.EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

internal void drawRectangleEx(Rectangle rect, V2f origin, f32 rotation,
                              Color color) {

  f32 theta = DegreeToRadian(rotation);
  f32 left = rect.x - origin.x;
  f32 right = rect.x + rect.width - origin.x;
  f32 top = rect.y - origin.y;
  f32 bottom = rect.y + rect.height - origin.y;

  V2f tl = rotatePointRadian({left, top}, theta);
  V2f tr = rotatePointRadian({right, top}, theta);
  V2f bl = rotatePointRadian({left, bottom}, theta);
  V2f br = rotatePointRadian({right, bottom}, theta);

  V2f topLeft = {origin.x + tl.x, origin.y + tl.y};
  V2f topRight = {origin.x + tr.x, origin.y + tr.y};
  V2f bottomLeft = {origin.x + bl.x, origin.y + bl.y};
  V2f bottomRight = {origin.x + br.x, origin.y + br.y};

  V2f topLeftNDC = fromScreenSpacetoNDC(topLeft);
  V2f topRightNDC = fromScreenSpacetoNDC(topRight);
  V2f bottomLeftNDC = fromScreenSpacetoNDC(bottomLeft);
  V2f bottomRightNDC = fromScreenSpacetoNDC(bottomRight);

  ColorNormolized colorNormolized = normolizeColor(color);

  f32 vertices[] = {
      topLeftNDC.x,
      topLeftNDC.y,
      0.0f,
      colorNormolized.r,
      colorNormolized.g,
      colorNormolized.b,
      colorNormolized.a,
      topRightNDC.x,
      topRightNDC.y,
      0.0f,
      colorNormolized.r,
      colorNormolized.g,
      colorNormolized.b,
      colorNormolized.a,
      bottomRightNDC.x,
      bottomRightNDC.y,
      0.0f,
      colorNormolized.r,
      colorNormolized.g,
      colorNormolized.b,
      colorNormolized.a,
      bottomLeftNDC.x,
      bottomLeftNDC.y,
      0.0f,
      colorNormolized.r,
      colorNormolized.g,
      colorNormolized.b,
      colorNormolized.a,
  };
  u32 indices[] = {
      0, 1, 2, 0, 2, 3,
  };

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GPU_STATE.EAO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

internal void drawRectangleOutline(Rectangle rect, f32 thickness, Color color) {
  Rectangle top = {rect.x, rect.y, rect.width, thickness};
  Rectangle bottom = {rect.x, rect.y + rect.height - thickness, rect.width,
                      thickness};
  Rectangle left = {rect.x, rect.y + thickness, thickness,
                    rect.height - 2 * thickness};
  Rectangle right = {rect.x + rect.width - thickness, rect.y + thickness,
                     thickness, rect.height - 2 * thickness};
  drawRectangle(top, color);
  drawRectangle(bottom, color);
  drawRectangle(left, color);
  drawRectangle(right, color);
}

internal void drawPolygon(V2f center, u32 segments, f32 radius, Color color) {

  V2f centerNDC = fromScreenSpacetoNDC(center);
  f32 radiusX = (radius / WINDOW_WIDTH) * 2.0f;
  f32 radiusY = (radius / WINDOW_HEIGHT) * 2.0f;
  u32 vertexCount = segments + 2;
  f32 vertices[7 * vertexCount];
  vertices[0] = centerNDC.x;
  vertices[1] = centerNDC.y;
  vertices[2] = 0.0f;
  vertices[3] = NormalizeColorChannel(color.r);
  vertices[4] = NormalizeColorChannel(color.g);
  vertices[5] = NormalizeColorChannel(color.b);
  vertices[6] = NormalizeColorChannel(color.a);

  for (i32 i = 0; i < segments + 1; i++) {
    f32 theta = 2.0f * PI * f32(i) / f32(segments);
    f32 x = centerNDC.x + radiusX * cosf(theta);
    f32 y = centerNDC.y + radiusY * sinf(theta);

    u32 verticesIndex = (i + 1) * 7;
    vertices[verticesIndex] = x;
    vertices[verticesIndex + 1] = y;
    vertices[verticesIndex + 2] = 0.0f;
    vertices[verticesIndex + 3] = NormalizeColorChannel(color.r);
    vertices[verticesIndex + 4] = NormalizeColorChannel(color.g);
    vertices[verticesIndex + 5] = NormalizeColorChannel(color.b);
    vertices[verticesIndex + 6] = NormalizeColorChannel(color.a);
  }

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 7 * sizeof(f32), vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

internal void drawCircle(V2f center, f32 radius, Color color) {
  V2f centerNDC = fromScreenSpacetoNDC(center);
  f32 radiusX = (radius / WINDOW_WIDTH) * 2.0f;
  f32 radiusY = (radius / WINDOW_HEIGHT) * 2.0f;
  u32 segments = 50;
  u32 vertexCount = segments + 2;
  f32 vertices[vertexCount * 7];
  vertices[0] = centerNDC.x;
  vertices[1] = centerNDC.y;
  vertices[2] = 0.0f;
  vertices[3] = NormalizeColorChannel(color.r);
  vertices[4] = NormalizeColorChannel(color.g);
  vertices[5] = NormalizeColorChannel(color.b);
  vertices[6] = NormalizeColorChannel(color.a);

  for (i32 i = 0; i < segments + 1; i++) {
    f32 theta = 2.0f * PI * f32(i) / f32(segments);
    f32 x = centerNDC.x + radiusX * cosf(theta);
    f32 y = centerNDC.y + radiusY * sinf(theta);

    u32 verticesIndex = (i + 1) * 7;
    vertices[verticesIndex] = x;
    vertices[verticesIndex + 1] = y;
    vertices[verticesIndex + 2] = 0.0f;
    vertices[verticesIndex + 3] = NormalizeColorChannel(color.r);
    vertices[verticesIndex + 4] = NormalizeColorChannel(color.g);
    vertices[verticesIndex + 5] = NormalizeColorChannel(color.b);
    vertices[verticesIndex + 6] = NormalizeColorChannel(color.a);
  }

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 7 * sizeof(f32), vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

internal void drawCircleOutline(V2f center, f32 radius, Color color) {
  V2f centerNDC = fromScreenSpacetoNDC(center);
  f32 radiusX = (radius / WINDOW_WIDTH) * 2.0f;
  f32 radiusY = (radius / WINDOW_HEIGHT) * 2.0f;
  u32 segments = 500;
  u32 vertexCount = segments + 2;
  f32 vertices[7 * vertexCount];
  for (i32 i = 0; i < segments + 1; i++) {
    f32 theta = 2.0f * PI * f32(i) / f32(segments);
    f32 x = centerNDC.x + radiusX * cosf(theta);
    f32 y = centerNDC.y + radiusY * sinf(theta);

    u32 verticesIndex = i * 7;
    vertices[verticesIndex] = x;
    vertices[verticesIndex + 1] = y;
    vertices[verticesIndex + 2] = 0.0f;
    vertices[verticesIndex + 3] = NormalizeColorChannel(color.r);
    vertices[verticesIndex + 4] = NormalizeColorChannel(color.g);
    vertices[verticesIndex + 5] = NormalizeColorChannel(color.b);
    vertices[verticesIndex + 6] = NormalizeColorChannel(color.a);
  }

  glBindBuffer(GL_ARRAY_BUFFER, GPU_STATE.VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * 7 * sizeof(f32), vertices);

  glDrawArrays(GL_POINTS, 0, vertexCount);
}
