#include "gui.h"

internal b32 checkCollisionBetweenMouseAndRect(Rectangle rect) {
  V2f mouse_position = getMousePosition();
  if (mouse_position.x > rect.x && mouse_position.x < rect.x + rect.width &&
      mouse_position.y > rect.y && mouse_position.y < rect.y + rect.height) {
    return true;
  }
  return false;
}

internal ButtonState guiButton(Rectangle button, const char *text, Font *font,
                               Color buttonColor, Color textColor) {

  f32 textWidth = getTextWidth(text, font);
  ButtonState buttonState = BUTTON_IDLE;
  if (checkCollisionBetweenMouseAndRect(button)) {
    if (isMouseButtonDown(0)) {
      buttonState = BUTTON_DOWN;
    } else if (isMouseButtonReleased(0)) {
      buttonState = BUTTON_RELEASED;
    } else {
      buttonState = BUTTON_HOVERED;
    }
  }
  V2f textPos = {
      .x = button.x + button.width / 2 - textWidth / 2,
      .y = button.y + button.height / 2,
  };
  drawRectangle(button, buttonColor);
  drawText(textPos, text, font, textColor);
  return buttonState;
}
