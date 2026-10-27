#include "gui.h"

internal b32 checkCollisionBetweenMouseAndRect(Rectangle rect) {
  if (MOUSE_POSITION.x > rect.x && MOUSE_POSITION.x < rect.x + rect.width &&
      MOUSE_POSITION.y > rect.y && MOUSE_POSITION.y < rect.y + rect.height) {
    return true;
  }
  return false;
}

internal ButtonState guiButton(Rectangle button, const char *text, Font *font,
                               Color buttonColor, Color textColor) {
  f32 textWidth = getTextWidth(text, font);
  ButtonState buttonState = IDLE;
  // if (checkCollisionBetweenMouseAndRect(button)) {
  //   if (isMouseButtonDown(0)) {
  //     buttonState = DOWN;
  //   } else if (isMouseButtonReleased(0)) {
  //     buttonState = RELEASED;
  //   } else {
  //     buttonState = HOVERED;
  //   }
  // }
  V2f textPos = {
      .x = button.x + button.width / 2 - textWidth / 2,
      .y = button.y + button.height / 2,
  };
  drawRectangle(button, buttonColor);
  drawText(textPos, text, font, textColor);
  return buttonState;
}
