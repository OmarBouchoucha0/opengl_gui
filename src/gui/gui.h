#ifndef GUI_H
#define GUI_H

#include "../base/base_types.cpp"
#include "../base/base_math.cpp"
#include "../render/render.cpp"

enum ButtonState {
  BUTTON_IDLE,
  BUTTON_HOVERED,
  BUTTON_DOWN,
  BUTTON_RELEASED,
};

internal ButtonState guiButton(Rectangle button, const char *text, Font *font,
                               Color buttonColor, Color textColor);

#endif // GUI_H
