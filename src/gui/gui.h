#ifndef GUI_H
#define GUI_H

#include "../base/base_types.cpp"
#include "../base/base_math.cpp"
#include "../render/render.cpp"

enum ButtonState {
  IDLE,
  HOVERED,
  DOWN,
  RELEASED,
};

internal ButtonState guiButton(Rectangle button, const char *text, Font *font,
                               Color buttonColor, Color textColor);

#endif // GUI_H
