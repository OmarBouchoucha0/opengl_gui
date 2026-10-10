#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main() {
  if (!glfwInit()) {
    return -1;
  }
  GLFWwindow *window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  if (!window) {
    printf("Window creation failed");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("glad load failed");
    glfwTerminate();
    return -1;
  }
  printf("OpenGL Loaded: %s\n", glGetString(GL_VERSION));

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
